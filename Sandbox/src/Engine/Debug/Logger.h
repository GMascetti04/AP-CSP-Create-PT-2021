#pragma once
#include "xpch.h"
#include "MemoryTracker.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#if defined(DEBUG)

class Logger {
public:
	static Logger* getLogger() { return s_loggerInstance; }

	spdlog::logger* get_client_logger() { return m_client_logger.get(); }
	spdlog::logger* get_engine_logger() { return m_engine_logger.get(); }

	static void init(const char* engine_name, const char* client_name) {
		spdlog::set_pattern("%^[%T][%n][%l]: %v %$");
		if (!s_loggerInstance) {
			s_loggerInstance = new Logger(engine_name, client_name);
		}
	}

	static void destroy() {
		delete s_loggerInstance;
		s_loggerInstance = nullptr;
	}

private:
	static Logger* s_loggerInstance;

	std::shared_ptr<spdlog::logger> m_client_logger;
	std::shared_ptr<spdlog::logger> m_engine_logger;

	std::string m_client_name;
	std::string m_engine_name;

	Logger(const char* engine_name, const char* client_name) {
		m_client_logger = spdlog::stdout_color_mt(client_name);
		m_engine_logger = spdlog::stderr_color_mt(engine_name);
		m_client_name = client_name;
		m_engine_name = engine_name;
		m_client_logger.get()->set_level(spdlog::level::trace);
		m_engine_logger.get()->set_level(spdlog::level::trace);
	}

	~Logger() { 
		m_client_logger.get()->flush(); 
		m_engine_logger.get()->flush(); 
		spdlog::drop(m_client_name); 
		spdlog::drop(m_engine_name); 
		spdlog::drop_all(); }
};

#define LOG_INIT(e,c) Logger::init( e , c )
#define LOG_DESTROY() Logger::destroy()

#define Log_EngineInfo(...) Logger::getLogger()->get_engine_logger()->info( __VA_ARGS__)
#define Log_EngineWarn(...) Logger::getLogger()->get_engine_logger()->warn(__VA_ARGS__)
#define Log_EngineError(...) Logger::getLogger()->get_engine_logger()->error(__VA_ARGS__)
#define log_EngineCritical(...) Logger::getLogger()->get_engine_logger()->critical(__VA_ARGS__)
#define Log_EngineDebug(...) Logger::getLogger()->get_engine_logger()->debug(__VA_ARGS__)

#define Log_ClientInfo(...) Logger::getLogger()->get_client_logger()->info( __VA_ARGS__)
#define Log_ClientWarn(...) Logger::getLogger()->get_client_logger()->warn(__VA_ARGS__)
#define Log_ClientError(...) Logger::getLogger()->get_client_logger()->error(__VA_ARGS__)
#define Log_ClientCritical(...) Logger::getLogger()->get_client_logger()->critical(__VA_ARGS__)
#define Log_ClientDebug(...) Logger::getLogger()->get_client_logger()->debug(__VA_ARGS__)

#else

#define LOG_INIT(e,c)
#define LOG_DESTROY()

#define Log_EngineInfo(...)
#define Log_EngineWarn(...)
#define Log_EngineError(...)
#define log_EngineCritical(...)
#define Log_EngineDebug(...)

#define Log_ClientInfo(...)
#define Log_ClientWarn(...)
#define Log_ClientError(...)
#define Log_ClientCritical(...)
#define Log_ClientDebug(...)
#endif