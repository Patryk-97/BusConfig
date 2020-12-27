#pragma once

#include "ICanIntAttribute.h"
#include "ICanHexAttribute.h"
#include "ICanFloatAttribute.h"
#include "ICanStringAttribute.h"
#include "ICanEnumAttribute.h"

template <ICanAttribute::IValueType_e>
struct ICanAttributeType
{
   using type = ICanAttribute*;
};

template <>
struct ICanAttributeType<ICanAttribute::IValueType_e::INT>
{
   using type = ICanIntAttribute*;
};

template <>
struct ICanAttributeType<ICanAttribute::IValueType_e::HEX>
{
   using type = ICanHexAttribute*;
};

template <>
struct ICanAttributeType<ICanAttribute::IValueType_e::FLOAT>
{
   using type = ICanFloatAttribute*;
};

template <>
struct ICanAttributeType<ICanAttribute::IValueType_e::STRING>
{
   using type = ICanStringAttribute*;
};

template <>
struct ICanAttributeType<ICanAttribute::IValueType_e::ENUM>
{
   using type = ICanEnumAttribute*;
};

template <ICanAttribute::IValueType_e ValueType>
using ICanAttributeType_t = typename ICanAttributeType<ValueType>::type;