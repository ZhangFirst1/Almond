project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files                          
    {
       "src/**.h",
       "src/**.cpp"
    }

    includedirs                      
    {
        "%{wks.location}/Almond/vendor/spdlog/include",
        "%{wks.location}/Almond/src",
        "%{wks.location}/Almond/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}"
    }

    links                           --链接
    {
        "Almond"
    }

    filter "system:windows"        
        systemversion "latest"

        filter "configurations:Debug"
            defines "AM_DEBUG"
            runtime "Debug"
            symbols "on"            

        filter "configurations:Release"
            defines "AM_Release"
            runtime "Release"
            optimize "on"           

        filter "configurations:Dist"
            defines "AM_DIST"
            runtime "Release"
            optimize "on"