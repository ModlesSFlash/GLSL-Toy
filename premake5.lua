workspace "bubble-cum"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "bubble-cum"
	location "bubble-cum"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/SDL2-2.0.14-VC/include",
		"%{prj.name}/vendor/SDL2_ttf-2.0.15/include"
	}

	libdirs
	{
		"%{prj.name}/vendor/SDL2-2.0.14-VC/lib/x64",
		"%{prj.name}/vendor/SDL2_ttf-2.0.15/lib/x64"
	}

	links 
	{
		"SDL2",
		"SDL2main",
		"SDL2_ttf"
	}
	
	postbuildcommands
	{
		"{COPYFILE} \"%{prj.location}vendor/SDL2-2.0.14-VC/lib/x64/SDL2.dll\"           \"%{cfg.buildtarget.directory}\"",
		"{COPYFILE} \"%{prj.location}vendor/SDL2_ttf-2.0.15/lib/x64/SDL2_ttf.dll\"      \"%{cfg.buildtarget.directory}\"",
		"{COPYFILE} \"%{prj.location}vendor/SDL2_ttf-2.0.15/lib/x64/zlib1.dll\"         \"%{cfg.buildtarget.directory}\"",
		"{COPYFILE} \"%{prj.location}vendor/SDL2_ttf-2.0.15/lib/x64/libfreetype-6.dll\" \"%{cfg.buildtarget.directory}\"",
		"{COPYFILE} \"%{prj.location}vendor/fonts/Novem.ttf\"                           \"%{cfg.buildtarget.directory}\""
	}

	filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MF_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MF_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines "MF_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "MF_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "MF_DIST"
		optimize "On"