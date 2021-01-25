#include "CanNodeBuilder.h"
#include "CanNetwork.h"

CanNodeBuilder::~CanNodeBuilder()
{
   this->Clear();
}

void CanNodeBuilder::Clear(void)
{
   this->network = nullptr;
   this->name.clear();
   this->address = 0;
   this->comment.clear();
}

ICanNodeBuilder* CanNodeBuilder::WithName(const char* name)
{
   this->name = name;
   return this;
}

ICanNodeBuilder* CanNodeBuilder::WithAddress(int32_t address)
{
   this->address = address;
   return this;
}

ICanNodeBuilder* CanNodeBuilder::WithComment(const char* comment)
{
   this->comment = comment;
   return this;
}

ICanNode* CanNodeBuilder::Build(void)
{
   auto ifInvalidData = [this] { this->Clear(); return nullptr; };
   if (this->name.empty()) return ifInvalidData();
   if (this->network == nullptr) return ifInvalidData();

   auto node = new CanNode {};
   node->SetName(this->name.c_str());
   node->SetAddress(this->address);
   node->SetComment(this->comment.c_str());

   node->SetNetwork(this->network);
   this->network->AddNode(node);

   return node;
}