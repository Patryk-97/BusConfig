#pragma once

#include <span>

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
   
   template <typename T>
   inline void ClearPtr(T* ptr) { delete ptr; ptr = nullptr; }

   template <typename StlContainer, typename = std::enable_if_t<is_stl_container_v<StlContainer>>>
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
}