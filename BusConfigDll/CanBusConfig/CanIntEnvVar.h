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
   void ModifyName(const char* name) override;

   Type_e GetType(void) const override;
   void ModifyType(Type_e type) override;

   const char* GetUnit(void) const override;
   void ModifyUnit(const char* unit) override;

   uint32_t GetId(void) const override;   // obsolete
   void ModifyId(uint32_t id) override;   // obsolete

   AccessType_e GetAccessType(void) const override;
   void ModifyAccessType(AccessType_e accessType) override;

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

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;
   void ModifyComment(const char* comment) override;

private:
   int32_t minimum {};
   int32_t maximum {};
   int32_t initialValue {};
};