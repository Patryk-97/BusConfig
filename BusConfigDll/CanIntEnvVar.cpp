#include "CanIntEnvVar.h"

CanIntEnvVar::~CanIntEnvVar()
{
   this->Clear();
}

void CanIntEnvVar::Clear(void)
{
   CanEnvVar::Clear();
   this->minimum = 0;
   this->maximum = 0;
   this->initialValue = 0;
}

const char* CanIntEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

ICanEnvVar::Type_e CanIntEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}
const char* CanIntEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

uint32_t CanIntEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

ICanEnvVar::AccessType_e CanIntEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

ICanNode* CanIntEnvVar::GetAccessNode(void) const
{
   return CanEnvVar::GetAccessNode();
}

int32_t CanIntEnvVar::GetMinimum(void) const
{
   return this->minimum;
}

void CanIntEnvVar::SetMinimum(int32_t minimum)
{
   this->minimum = minimum;
}

int32_t CanIntEnvVar::GetMaximum(void) const
{
   return this->maximum;
}

void CanIntEnvVar::SetMaximum(int32_t maximum)
{
   this->maximum = maximum;
}

int32_t CanIntEnvVar::GetInitialValue(void) const
{
   return this->initialValue;
}

void CanIntEnvVar::SetInitialValue(int32_t initialValue)
{
   this->initialValue = initialValue;
}