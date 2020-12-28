#pragma once

#include "ICanStringEnvVar.h"
#include "CanEnvVar.h"

class CanStringEnvVar : public CanEnvVar, public ICanStringEnvVar
{
public:
   CanStringEnvVar() : CanEnvVar(Type_e::STRING) {}
   ~CanStringEnvVar();

   void Clear(void) override;
   const char* GetName(void) const override;
   Type_e GetType(void) const override;
   const char* GetUnit(void) const override;
   uint32_t GetId(void) const override;   // obsolete
   AccessType_e GetAccessType(void) const override;
   size_t GetAccessNodesCount(void) const override;
   ICanNode* GetAccessNodeByIndex(size_t index) const override;
   ICanNode* GetAccessNodeByName(const char* name) const override;
   ICanValueTable* GetValueTable(void) const override;

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;
};