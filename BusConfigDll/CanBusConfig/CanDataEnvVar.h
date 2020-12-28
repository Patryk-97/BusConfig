#pragma once

#include "ICanDataEnvVar.h"
#include "CanEnvVar.h"

class CanDataEnvVar : public CanEnvVar, public ICanDataEnvVar
{
public:
   CanDataEnvVar() = default;
   ~CanDataEnvVar();

   void Clear(void) override;
   const char* GetName(void) const override;
   Type_e GetType(void) const override;
   const char* GetUnit(void) const override;
   uint32_t GetId(void) const override;   // obsolete
   AccessType_e GetAccessType(void) const override;
   ICanNode* GetAccessNode(void) const override;

   uint32_t GetLength(void) const override;
   void SetLength(uint32_t length);

private:
   uint32_t length {};
};