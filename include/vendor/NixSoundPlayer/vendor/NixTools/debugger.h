#pragma once
#include <iostream>
class Debugger
{
private:
	std::string m_className;
	std::string m_componentName;
	std::string m_previousMessage;
public:
	enum DebugLevel
	{
		Info,
		Warning,
		Error,
		Unknown
	};
	Debugger(std::string componentName, std::string className);
	Debugger(std::string className);
	
	void giveMessage(DebugLevel level, std::string message);
	void giveMessage(DebugLevel level, std::string funcName, std::string message);
};