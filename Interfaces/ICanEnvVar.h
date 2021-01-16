#pragma once

#include "ICanAttributeOwner.h"
#include "ICanNode.h"
#include "ICanValueTable.h"

class ICanNetwork; // circular dependency

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
   virtual void ModifyName(const char* name) = 0;

   virtual Type_e GetType(void) const = 0;
   virtual void ModifyType(Type_e type) = 0;

   virtual const char * GetUnit(void) const = 0;
   virtual void ModifyUnit(const char* unit) = 0;

   virtual uint32_t GetId(void) const = 0;   // obsolete
   virtual void ModifyId(uint32_t id) = 0;   // obsolete

   virtual AccessType_e GetAccessType(void) const = 0;
   virtual void ModifyAccessType(AccessType_e accessType) = 0;

   virtual size_t GetAccessNodesCount(void) const = 0;
   virtual ICanNode* GetAccessNodeByIndex(size_t index) const = 0;
   virtual ICanNode* GetAccessNodeByName(const char* name) const = 0;
   virtual ICanValueTable* GetValueTable(void) const = 0;

   virtual const char* GetComment(void) const = 0;
   virtual void ModifyComment(const char* comment) = 0;

   virtual ICanNetwork* GetNetwork(void) const = 0;

   static constexpr const char * ACCESS_TYPE_PREFIX = "DUMMY_NODE_VECTOR";
};