#pragma once

#include "ICanFloatEnvVar.h"
#include "CanEnvVar.h"

class CanFloatEnvVar : public CanEnvVar, public ICanFloatEnvVar
{
public:
   CanFloatEnvVar() = default;
   ~CanFloatEnvVar();

   void Clear(void) override;
   const char* GetName(void) const override;
   Type_e GetType(void) const override;
   const char* GetUnit(void) const override;
   uint32_t GetId(void) const override;   // obsolete
   AccessType_e GetAccessType(void) const override;
   ICanNode* GetAccessNode(void) const override;

   double GetMinimum(void) const override;
   void SetMinimum(double minimum);

   double GetMaximum(void) const override;
   void SetMaximum(double maximum);

   double GetInitialValue(void) const override;
   void SetInitialValue(double initialValue);

private:
   double minimum {};
   double maximum {};
   double initialValue {};
};