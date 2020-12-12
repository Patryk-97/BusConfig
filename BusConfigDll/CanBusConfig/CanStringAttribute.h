#pragma once

#include "ICanStringAttribute.h"
#include "CanAttribute.h"

class CanStringAttribute : public CanAttribute, ICanStringAttribute
{
public:
   CanStringAttribute() = default;
   ~CanStringAttribute();

   void Clear(void) override;

   const char* GetDefaultValue(void) const override;
   void SetDefaultValue(const char* defaultValue);

private:
   std::string defaultValue{};
};