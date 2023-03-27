#pragma once
#include "directory.h"
#include "debugger.h"

class System
{
public:
	Debugger debugger;
	Directory directory;
	std::string m_componentName;

	System(std::string className);
	System(std::string componentName, std::string className);
};