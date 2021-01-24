#pragma once

#include "ICanAttributeOwner.h"
#include "ICanNode.h"
#include "ICanMessage.h"
#include "ICanSignal.h"
#include "ICanEnvVar.h"
#include "ICanSignalBuilder.h"

class ICanNetwork : public ICanAttributeOwner
{
protected:
   ICanNetwork() {};
   virtual ~ICanNetwork() {};
public:
   virtual void Clear(void) = 0;
   virtual const char* GetName(void) const = 0;
   virtual void ModifyName(const char* name) = 0;

   virtual const char* GetProtocol(void) const = 0;
   virtual void ModifyProtocol(const char* protocol) = 0;

   virtual size_t GetNodesCount(void) const = 0;
   virtual ICanNode* GetNodeByIndex(size_t index) const = 0;
   virtual ICanNode* GetNodeByName(const char* name) const = 0;
   virtual size_t GetNodeIndex(const char* name) const = 0;
   virtual bool RemoveNodeByIndex(size_t index) = 0;
   virtual bool RemoveNodeByName(const char* name) = 0;

   virtual size_t GetMessagesCount(void) const = 0;
   virtual ICanMessage* GetMessageById(uint32_t id) const = 0;
   virtual ICanMessage* GetMessageByName(const char* name) const = 0;
   virtual ICanMessage* GetMessageByIndex(size_t index) const = 0;
   virtual ICanMessage* GetMessageFront(void) const = 0;
   virtual ICanMessage* GetMessageBack(void) const = 0;
   virtual bool RemoveMessageByIndex(size_t index) = 0;
   virtual bool RemoveMessageByName(const char* name) = 0;
   virtual bool RemoveMessageById(uint32_t id) = 0;

   virtual size_t GetSignalsCount(void) const = 0;
   virtual ICanSignal* GetSignalByIndex(size_t index) const = 0;
   virtual ICanSignal* GetSignalByName(const char* name) const = 0;
   virtual bool SignalExists(const char* name) const = 0;
   virtual size_t GetSignalIndex(const char* name) const = 0;
   virtual bool RemoveSignalByIndex(size_t index) = 0;
   virtual bool RemoveSignalByName(const char* name) = 0;
   virtual void SortSignalsByName(bool caseSensitive = false) = 0;
   virtual void SortSignalsByMessageName(bool caseSensitive = false) = 0;
   virtual ICanSignalBuilder* SignalBuilder(void) const = 0;

   virtual size_t GetEnvVarsCount(void) const = 0;
   virtual ICanEnvVar* GetEnvVarByIndex(size_t index) const = 0;
   virtual ICanEnvVar* GetEnvVarByName(const char* name) const = 0;
   virtual bool RemoveEnvVarByIndex(size_t index) = 0;
   virtual bool RemoveEnvVarByName(const char* name) = 0;

   virtual const char* GetComment(void) const = 0;
   virtual void ModifyComment(const char* comment) = 0;

   static constexpr size_t INVALID_INDEX = (size_t)(-1);

   static constexpr const char * NETWORK_NAME = "DBName";
   static constexpr const char * PROTOCOL = "BusType";
};