newoption 
{
   trigger = "opengl43",
   description = "use OpenGL 4.3"
}

workspace "RLGameGui"
	configurations { "Debug","Debug.DLL", "Release", "Release.DLL" }
	platforms { "x64", "x86"}

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
		
	filter { "platforms:x86" }
		architecture "x86"

	targetdir "bin/%{cfg.buildcfg}/"

project "raylib"
		filter "configurations:Debug.DLL OR Release.DLL"
			kind "SharedLib"
			defines {"BUILD_LIBTYPE_SHARED"}
			
		filter "configurations:Debug OR Release"
			kind "StaticLib"
			
		filter "action:vs*"
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
			characterset ("MBCS")
		
		filter "system:windows"
			defines{"_WIN32"}
			links {"winmm", "kernel32", "opengl32", "kernel32", "gdi32"}
			
		filter "system:linux"
			links {"pthread", "GL", "m", "dl", "rt", "X11"}
			
		filter{}
		
		defines{"PLATFORM_DESKTOP"}
		if (_OPTIONS["opengl43"]) then
			defines{"GRAPHICS_API_OPENGL_43"}
		else
			defines{"GRAPHICS_API_OPENGL_33"}
		end
	
		location "build"
		language "C"
		targetdir "bin/%{cfg.buildcfg}"
		
		includedirs { "raylib/src", "raylib/src/external/glfw/include"}
		vpaths 
		{
			["Header Files"] = { "raylib/src/**.h"},
			["Source Files/*"] = {"raylib/src/**.c"},
		}
		files {"raylib/src/*.h", "raylib/src/*.c"}
		
project "RLGameGui"
	kind "StaticLib"
		
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		links {"winmm"}
				
	filter{}
	
	location "./RLGameGui/"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++17"
	
	includedirs { "raylib/src","RLGameGui","RLGameGui/include"}
	vpaths 
	{
		["Header Files"] = { "RLGameGui/**.h"},
		["Source Files"] = {"RLGameGui/**.c"},
	}
	files {"RLGameGui/**.h", "RLGameGui/**.cpp"}
	
		
project "test"
	kind "ConsoleApp"
	location "Game"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++17"
	
	vpaths 
	{
		["Header Files"] = { "**.h"},
		["Source Files"] = {"**.c", "**.cpp"},
	}
	files {"test/**.c", "test/**.cpp", "test/**.h"}

	links {"raylib", "RLGameGui"}
	
	includedirs { "test", "raylib/src" , "RLGameGui/include"}
	
	defines{"PLATFORM_DESKTOP"}
	if (_OPTIONS["opengl43"]) then
		defines{"GRAPHICS_API_OPENGL_43"}
	else
		defines{"GRAPHICS_API_OPENGL_33"}
	end
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib"}
		links {"raylib.lib"}
        characterset ("MBCS")
		
	filter "system:windows"
		defines{"_WIN32"}
		links {"winmm", "kernel32", "opengl32", "kernel32", "gdi32"}
		libdirs {"bin/%{cfg.buildcfg}"}
		
	filter "system:linux"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}