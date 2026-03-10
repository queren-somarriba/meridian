#pragma once

#include <iostream>
#include "flowfield.hpp"

bool		fileExist(const std::string& name);

bool		loadConfig(const std::string& filename, otData& data);

std::string	readFile(const char* fileName);