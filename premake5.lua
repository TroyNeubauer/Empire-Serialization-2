newoption {
	trigger     = "coverage",
	description = "Compile with code coverage enabled"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}

workspace "Empire Serialization 2"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Release-Size",
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

	filter "configurations:Release-Size"
		runtime "Release"
		optimize "size"
		inlining "auto"
		floatingpoint "Fast"



project "C++"
	location "C++"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on" 
	intrinsics "on"
	systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src/",
		"%{prj.name}/include/",
		"%{prj.name}/include/EmpireSerialization/",
	}

	
	configuration "coverage"
		buildoptions { "--coverage", "-fprofile-abs-path" }

	filter { "action:gmake*", "toolset:gcc" }
		buildoptions { "-masm=intel" }



project "Test"
	location "Test"
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
		"%{prj.name}/src/",
	}

	sysincludedirs
	{
		"C++/include/",
		"C++/src/",--To allow for accessing internals in test cases
		"Test/vendor/",
	}

	links 
	{
		"C++",
	}

	configuration "coverage"
		buildoptions { "--coverage", "-fprofile-abs-path" }

	filter { "action:gmake*", "toolset:gcc" }
		buildoptions { "-fPIC" }


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
		"%{prj.name}/src/",
	}

	sysincludedirs
	{
		"C++/include/",
		"C++/src/",--To allow for accessing internals in test cases
	}

	links 
	{
		"C++",
	}

	configuration "coverage"
		buildoptions { "--coverage", "-fprofile-abs-path" }

	filter { "action:gmake*", "toolset:gcc" }
		buildoptions { "-fPIC" }

