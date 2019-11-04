workspace "Mandlebrot"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["glm"] = "vendor/glm/"

include "vendor/GLFW"
include "vendor/Glad"

project "Mandlebrot"
	location "Mandlebrot"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
	}

	links
	{
		"GLFW",
		"Glad",
	}

	filter "system:linux"
		pic "on"
		systemversion "latest"

		links
		{
			"Xrandr",
			"Xi",
			"X11",
			"dl",
			"pthread"
		}

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "system:windows"
		systemversion "latest"

		links
		{
			"opengl32.lib"
		}

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
