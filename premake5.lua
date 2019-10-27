newoption {
	trigger     = "coverage",
	description = "Compile with --coverage"
}

workspace "Empire Serialization 2"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}



project "C++"
	location "C++"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on" 
	intrinsics "on"
	systemversion "latest"

	vectorextensions "AVX"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	configuration "coverage"
		buildoptions { "--coverage" }

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src/",
	}

	links 
	{ 

	}

	defines
	{

	}
	
	filter "action:vs*"
		defines
		{
			"EMPIRE_COMPILER_MSVC"
		}
		
	filter { "action:gmake*", "toolset:gcc" }
		defines
		{
			"EMPIRE_COMPILER_GCC"
		}
		print "Using GCC"
		buildoptions { "-fPIC", "-masm=intel" }

	filter "system:windows"
		defines
		{
			"EMPIRE_PLATFORM_WINDOWS",
		}


	filter "system:linux"
		defines
		{
			"EMPIRE_PLATFORM_UNIX"
		}

	filter "system:macosx"
		defines
		{
			"EMPIRE_PLATFORM_OSX",
			"EMPIRE_PLATFORM_UNIX"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		floatingpoint "Strict"


	filter "configurations:Release"
		runtime "Release"
		optimize "speed"
		inlining "auto"
		floatingpoint "Fast"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	vectorextensions "AVX"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	configuration "coverage"
		links { "gcov" }

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"./C++/src/",
		"%{prj.name}/src/",
	}

	links 
	{
		"C++",
	}

	defines
	{
	}

	filter "action:vs*"
		defines
		{
			"EMPIRE_COMPILER_MSVC"
		}
		
	filter { "action:gmake*", "toolset:gcc" }
		defines
		{
			"EMPIRE_COMPILER_GCC"
		}
		buildoptions { "-fPIC", "-masm=intel" }
	
	filter "system:windows"
		defines
		{
			"EMPIRE_PLATFORM_WINDOWS"
		}


	filter "system:linux"
		defines
		{
			"EMPIRE_PLATFORM_UNIX"
		}

	filter "system:macosx"

		defines
		{
			"EMPIRE_PLATFORM_OSX",
			"EMPIRE_PLATFORM_UNIX"
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		floatingpoint "Strict"

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"
		inlining "auto"
		floatingpoint "Fast"


project "Test"
	location "Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	vectorextensions "AVX"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	configuration "coverage"
		links { "gcov" }
		print "Code coverage is enabled for Test"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"./C++/src/",
		"%{prj.name}/src/",
		"%{prj.name}/vendor/",
	}

	links 
	{
		"C++",
	}

	defines
	{
	}

	filter "action:vs*"
		defines
		{
			"EMPIRE_COMPILER_MSVC"
		}
		
	filter { "action:gmake*", "toolset:gcc" }
		defines
		{
			"EMPIRE_COMPILER_GCC"
		}
		buildoptions { "-fPIC" }
	
	filter "system:windows"
		defines
		{
			"EMPIRE_PLATFORM_WINDOWS"
		}


	filter "system:linux"
		defines
		{
			"EMPIRE_PLATFORM_UNIX"
		}

	filter "system:macosx"

		defines
		{
			"EMPIRE_PLATFORM_OSX",
			"EMPIRE_PLATFORM_UNIX"
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		floatingpoint "Strict"

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"
		inlining "auto"
		floatingpoint "Fast"