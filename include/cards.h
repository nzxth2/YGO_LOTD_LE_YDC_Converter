#pragma once
#include <unordered_map>
#include "filehandling.h"

void LoadCards(std::unordered_map<Short,std::string> &ID_to_name, std::unordered_map<std::string,Short> &name_to_ID);