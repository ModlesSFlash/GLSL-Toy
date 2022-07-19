workspace "GLSL-Toy"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GLSL-Toy"
	location "GLSL-Toy"
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
		"%{prj.name}/vendor/glew-2.1.0/include",
		"%{prj.name}/vendor/glfw-3.3.4.bin.WIN64/include"
	}

	libdirs
	{
		"%{prj.name}/vendor/glew-2.1.0/lib/Release/x64",
		"%{prj.name}/vendor/glfw-3.3.4.bin.WIN64/lib-static-ucrt"
	}

	links 
	{
		"glew32s", -- static lib so no need to copy the dll into the bin folder
		"glfw3dll",
		"OpenGL32",
		"GLu32"
	}

	postbuildcommands
	{
		"{COPYFILE} \"%{prj.location}vendor/glfw-3.3.4.bin.WIN64/lib-static-ucrt/glfw3.dll\"  \"%{cfg.buildtarget.directory}\""
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