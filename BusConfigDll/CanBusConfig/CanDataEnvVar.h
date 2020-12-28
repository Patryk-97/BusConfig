#pragma once

#include "ICanDataEnvVar.h"
#include "CanEnvVar.h"

class CanDataEnvVar : public CanEnvVar, public ICanDataEnvVar
{
public:
   CanDataEnvVar() : CanEnvVar(Type_e::DATA) {}
   ~CanDataEnvVar();

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

   uint32_t GetLength(void) const override;
   void SetLength(uint32_t length);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

private:
   uint32_t length {};
};