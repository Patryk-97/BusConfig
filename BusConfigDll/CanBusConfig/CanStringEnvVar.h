#pragma once

#include "ICanStringEnvVar.h"
#include "CanEnvVar.h"

class CanStringEnvVar : public CanEnvVar, public ICanStringEnvVar
{
public:
   CanStringEnvVar() = default;
   ~CanStringEnvVar();

   void Clear(void) override;
   const char* GetName(void) const override;
   Type_e GetType(void) const override;
   const char* GetUnit(void) const override;
   uint32_t GetId(void) const override;   // obsolete
   AccessType_e GetAccessType(void) const override;
   ICanNode* GetAccessNode(void) const override;
};