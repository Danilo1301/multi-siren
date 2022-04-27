#include "Log.h"

std::fstream Log::file;

void Log::Open(std::string fileName) {
	char logPath[256];
	sprintf(logPath, "\\%s.log", fileName.c_str());
	file.open(PLUGIN_PATH(logPath), std::fstream::out | std::fstream::trunc);
}