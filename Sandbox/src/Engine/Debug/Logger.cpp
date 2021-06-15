#include "xpch.h"
#include "Logger.h"


#if defined(DEBUG)
Logger* Logger::s_loggerInstance = nullptr;
#endif