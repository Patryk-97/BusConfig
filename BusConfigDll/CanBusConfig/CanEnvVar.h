#pragma once

#include "ICanEnvVar.h"
#include "CanValueTable.h"
#include "CanAttributeOwner.h"
#include "CanNode.h"
#include <string>

class CanEnvVar : public CanAttributeOwner, public ICanEnvVar
{
public:
   CanEnvVar(Type_e type) :
      CanAttributeOwner(ICanAttribute::IObjectType_e::ENVIRONMENT_VARIABLE), type(type) {}
   ~CanEnvVar();

   void Clear(void) override;

   const char* GetName(void) const override;
   void ModifyName(const char* name) override;
   void SetName(const char * name);

   Type_e GetType(void) const override;
   void ModifyType(Type_e type) override;
   void SetType(Type_e type);

   const char* GetUnit(void) const override;
   void ModifyUnit(const char* unit) override;
   void SetUnit(const char* unit);

   uint32_t GetId(void) const override;   // obsolete
   void ModifyId(uint32_t id) override;   // obsolete
   void SetId(uint32_t id);

   AccessType_e GetAccessType(void) const override;
   void ModifyAccessType(AccessType_e accessType) override;
   void SetAccessType(AccessType_e accessType);
   void SetAccessTypeValue(uint32_t accessTypeValue);

   size_t GetAccessNodesCount(void) const override;
   ICanNode* GetAccessNodeByIndex(size_t index) const override;
   ICanNode* GetAccessNodeByName(const char* name) const override;
   std::vector<CanNode*> GetAccessNodes(void) const;
   void AddAccessNode(CanNode* accessNode);

   ICanValueTable* GetValueTable(void) const override;
   void SetValueTable(CanValueTable* valueTable);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;
   void ModifyComment(const char* comment) override;
   void SetComment(const char* comment);

private:
   std::string name;
   Type_e type { Type_e::UNDEFINED };
   std::string unit;
   uint32_t id {};
   AccessType_e accessType { AccessType_e::UNDEFINED };
   std::vector<CanNode*> accessNodes;
   CanValueTable* valueTable { nullptr };
   std::string comment;
};