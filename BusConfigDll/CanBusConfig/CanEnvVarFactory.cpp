#include "CanEnvVarFactory.h"
#include "CanIntEnvVar.h"
#include "CanFloatEnvVar.h"
#include "CanStringEnvVar.h"
#include "CanDataEnvVar.h"

CanEnvVar* CanEnvVarFactory::CreateEnvVar(ICanEnvVar::Type_e type)
{
   switch (type)
   {
      case ICanEnvVar::Type_e::INTEGER:
      {
         return new CanIntEnvVar {};
      }
      case ICanEnvVar::Type_e::FLOAT:
      {
         return new CanFloatEnvVar {};
      }
      case ICanEnvVar::Type_e::STRING:
      {
         return new CanStringEnvVar {};
      }
      case ICanEnvVar::Type_e::DATA:
      {
         return new CanDataEnvVar {};
      }
   }
   return nullptr;
}

CanEnvVar* CanEnvVarFactory::CreateEnvVar(uint32_t type)
{
   switch (type)
   {
      case static_cast<uint32_t>(ICanEnvVar::Type_e::INTEGER):
      {
         return new CanIntEnvVar {};
      }
      case static_cast<uint32_t>(ICanEnvVar::Type_e::FLOAT) :
      {
         return new CanFloatEnvVar {};
      }
      case static_cast<uint32_t>(ICanEnvVar::Type_e::STRING) :
      {
         return new CanStringEnvVar {};
      }
      case static_cast<uint32_t>(ICanEnvVar::Type_e::DATA) :
      {
         return new CanDataEnvVar {};
      }
   }
   return nullptr;
}