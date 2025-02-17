workspace "Almond"
	architecture "x64"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Almond/vendor/GLFW/include"

include "Almond/vendor/GLFW"

project "Almond"
	location "Almond"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ampch.h"
	pchsource "Almond/src/ampch.cpp"

	buildoptions{
		"/utf-8"
	}

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}"
	}

	links{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"AM_PLATFORM_WINDOWS",
			"AM_BUILD_DLL"
		}

		postbuildcommands{
			("{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "AM_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "AM_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AM_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	buildoptions{
		"/utf-8"
	}

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"$(SolutionDir)/Almond/vendor/spdlog/include",
		"$(SolutionDir)Almond/src"
	}

	links{
		"Almond"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"AM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "AM_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "AM_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AM_DIST"
		optimize "On"