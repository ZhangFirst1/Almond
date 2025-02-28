workspace "Almond"
	architecture "x64"
	startproject "Sandbox"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Almond/vendor/GLFW/include"
IncludeDir["Glad"] = "Almond/vendor/Glad/include"
IncludeDir["ImGui"] = "Almond/vendor/imgui"
IncludeDir["glm"] = "Almond/vendor/glm"

include "Almond/vendor/GLFW"
include "Almond/vendor/Glad"
include "Almond/vendor/imgui"

project "Almond"
	location "Almond"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ampch.h"
	pchsource "Almond/src/ampch.cpp"

	buildoptions{
		"/utf-8"
	}

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"AM_PLATFORM_WINDOWS",
			"AM_BUILD_DLL"
		}

		postbuildcommands{
			("{COPYFILE} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
		}

	filter "configurations:Debug"
		defines "AM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "AM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "AM_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"$(SolutionDir)Almond/src",
		"$(SolutionDir)Almond/vendor",
		"%{IncludeDir.glm}"
	}

	links{
		"Almond"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"AM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "AM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "AM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "AM_DIST"
		runtime "Release"
		optimize "On"