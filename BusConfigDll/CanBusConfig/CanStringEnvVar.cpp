#include "CanStringEnvVar.h"

CanStringEnvVar::~CanStringEnvVar()
{
   this->Clear();
}

void CanStringEnvVar::Clear(void)
{
   CanEnvVar::Clear();
}

const char* CanStringEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

ICanEnvVar::Type_e CanStringEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}
const char* CanStringEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

uint32_t CanStringEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

ICanEnvVar::AccessType_e CanStringEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

ICanNode* CanStringEnvVar::GetAccessNode(void) const
{
   return CanEnvVar::GetAccessNode();
}