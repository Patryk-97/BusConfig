#pragma once

#include "ICanIntAttributeValue.h"
#include "ICanHexAttributeValue.h"
#include "ICanFloatAttributeValue.h"
#include "ICanStringAttributeValue.h"
#include "ICanEnumAttributeValue.h"
#include "ICanAttribute.h"

template <ICanAttribute::IValueType_e>
struct ICanAttributeValueType
{
   using type = ICanAttributeValue*;
};

template <>
struct ICanAttributeValueType<ICanAttribute::IValueType_e::INT>
{
   using type = ICanIntAttributeValue*;
};

template <>
struct ICanAttributeValueType<ICanAttribute::IValueType_e::HEX>
{
   using type = ICanHexAttributeValue*;
};

template <>
struct ICanAttributeValueType<ICanAttribute::IValueType_e::FLOAT>
{
   using type = ICanFloatAttributeValue*;;
};

template <>
struct ICanAttributeValueType<ICanAttribute::IValueType_e::STRING>
{
   using type = ICanStringAttributeValue*;
};

template <>
struct ICanAttributeValueType<ICanAttribute::IValueType_e::ENUM>
{
   using type = ICanEnumAttributeValue*;
};

template <ICanAttribute::IValueType_e ValueType>
using ICanAttributeValueType_t = typename ICanAttributeValueType<ValueType>::type;