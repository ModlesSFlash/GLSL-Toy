-- pretty much the script from The Cherno's video about premake for his game engine
-- https://youtu.be/sULV3aB2qeU

-- i might premake the make... uh? i might make the premake file work on linux/mac as well but i'm no expert and i have no clue about nuances of these systems
-- would appreciate some help regarding non-windows library names etc
-- you could just point at something and tell me "THAT won't work on linux, but THIS should" etc

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
		"glew32",   -- glew;
		"glfw3dll", -- glfw;
		"OpenGL32", -- opengl; only works for windows, on mac/linux "GL" should work but don't quote me on that
		"GLu32"     -- glu; same thing here, the file should be called "libGLU.so" or smth, so try "GLU"
	}

	postbuildcommands
	{
		-- copy libs
		"{COPYFILE} \"%{prj.location}vendor/glfw-3.3.4.bin.WIN64/lib-static-ucrt/glfw3.dll\"  \"%{cfg.buildtarget.directory}\"",
		"{COPYFILE} \"%{prj.location}vendor/glew-2.1.0/bin/Release/x64/glew32.dll\"           \"%{cfg.buildtarget.directory}\"",

		-- copy shader files
		"{MKDIR}																			  \"%{cfg.buildtarget.directory}/shaders\"",
		"{COPYDIR}  \"%{prj.location}src/GLSL-Toy/shaders\"                                   \"%{cfg.buildtarget.directory}/shaders\""
	}

	-- tried to change the working directory, doesn't really work, will update this off-stream if i find a solution
	-- debugdir "%{cfg.buildtarget.directory}"

	filter "configurations:Debug"
		defines "MF_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "MF_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "MF_DIST"
		optimize "On"