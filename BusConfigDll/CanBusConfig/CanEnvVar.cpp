#include "CanEnvVar.h"
#include "helpers.h"
#include <algorithm>
#include "CanNetwork.h" // circular dependency

namespace ranges = std::ranges;

CanEnvVar::~CanEnvVar()
{
   this->Clear();
}

void CanEnvVar::Clear(void)
{
   CanAttributeOwner::Clear();
   this->name.clear();
   this->type = Type_e::UNDEFINED;
   this->unit.clear();
   this->id = 0;
   this->accessType = AccessType_e::UNDEFINED;
   this->accessNodes.clear();
   helpers::ClearPtr(this->valueTable);
   this->comment.clear();
}

const char* CanEnvVar::GetName(void) const
{
   return this->name.c_str();
}

void CanEnvVar::ModifyName(const char* name)
{
   this->name = name;
}

void CanEnvVar::SetName(const char* name)
{
   this->name = name;
}

ICanEnvVar::Type_e CanEnvVar::GetType(void) const
{
   return this->type;
}

void CanEnvVar::ModifyType(Type_e type)
{
   this->type = type;
}

void CanEnvVar::SetType(Type_e type)
{
   this->type = type;
}

const char* CanEnvVar::GetUnit(void) const
{
   return this->unit.c_str();
}

void CanEnvVar::ModifyUnit(const char* unit)
{
   this->unit = unit;
}

void CanEnvVar::SetUnit(const char* unit)
{
   this->unit = unit;
}

uint32_t CanEnvVar::GetId(void) const
{
   return this->id;
}

void CanEnvVar::ModifyId(uint32_t id)
{
   this->id = id;
}

void CanEnvVar::SetId(uint32_t id)
{
   this->id = id;
}

ICanEnvVar::AccessType_e CanEnvVar::GetAccessType(void) const
{
   return this->accessType;
}

void CanEnvVar::ModifyAccessType(AccessType_e accessType)
{
   this->accessType = accessType;
}

void CanEnvVar::SetAccessType(AccessType_e accessType)
{
   this->accessType = accessType;
}

void CanEnvVar::SetAccessTypeValue(uint32_t accessTypeValue)
{
   if (accessTypeValue & 0x8000)
   {
      this->type = Type_e::STRING;
   }
   accessTypeValue &= 0x000F;
   if (accessTypeValue < 4)
   {
      this->accessType = static_cast<AccessType_e>(accessTypeValue);
   }
}

size_t CanEnvVar::GetAccessNodesCount(void) const
{
   return this->accessNodes.size();
}

ICanNode* CanEnvVar::GetAccessNodeByIndex(size_t index) const
{
   return (index < this->accessNodes.size() ? this->accessNodes[index] : nullptr);
}

ICanNode* CanEnvVar::GetAccessNodeByName(const char* name) const
{
   auto it = ranges::find_if(this->accessNodes, [&name](CanNode* accessNode) { return !std::strcmp(accessNode->GetName(), name); });
   return (it != this->accessNodes.end() ? *it : nullptr);
}

std::vector<CanNode*> CanEnvVar::GetAccessNodes(void) const
{
   return this->accessNodes;
}

void CanEnvVar::AddAccessNode(CanNode* accessNode)
{
   if (accessNode != nullptr)
   {
      this->accessNodes.push_back(accessNode);
   }
}

ICanValueTable* CanEnvVar::GetValueTable(void) const
{
   return this->valueTable;
}

void CanEnvVar::SetValueTable(CanValueTable* valueTable)
{
   this->valueTable = valueTable;
}

size_t CanEnvVar::GetAttributesCount(void) const
{
   return CanAttributeOwner::GetAttributesCount();
}

ICanAttribute* CanEnvVar::GetAttributeByIndex(size_t index) const
{
   return CanAttributeOwner::GetAttributeByIndex(index);
}

ICanAttribute* CanEnvVar::GetAttributeByName(const char* name) const
{
   return CanAttributeOwner::GetAttributeByName(name);
}

size_t CanEnvVar::GetAttributesValuesCount(void) const
{
   return CanAttributeOwner::GetAttributesValuesCount();
}

ICanAttributeValue* CanEnvVar::GetAttributeValue(const char* attributeName) const
{
   return CanAttributeOwner::GetAttributeValue(attributeName);
}

const char* CanEnvVar::GetComment(void) const
{
   return this->comment.c_str();
}

void CanEnvVar::ModifyComment(const char* comment)
{
   this->comment = comment;
}

void CanEnvVar::SetComment(const char* comment)
{
   this->comment = comment;
}

ICanNetwork* CanEnvVar::GetNetwork(void) const
{
   return this->network;
}

void CanEnvVar::SetNetwork(CanNetwork* network)
{
   this->network = network;
}