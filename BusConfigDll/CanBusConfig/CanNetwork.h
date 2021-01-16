#pragma once

#include "ICanNetwork.h"
#include "CanAttributeOwner.h"
#include "CanNode.h"
#include "CanMessage.h"
#include "CanSignal.h"
#include "CanEnvVar.h"

class CanNetwork : public CanAttributeOwner, public ICanNetwork
{
public:
   CanNetwork() : CanAttributeOwner(ICanAttribute::IObjectType_e::NETWORK) {}
   ~CanNetwork();

   void Clear(void) override;

   const char* GetName(void) const override;
   void SetName(const char* name);

   size_t GetNodesCount(void) const override;
   ICanNode* GetNodeByIndex(size_t index) const override;
   ICanNode* GetNodeByName(const char* name) const override;
   std::vector<CanNode*> GetNodes(void);
   size_t GetNodeIndex(const char* name) const override;
   bool RemoveNodeByIndex(size_t index) override;
   bool RemoveNodeByName(const char* name) override;
   void AddNode(CanNode* node);
   CanNode* CreateAndAddNode(void);

   size_t GetMessagesCount(void) const override;
   ICanMessage* GetMessageById(uint32_t id) const override;
   ICanMessage* GetMessageByName(const char* name) const override;
   ICanMessage* GetMessageByIndex(size_t index) const override;
   std::vector<CanMessage*> GetMessages(void);
   ICanMessage* GetMessageFront(void) const override;
   ICanMessage* GetMessageBack(void) const override;
   bool RemoveMessageByIndex(size_t index) override;
   bool RemoveMessageByName(const char* name) override;
   bool RemoveMessageById(uint32_t id) override;
   void AddMessage(CanMessage* message);
   CanMessage* CreateAndAddMessage(void);

   size_t GetSignalsCount(void) const override;
   ICanSignal* GetSignalByIndex(size_t index) const override;
   ICanSignal* GetSignalByName(const char* name) const override;
   std::vector<CanSignal*> GetSignals(void);
   size_t GetSignalIndex(const char* name) const override;
   bool RemoveSignalByIndex(size_t index) override;
   bool RemoveSignalByName(const char* name) override;
   void SortSignalsByName(bool caseSensitive = false) override;
   void SortSignalsByMessageName(bool caseSensitive = false) override;
   void AddSignal(CanSignal* signal);
   CanSignal* CreateAndAddSignal(void);

   size_t GetEnvVarsCount(void) const override;
   ICanEnvVar* GetEnvVarByIndex(size_t index) const override;
   ICanEnvVar* GetEnvVarByName(const char* name) const override;
   std::vector<CanEnvVar*> GetEnvVars(void);
   bool RemoveEnvVarByIndex(size_t index) override;
   bool RemoveEnvVarByName(const char* name) override;
   void AddEnvVar(CanEnvVar* envVar);

   size_t GetAttributesCount(void) const override;
   ICanAttribute* GetAttributeByIndex(size_t index) const override;
   ICanAttribute* GetAttributeByName(const char* name) const override;

   size_t GetAttributesValuesCount(void) const override;
   ICanAttributeValue* GetAttributeValue(const char* attributeName) const override;

   const char* GetComment(void) const override;
   void SetComment(const char* comment);

   void SetMainAttributes(void);

private:
   std::string name;
   std::vector<CanMessage*> messages;
   std::vector<CanNode*> nodes;
   std::vector<CanSignal*> signals;
   std::vector<CanEnvVar*> envVars;
   std::string comment;
};