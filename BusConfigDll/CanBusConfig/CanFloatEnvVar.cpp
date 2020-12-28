#include "CanFloatEnvVar.h"

CanFloatEnvVar::~CanFloatEnvVar()
{
   this->Clear();
}

void CanFloatEnvVar::Clear(void)
{
   CanEnvVar::Clear();
   this->minimum = 0.0;
   this->maximum = 0.0;
   this->initialValue = 0.0;
}

const char* CanFloatEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

ICanEnvVar::Type_e CanFloatEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}
const char* CanFloatEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

uint32_t CanFloatEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

ICanEnvVar::AccessType_e CanFloatEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

ICanNode* CanFloatEnvVar::GetAccessNode(void) const
{
   return CanEnvVar::GetAccessNode();
}

double CanFloatEnvVar::GetMinimum(void) const
{
   return this->minimum;
}

void CanFloatEnvVar::SetMinimum(double minimum)
{
   this->minimum = minimum;
}

double CanFloatEnvVar::GetMaximum(void) const
{
   return this->maximum;
}

void CanFloatEnvVar::SetMaximum(double maximum)
{
   this->maximum = maximum;
}

double CanFloatEnvVar::GetInitialValue(void) const
{
   return this->initialValue;
}

void CanFloatEnvVar::SetInitialValue(double initialValue)
{
   this->initialValue = initialValue;
}