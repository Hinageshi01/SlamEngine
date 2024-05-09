print("Generating engine...")

project("Slam")
	kind("StaticLib")
	language("C++")
	cppdialect("C++20")
	
	-- Project, binary and intermediate files path.
	location(IntermediatePath)
	targetdir(path.join(BinaryPath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	objdir(path.join(IntermediatePath, "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"))
	
	-- Set definitions.
	defines
	{
		"SL_ROOT_PATH=\""..RootPath.."\"",
		"SL_ASSET_PATH=\""..path.join(RootPath, "Engine/Asset").."\"",
		"SPDLOG_NO_EXCEPTIONS", "SPDLOG_USE_STD_FORMAT",
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
		ThirdPartyPath,
		path.join(ThirdPartyPath, "spdlog/include"),
		path.join(ThirdPartyPath, "glfw/include"),
		path.join(ThirdPartyPath, "glad/include"),
		path.join(ThirdPartyPath, "imgui"),
		path.join(ThirdPartyPath, "glm"),
		path.join(ThirdPartyPath, "entt/src"),
	}
	
	-- Set files.
	files
	{
		path.join(EnginePath, "**.*"),
	}
	
	-- Link to thirdparty libs.
	filter { "configurations:Debug" }
		libdirs
		{
			path.join(ThirdPartyPath, "glfw/build/src/Debug"),
			path.join(ThirdPartyPath, "build/glad/bin/Debug"),
			path.join(ThirdPartyPath, "build/imgui/bin/Debug"),
			path.join(ThirdPartyPath, "build/implot/bin/Debug"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Debug"),
		}
		links
		{
			"glfw3", "glad", "imgui", "implot", "imguizmo",
		}
	filter { "configurations:Release" }
		libdirs
		{
			path.join(ThirdPartyPath, "glfw/build/src/Release"),
			path.join(ThirdPartyPath, "build/glad/bin/Release"),
			path.join(ThirdPartyPath, "build/imgui/bin/Release"),
			path.join(ThirdPartyPath, "build/implot/bin/Release"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Release"),
		}
		links
		{
			"glfw3", "glad", "imgui", "implot", "imguizmo",
		}
	filter { "configurations:Final" }
		libdirs
		{
			path.join(ThirdPartyPath, "glfw/build/src/Release"),
			path.join(ThirdPartyPath, "build/glad/bin/Release"),
			path.join(ThirdPartyPath, "build/imgui/bin/Release"),
			path.join(ThirdPartyPath, "build/implot/bin/Release"),
			path.join(ThirdPartyPath, "build/imguizmo/bin/Release"),
		}
		links
		{
			"glfw3", "glad", "imgui", "implot", "imguizmo",
		}
	filter {}
	
	-- Use /MT and /MTd.
	staticruntime "on"
	filter { "configurations:Debug" }
		runtime("Debug") -- /MTd
	filter { "configurations:Release" }
		runtime("Release") -- /MT
	filter { "configurations:Final" }
		runtime("Release") -- /MT
	filter {}
	
	rtti("Off")
	exceptionhandling("Off")
	justmycode("Off")
	editAndContinue("Off")
	
	flags
	{
		-- Treat all warnings as errors.
		"FatalWarnings",
		-- Enable Visual Studio to use multiple compiler processes when building.
		"MultiProcessorCompile",
	}
	