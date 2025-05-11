project "Almond-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    buildoptions{
        "/utf-8"
    }

    files                          
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    defines
    {
        "YAML_CPP_STATIC_DEFINE"
    }

    includedirs                      
    {
        "%{wks.location}/Almond/vendor/spdlog/include",
        "%{wks.location}/Almond/src",
        "%{wks.location}/Almond/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links
    {
        "Almond"
    }

    filter "system:windows"   
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