#pragma once

#include "ICanIntAttribute.h"
#include "CanAttribute.h"

class CanIntAttribute : public CanAttribute, public ICanIntAttribute
{
public:
   CanIntAttribute() = default;
   ~CanIntAttribute();

   void Clear(void) override;

   const char* GetName(void) const override;
   IValueType_e GetValueType(void) const override;
   IObjectType_e GetObjectType(void) const override;

   int32_t GetMinimum(void) const override;
   void SetMinimum(int32_t minimum);

   int32_t GetMaximum(void) const override;
   void SetMaximum(int32_t maximum);
   
   int32_t GetDefaultValue(void) const override;
   void SetDefaultValue(int32_t defaultValue);

private:
   int32_t minimum {};
   int32_t maximum {};
   int32_t defaultValue {};
};