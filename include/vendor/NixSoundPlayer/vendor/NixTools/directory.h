#pragma once
#include <iostream>

#ifdef __linux__
#include "unistd.h"
#endif

#ifdef _WIN64
#include <direct.h>
#define GetCurrentDir _getcwd
#endif

class Directory
{
private:
	std::string m_path;
	std::string m_cwd;
	void processCwd();
public:
	Directory(const std::string& path);
	Directory();
	
	void changePath(std::string path);
	std::string getPath();
	std::string getWorkDirectory();
	std::string getFileName();
};