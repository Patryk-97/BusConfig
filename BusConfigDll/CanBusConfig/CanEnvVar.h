#pragma once

#include "ICanEnvVar.h"
#include "CanValueTable.h"
#include "CanAttributeOwner.h"
#include "CanNode.h"
#include <string>

class CanEnvVar : public CanAttributeOwner, public ICanEnvVar
{
public:
   CanEnvVar() : CanAttributeOwner(ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE) {}
   ~CanEnvVar();

   void Clear(void) override;

   const char* GetName(void) const override;
   void SetName(const char * name);

   Type_e GetType(void) const override;
   void SetType(Type_e type);

   const char* GetUnit(void) const override;
   void SetUnit(const char* unit);

   uint32_t GetId(void) const override;   // obsolete
   void SetId(uint32_t id);

   AccessType_e GetAccessType(void) const override;
   void SetAccessType(AccessType_e accessType);

   ICanNode* GetAccessNode(void) const override;
   void SetAccessNode(CanNode* accessNode);

   ICanValueTable* GetValueTable(void) const override;
   void SetValueTable(CanValueTable* valueTable);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

private:
   std::string name;
   Type_e type { Type_e::UNDEFINED };
   std::string unit;
   uint32_t id {};
   AccessType_e accessType { AccessType_e::UNDEFINED };
   CanNode* accessNode { nullptr };
   CanValueTable* valueTable { nullptr };
};