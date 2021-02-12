workspace "RLGameGui"
	configurations { "Debug","Debug.DLL", "Release", "Release.DLL" }
	platforms { "x64"}
	defaultplatform "x64"
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Debug.DLL"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter "configurations:Release.DLL"
		defines { "NDEBUG" }
		optimize "On"	
		
	filter { "platforms:x64" }
		architecture "x86_64"
		
	targetdir "bin/%{cfg.buildcfg}/"
	
	defines{"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33"}
	
project "raylib"
	filter "configurations:Debug.DLL OR Release.DLL"
		kind "SharedLib"
		defines {"BUILD_LIBTYPE_SHARED"}
		
	filter "configurations:Debug OR Release"
		kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
	
	filter{}
	
	location "./"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++17"
	
	includedirs { "raylib/src", "raylib/src/external/glfw/include"}
	vpaths 
	{
		["Header Files"] = { "raylib/src/**.h"},
		["Source Files/*"] = {"raylib/src/**.c"},
	}
	files {"raylib/src/*.h", "raylib/src/*.c"}
	
project "RLGameGui"
	kind "StaticLib"
		
	filter "configurations:Debug OR Release"
		kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "./RLGameGui/"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++17"
	
	includedirs { "raylib/src","RLGameGui/","RLGameGui/include/"}
	vpaths 
	{
		["Header Files"] = { "RLGameGui/*.h","RLGameGui/include/**.h"},
		["Source Files"] = {"RLGameGui/**.cpp"},
	}
	files {"RLGameGui/**.h", "RLGameGui/**.cpp"}


project "test"
	kind "ConsoleApp"
	location "test"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++17"
	
	vpaths 
	{
		["Header Files"] = { "**.h"},
		["Source Files"] = {"**.c", "**.cpp"},
	}
	files {"test/**.cpp", "test}/**.h"}

	links {"raylib","RLGameGui"}
	
	includedirs { "%{wks.name}", "raylib/src", "RLGameGui/include" }
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib"}
		links {"winmm", "raylib.lib"}
		libdirs {"bin/%{cfg.buildcfg}"}
		
	filter "action:gmake*"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}
	