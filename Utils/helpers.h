#pragma once

#include <span>
#include <functional>
#include <string_view>
#include <string>
#include <algorithm>

namespace ranges = std::ranges;

namespace helpers
{
   template <typename T, typename = void>
   struct is_iterable : std::false_type {};

   template <typename T>
   struct is_iterable<T, std::void_t<decltype(std::declval<T&>().begin(), std::declval<T&>().end())>> : std::true_type {};

   template <typename T>
   inline constexpr bool is_iterable_v = is_iterable<T>::value;

   template <typename T, typename = void>
   struct is_stl_container : std::false_type {};

   template <typename T>
   struct is_stl_container<T, std::void_t<decltype(is_iterable_v<T>, std::declval<T&>().clear()), typename T::value_type>> : std::true_type {};

   template <typename T>
   inline constexpr bool is_stl_container_v = is_stl_container<T>::value;
   
   template <typename T,
      typename = std::enable_if_t<std::is_pointer_v<T>>>
   inline void ClearPtr(T& ptr) { if (ptr) { delete ptr; ptr = nullptr; }}

   template <typename StlContainer, typename = std::enable_if_t<is_stl_container_v<StlContainer>>,
      typename = std::enable_if_t<std::is_pointer_v<typename StlContainer::value_type>>>
   inline void ClearContainer(StlContainer& stlContainer) { for (auto& ptr : stlContainer) { delete ptr; }; stlContainer.clear(); }


   /*
      Until MSVC doesn't support this std::span constructor
      template< class It, class End >
      explicit(extent != std::dynamic_extent)
      constexpr span( It first, End last );
   */
   template<typename It>
   constexpr auto make_span(It begin, It end)
   {
      return std::span<std::remove_pointer_t<It::pointer>>(&(*begin), std::distance(begin, end));
   }

   template<typename T> struct remove_class {};
   template<typename C, typename R, typename... A>
   struct remove_class<R(C::*)(A...)> { using type = R(A...); };
   template<typename C, typename R, typename... A>
   struct remove_class<R(C::*)(A...) const> { using type = R(A...); };
   template<typename C, typename R, typename... A>
   struct remove_class<R(C::*)(A...) volatile> { using type = R(A...); };
   template<typename C, typename R, typename... A>
   struct remove_class<R(C::*)(A...) const volatile> { using type = R(A...); };

   template<typename T>
   struct get_signature_impl
   {
      using type = typename remove_class<
         decltype(&std::remove_reference<T>::type::operator())>::type;
   };
   template<typename R, typename... A>
   struct get_signature_impl<R(A...)> { using type = R(A...); };
   template<typename R, typename... A>
   struct get_signature_impl<R(&)(A...)> { using type = R(A...); };
   template<typename R, typename... A>
   struct get_signature_impl<R(*)(A...)> { using type = R(A...); };
   template<typename T> using get_signature = typename get_signature_impl<T>::type;

   template<typename Base, typename T>
   bool typecaseHelper(Base* base, std::function<void(T*)> func)
   {
      if (T* first = dynamic_cast<T*>(base))
      {
         func(first);
         return true;
      }
      else
      {
         return false;
      }
   }

   template<typename Base>
   void typecase(Base*)
   {
      ;
   }

   template<typename Base, typename FirstSubclass, typename... RestOfSubclasses>
   void typecase(
      Base* base,
      FirstSubclass&& first,
      RestOfSubclasses &&... rest)
   {

      using Signature = get_signature<FirstSubclass>;
      using Function = std::function<Signature>;

      if (typecaseHelper(base, (Function)first))
      {
         return;
      }
      else
      {
         typecase(base, rest...);
      }
   }

   static std::string RemovePhrases(const std::string& word, std::string_view phrase)
   {
      std::string rV = word;

      while (true)
      {
         size_t index = rV.find(phrase);
         if (index == std::string::npos)
         {
            break;
         }

         rV.erase(index, phrase.size());
      }

      return rV;
   }

   static inline bool iequals(const std::string& str1, const std::string& str2)
   {
      return ranges::equal(str1, str2, [] (char a, char b) { return std::tolower(a) == std::tolower(b); });
   }

   static inline int icompare(const std::string& str1, const std::string& str2)
   {
      auto it = std::mismatch(str1.begin(), str1.end(), str2.begin(), str2.end(), [] (char a, char b)
         { return std::tolower(a) == std::tolower(b); });
      if (it.first != str1.end() && it.second != str2.end())
      {
         return (std::tolower(*(it.first)) - std::tolower(*(it.second)));
      }
      if (it.first == str1.end() && it.second == str2.end())
      {
         return 0;
      }
      return (int)str1.size() - (int)str2.size();
   }

   static inline bool iless(const std::string& str1, const std::string& str2)
   {
      return icompare(str1, str2) < 0;
   }

   static inline bool igreater(const std::string& str1, const std::string& str2)
   {
      return icompare(str1, str2) > 0;
   }
}