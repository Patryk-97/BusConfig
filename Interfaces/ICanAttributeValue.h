#pragma once

class ICanAttributeValue
{
protected:
   ICanAttributeValue() {};
   virtual ~ICanAttributeValue() {};
public:
   virtual void Clear(void) = 0;
};