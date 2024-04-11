#pragma once

#include "Event/Event.h"

#ifndef SL_FINAL
	#pragma warning(push)
	// For deprecated stdext::checked_array_iterator
	#pragma warning(disable:4996)
	// For rtti dynamic_cast
	#pragma warning(disable:4541)
		#include <spdlog/spdlog.h>
		#include <spdlog/fmt/ostr.h>
	#pragma warning(pop) 
#endif

namespace sl
{

class Log final
{
public:
	static void Init();

#ifndef SL_FINAL

	static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_pEngineLogger; }
	static std::shared_ptr<spdlog::logger> GetEditorLogger() { return s_pEditorLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_pEngineLogger;
	static std::shared_ptr<spdlog::logger> s_pEditorLogger;

#endif

};

} // namespace sl

#ifndef SL_FINAL

inline std::ostream &operator<<(std::ostream &os, const sl::Event &event)
{
	return os << event.ToString();
}
template <> struct fmt::formatter<sl::Event> : ostream_formatter {};

#endif

#ifndef SL_FINAL
	// Engine logger macros.
	#define SL_ENGINE_TRACE(...) ::sl::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define SL_ENGINE_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_ENGINE_INFO(...)  ::sl::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define SL_ENGINE_WARN(...)  ::sl::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define SL_ENGINE_ERROR(...) ::sl::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define SL_ENGINE_FATAL(...) ::sl::Log::GetEngineLogger()->critical(__VA_ARGS__)
	
	// Editor logger macros.
	#define SL_EDITOR_TRACE(...) ::sl::Log::GetEditorLogger()->trace(__VA_ARGS__)
	#define SL_EDITOR_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_EDITOR_INFO(...)  ::sl::Log::GetEditorLogger()->info(__VA_ARGS__)
	#define SL_EDITOR_WARN(...)  ::sl::Log::GetEditorLogger()->warn(__VA_ARGS__)
	#define SL_EDITOR_ERROR(...) ::sl::Log::GetEditorLogger()->error(__VA_ARGS__)
	#define SL_EDITOR_FATAL(...) ::sl::Log::GetEditorLogger()->critical(__VA_ARGS__)
#else
	#define SL_ENGINE_TRACE(...)
	#define SL_ENGINE_DEBUG(...)
	#define SL_ENGINE_INFO(...)
	#define SL_ENGINE_WARN(...)
	#define SL_ENGINE_ERROR(...)
	#define SL_ENGINE_FATAL(...)
	#define SL_EDITOR_TRACE(...)
	#define SL_EDITOR_DEBUG(...)
	#define SL_EDITOR_INFO(...)
	#define SL_EDITOR_WARN(...)
	#define SL_EDITOR_ERROR(...)
	#define SL_EDITOR_FATAL(...)
#endif
