#pragma once

#include "ICanHexAttribute.h"
#include "CanAttribute.h"

class CanHexAttribute : public CanAttribute, public ICanHexAttribute
{
public:
   CanHexAttribute() : CanAttribute { IValueType_e::HEX } {}
   ~CanHexAttribute();

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
   int32_t minimum{};
   int32_t maximum{};
   int32_t defaultValue{};
};