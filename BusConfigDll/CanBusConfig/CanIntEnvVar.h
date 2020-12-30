#pragma once

#include "ICanIntEnvVar.h"
#include "CanEnvVar.h"

class CanIntEnvVar : public CanEnvVar, public ICanIntEnvVar
{
public:
   CanIntEnvVar() : CanEnvVar(Type_e::INTEGER) {}
   ~CanIntEnvVar();

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
   
   int32_t GetMinimum(void) const override;
   void SetMinimum(int32_t minimum);

   int32_t GetMaximum(void) const override;
   void SetMaximum(int32_t maximum);
   
   int32_t GetInitialValue(void) const override;
   void SetInitialValue(int32_t initialValue);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;

private:
   int32_t minimum {};
   int32_t maximum {};
   int32_t initialValue {};
};