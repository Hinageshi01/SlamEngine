print("Generating editor...")

project("Editor")
	kind("ConsoleApp")
	language("C++")
	cppdialect("C++20")
	dependson { "Slam" }
	
	-- Project, binary and intermediate files path.
	location(IntermediatePath)
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	-- Set definitions.
	defines
	{
		"SPDLOG_NO_EXCEPTIONS", "SPDLOG_FMT_EXTERNAL", "FMT_HEADER_ONLY", "SPDLOG_COMPILED_LIB",
	}
	
	filter { "configurations:Debug" }
		defines { "SL_DEBUG" }
	filter { "configurations:Release" }
		defines { "SL_RELEASE" }
	filter { "configurations:Final" }
		defines { "SL_FINAL" }
	filter { "system:Windows" }
		defines { "SL_WINDOWS" }
	filter {}
	
	-- Set include paths.
	includedirs
	{
		EnginePath,
		EditorPath,
		ThirdPartyPath,
		path.join(ThirdPartyPath, "fmt/include"),
		path.join(ThirdPartyPath, "spdlog/include"),
	}
	
	-- Set files.
	files
	{
		path.join(EditorPath, "**.*"),
		path.join(ThirdPartyPath, "spdlog/fmt/**.*"),
		path.join(ThirdPartyPath, "spdlog/include/**.*"),
	}
	
	removefiles
	{
		path.join(ThirdPartyPath, "spdlog/include/spdlog/fmt/bundled/**.*"),
	}
	
	-- Link to engine.
	libdirs
	{
		path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
	}
	links
	{
		"Slam",
	}
	
	-- Link to thirdparty libs.
	filter { "configurations:Debug" }
		libdirs
		{
			path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
			path.join(ThirdPartyPath, "spdlog/build/Debug"),
		}
		links
		{
			"Slam", "spdlogd"
		}
	filter { "configurations:Release" }
		libdirs
		{
			path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
			path.join(ThirdPartyPath, "spdlog/build/Release"),
		}
		links
		{
			"Slam", "spdlog"
		}
	filter { "configurations:Final" }
		libdirs
		{
			path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Slam"),
			path.join(ThirdPartyPath, "spdlog/build/Release"),
		}
		links
		{
			"Slam", "spdlog"
		}
	filter {}
	
	-- Use /MT and /MTd.
	staticruntime "on"
	filter "configurations:Debug"
		runtime("Debug") -- /MTd
	filter "configurations:Release"
		runtime("Release") -- /MT
	filter "configurations:Final"
		runtime("Release") -- /MT
	filter {}
	
	rtti("Off")
	exceptionhandling("Off")
	
	flags
	{
		-- Treat all warnings as errors.
		"FatalWarnings",
		-- Enable Visual Studio to use multiple compiler processes when building.
		"MultiProcessorCompile",
	}
	