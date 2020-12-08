#pragma once

#include "ICanValueDescription.h"
#include <string>

class CanValueDescription : public ICanValueDescription
{
public:
   CanValueDescription() = default;
   ~CanValueDescription();

   void Clear(void) override;

   uint32_t GetValue(void) const override;
   void SetValue(uint32_t value);

   const char* GetDescription(void) const override;
   void SetDescription(const char * description);

private:
   uint32_t value {};
   std::string description;
};