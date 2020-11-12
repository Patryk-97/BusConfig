#include "CanBusConfig.h"
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>

CanBusConfig::~CanBusConfig()
{

}

bool CanBusConfig::Load(const char* filename)
{
   // locals
   bool rV { false };
   std::string line;
   std::ifstream file(filename);

   if (file.is_open())
   {
      rV = true;

      while (std::getline(file, line))
      {
         std::cout << line << "\n";
      }
   }

   return rV;
}