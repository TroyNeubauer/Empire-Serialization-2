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

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
	}

	links 
	{ 

	}

	defines
	{

	}

	filter "system:windows"


		defines
		{
			"EMPIRE_PLATFORM_WINDOWS"
		}


	filter "system:linux"
	
		libdirs
		{
			"EMPIRE_PLATFORM_LINUX"
		}
	

		defines
		{

		}

	filter "system:macosx"

		defines
		{
			"EMPIRE_PLATFORM_OSX"
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

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"C++/src",
	}

	links 
	{
		"C++",
	}

	defines
	{
	}

		filter "system:windows"


		defines
		{
			"EMPIRE_PLATFORM_WINDOWS"
		}


	filter "system:linux"
	
		libdirs
		{
			"EMPIRE_PLATFORM_LINUX"
		}
	

		defines
		{

		}

	filter "system:macosx"

		defines
		{
			"EMPIRE_PLATFORM_OSX"
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
