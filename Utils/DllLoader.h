#pragma once
#include <string>
#include <Windows.h>
#include <filesystem>
template <typename IClass, typename ptrFnc_Delete_rV_t = bool>
class DllLoader
{
private:

   const char* dllName;
   //function names from dll
   const char* ptrFnc_Cname;
   const char* ptrFnc_Dname;

   // Handle to DLL
   HINSTANCE hSPoDoFoDLL = NULL;

   // Create function type definition
   using ptrFnc_Create_t = IClass* (*) (void);

   // Delete function type definition
   using ptrFnc_Delete_t = ptrFnc_Delete_rV_t (*) (IClass*);

public:
   // Pointer to create function
   ptrFnc_Create_t pfCreate = NULL;

   // Pointer to delete function
   ptrFnc_Delete_t pfDelete = NULL;

   DllLoader() {};
   DllLoader(const char* Name, const char* Cname, const char* Dname)
   {
      this->dllName = Name;
      this->ptrFnc_Cname = Cname;
      this->ptrFnc_Dname = Dname;
   };
   ~DllLoader() {};

   bool LoadDll()
   {
      //////////////////// load the DLL
      // local varaibles
      bool retVal = false;
      PCSTR pszModuleName = this->dllName;
      // DLL not loaded?
      if(this->hSPoDoFoDLL == NULL)
      {
         // Is DLL loaded?
         if(GetModuleHandleA(pszModuleName) == NULL)
         {
            // not loaded, so load DLL
            this->hSPoDoFoDLL = LoadLibraryA(pszModuleName);
            // check if DLL loaded successful
            if(this->hSPoDoFoDLL != NULL)
            {
               // requested module name exists?
               if(GetModuleHandleA(pszModuleName) != NULL)
               {
                  // apply function pointer
                  this->pfCreate = (ptrFnc_Create_t)GetProcAddress(hSPoDoFoDLL, this->ptrFnc_Cname);
                  // apply function pointer
                  this->pfDelete = (ptrFnc_Delete_t)GetProcAddress(hSPoDoFoDLL, this->ptrFnc_Dname);
                  // function exists?
                  if((this->pfCreate != NULL) && (this->pfDelete != NULL))
                  {
                     // success
                     retVal = true;
                  }
               }
            }
         }
      }
      return(retVal);
   };

   bool UnLoadDll()
   {
      bool rV = false;
      //resetting local values
      this->pfCreate = NULL;
      this->pfDelete = NULL;
      this->hSPoDoFoDLL = NULL;
      PCSTR pszModuleName = this->dllName;

      //Check if freeing library succeseded
      if(FreeLibrary(GetModuleHandleA(pszModuleName)) != 0)
      {
         rV = true;
      }
      return rV;
   }

   bool IsLoaded()
   {
      bool rV = false;
      // pointer fo functions still exists?
      if((this->pfCreate != NULL) && (this->pfDelete != NULL))
      {
         rV = true;
      }
      return rV;
   }
};
