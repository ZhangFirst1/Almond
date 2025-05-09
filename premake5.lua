workspace "Almond"
	architecture "x64"
	startproject "Almond-Editor"

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
IncludeDir["entt"] = "Almond/vendor/entt/include"
IncludeDir["stb_image"] = "Almond/vendor/stb_image"
IncludeDir["yaml_cpp"] = "Almond/vendor/yaml-cpp/include"

include "Almond/vendor/GLFW"
include "Almond/vendor/Glad"
include "Almond/vendor/imgui"
include "Almond/vendor/yaml-cpp"

project "Almond"
	location "Almond"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs {
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
	}

	links{
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui",
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"AM_PLATFORM_WINDOWS",
			"AM_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "AM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "AM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AM_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
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
		symbols "on"

	filter "configurations:Release"
		defines "AM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AM_DIST"
		runtime "Release"
		optimize "on"


project "Almond-Editor"
	location "Almond-Editor" 
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	buildoptions{
		"/utf-8"
	}

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines{
		"YAML_CPP_STATIC_DEFINE"
	}

	includedirs {
		"$(SolutionDir)/Almond/vendor/spdlog/include",
		"$(SolutionDir)Almond/src",
		"$(SolutionDir)Almond/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}"
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
		symbols "on"

	filter "configurations:Release"
		defines "AM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "AM_DIST"
		runtime "Release"
		optimize "on"