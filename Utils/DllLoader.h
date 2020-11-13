#pragma once
#include <Windows.h>

template <typename IClass, typename pfDelete_rV_t = bool>
class DllLoader
{
private:

   // Create function type definition
   using pfCreate_t = IClass * (*) (void);

   // Delete function type definition
   using pfDelete_t = pfDelete_rV_t(*) (IClass*);

public:
   // Pointer to create function
   pfCreate_t pfCreate { NULL };

   // Pointer to delete function
   pfDelete_t pfDelete { NULL };

   DllLoader() = delete;
   DllLoader(const char* dllName, const char* pfCreateName, const char* pfDeleteName) :
      dllName(dllName), pfCreateName(pfCreateName), pfDeleteName(pfDeleteName) {}
   ~DllLoader()
   {
      this->Clear();
   };

   bool Load()
   {
      // locals
      bool rV = false;
      PCSTR hDllModule = this->dllName;

      // DLL not loaded?
      if(this->dllHandler == NULL)
      {
         // Is DLL loaded?
         if(GetModuleHandleA(hDllModule) == NULL)
         {
            // not loaded, so load DLL
            this->dllHandler = LoadLibraryA(hDllModule);
            // check if DLL loaded successful
            if(this->dllHandler != NULL)
            {
               // requested module name exists?
               if(GetModuleHandleA(hDllModule) != NULL)
               {
                  // apply function pointer
                  this->pfCreate = (pfCreate_t)GetProcAddress(this->dllHandler, this->pfCreateName);
                  // apply function pointer
                  this->pfDelete = (pfDelete_t)GetProcAddress(this->dllHandler, this->pfDeleteName);
                  // function exists?
                  if((this->pfCreate != NULL) && (this->pfDelete != NULL))
                  {
                     // success
                     rV = true;
                  }
               }
            }
         }
      }
      return rV;
   };

   bool Clear()
   {
      this->pfCreate = NULL;
      this->pfDelete = NULL;
      this->dllHandler = NULL;
      PCSTR hDllModule = this->dllName;
      return FreeLibrary(GetModuleHandleA(hDllModule)) != 0;
   }

   bool IsLoaded()
   {
      return (this->pfCreate != NULL) && (this->pfDelete != NULL);
   }

private:

   const char* dllName { nullptr };
   //function names from dll
   const char* pfCreateName { nullptr };
   const char* pfDeleteName { nullptr };

   // Handle to DLL
   HINSTANCE dllHandler { NULL };
};
