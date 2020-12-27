#pragma once

#include "ICanIntAttributeValue.h"
#include "ICanHexAttributeValue.h"
#include "ICanFloatAttributeValue.h"
#include "ICanStringAttributeValue.h"
#include "ICanEnumAttributeValue.h"
#include "ICanAttribute.h"

#include <string>

template <ICanAttribute::IValueType_e>
struct ICanAttributeCppValueType;

template <>
struct ICanAttributeCppValueType<ICanAttribute::IValueType_e::INT>
{
   using type = int32_t;
};

template <>
struct ICanAttributeCppValueType<ICanAttribute::IValueType_e::HEX>
{
   using type = int32_t;
};

template <>
struct ICanAttributeCppValueType<ICanAttribute::IValueType_e::FLOAT>
{
   using type = double;
};

template <>
struct ICanAttributeCppValueType<ICanAttribute::IValueType_e::STRING>
{
   using type = std::string;
};

template <>
struct ICanAttributeCppValueType<ICanAttribute::IValueType_e::ENUM>
{
   using type = std::string;
};

template <ICanAttribute::IValueType_e ValueType>
using ICanAttributeCppValueType_t = typename ICanAttributeCppValueType<ValueType>::type;