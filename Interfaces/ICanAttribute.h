#pragma once

class ICanAttribute
{
protected:
   ICanAttribute() {};
   virtual ~ICanAttribute() {};
public:
   
   enum class IValueType_e
   {
      UNDEFINED = 0,
      INTEGER = 1,
      HEX = 2,
      FLOAT = 3,
      STRING = 4,
      ENUM = 5
   };

   enum class IObjectType_e
   {
      UNDEFINED = 0,
      NETWORK = 1,
      NODE = 2,
      MESSAGE = 3,
      SIGNAL = 4
   };

   virtual void Clear(void) = 0;
   virtual const char * GetName(void) const = 0;
   virtual IValueType_e GetValueType(void) const = 0;
   virtual IObjectType_e GetObjectType(void) const = 0;
};

