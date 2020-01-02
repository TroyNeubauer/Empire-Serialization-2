
local function add_new_gcc_toolset(name, prefix, suffix)
	local gcc                         = premake.tools.gcc
	local new_toolset                 = {}  
	new_toolset.getcflags             = gcc.getcflags
	new_toolset.getcxxflags           = gcc.getcxxflags
	new_toolset.getforceincludes      = gcc.getforceincludes
	new_toolset.getldflags            = gcc.getldflags
	new_toolset.getcppflags           = gcc.getcppflags
	new_toolset.getdefines            = gcc.getdefines
	new_toolset.getincludedirs        = gcc.getincludedirs
	new_toolset.getLibraryDirectories = gcc.getLibraryDirectories
	new_toolset.getlinks              = gcc.getlinks
	new_toolset.getundefines          = gcc.getundefines
	new_toolset.getmakesettings       = gcc.getmakesettings
	new_toolset.getrunpathdirs        = gcc.getrunpathdirs

	new_toolset.toolset_prefix        = prefix
	new_toolset.toolset_suffix		  = suffix

	function new_toolset.gettoolname (cfg, tool)  
		if tool == "cc" then
			name = new_toolset.toolset_prefix.."gcc"..new_toolset.toolset_suffix
		elseif tool == "cxx" then
			name = new_toolset.toolset_prefix.."g++"..new_toolset.toolset_suffix
		elseif tool == "ar" then
			name = new_toolset.toolset_prefix.."ar"
		end
		return name
	end  

	premake.tools[name] = new_toolset

	return name
end

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

	configuration "coverage"
		add_new_gcc_toolset("gcc-9", "/usr/bin/", "-9")--Force gcc9
		toolset "gcc-9"



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

	
	configurations "coverage"
		buildoptions { "--coverage", "-fprofile-abs-path" }

	filter { "toolset:gcc*" }
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
		"gcov",
	}

	configuration "coverage"
		buildoptions { "-fprofile-abs-path" }


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
		"gcov",
	}

	configuration "coverage"
		buildoptions { "-fprofile-abs-path" }

