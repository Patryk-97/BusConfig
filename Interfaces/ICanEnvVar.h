#pragma once

#include "ICanAttributeOwner.h"
#include "ICanNode.h"
#include "ICanValueTable.h"

class ICanEnvVar : public ICanAttributeOwner
{
protected:
   ICanEnvVar() {};
   virtual ~ICanEnvVar() {};
public:
   
   enum class Type_e
   {
      UNDEFINED = -1,
      INTEGER = 0,
      FLOAT = 1,
      STRING = 2,
      DATA = 3
   };

   enum class AccessType_e
   {
      UNDEFINED = -1,
      UNRESTRICTED = 0,
      READ = 1,
      WRITE = 2,
      READ_WRITE = 3
   };

   virtual void Clear(void) = 0;
   virtual const char * GetName(void) const = 0;
   virtual Type_e GetType(void) const = 0;
   virtual const char * GetUnit(void) const = 0;
   virtual uint32_t GetId(void) const = 0;   // obsolete
   virtual AccessType_e GetAccessType(void) const = 0;
   virtual ICanNode* GetAccessNode(void) const = 0;
   virtual ICanValueTable* GetValueTable(void) const = 0;
};