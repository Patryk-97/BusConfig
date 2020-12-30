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
   Type_e GetType(void) const override;
   const char* GetUnit(void) const override;
   uint32_t GetId(void) const override;   // obsolete
   AccessType_e GetAccessType(void) const override;
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

   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;

private:
   double minimum {};
   double maximum {};
   double initialValue {};
};