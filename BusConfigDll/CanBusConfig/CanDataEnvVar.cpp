#include "CanDataEnvVar.h"

CanDataEnvVar::~CanDataEnvVar()
{
   this->Clear();
}

void CanDataEnvVar::Clear(void)
{
   CanEnvVar::Clear();
   this->length = 0;
}

const char* CanDataEnvVar::GetName(void) const
{
   return CanEnvVar::GetName();
}

ICanEnvVar::Type_e CanDataEnvVar::GetType(void) const
{
   return CanEnvVar::GetType();
}
const char* CanDataEnvVar::GetUnit(void) const
{
   return CanEnvVar::GetUnit();
}

uint32_t CanDataEnvVar::GetId(void) const
{
   return CanEnvVar::GetId();
}

ICanEnvVar::AccessType_e CanDataEnvVar::GetAccessType(void) const
{
   return CanEnvVar::GetAccessType();
}

ICanNode* CanDataEnvVar::GetAccessNode(void) const
{
   return CanEnvVar::GetAccessNode();
}

uint32_t CanDataEnvVar::GetLength(void) const
{
   return this->length;
}

void CanDataEnvVar::SetLength(uint32_t length)
{
   this->length = length;
}