#pragma once

#include "ICanFloatEnvVar.h"
#include "CanEnvVar.h"

class CanFloatEnvVar : public CanEnvVar, public ICanFloatEnvVar
{
public:
   CanFloatEnvVar() : CanEnvVar(Type_e::FLOAT) {}
   ~CanFloatEnvVar();

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

   double GetMinimum(void) const override;
   void SetMinimum(double minimum);

   double GetMaximum(void) const override;
   void SetMaximum(double maximum);

   double GetInitialValue(void) const override;
   void SetInitialValue(double initialValue);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;
   void ModifyComment(const char* comment) override;

private:
   double minimum {};
   double maximum {};
   double initialValue {};
};