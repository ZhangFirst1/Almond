--Almond
project "Almond"                                                   --项目
    kind "StaticLib"                                            --类型（动态库）
    language "C++"                                              --语言
    cppdialect "C++17"                                          --C++标准（编译时）
    staticruntime "off"                                         --是否将运行时库静态链接运行时库（dll属性的文件需要关闭）

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")               --输出目录(.. XX ..中 ".."是字符串连接符)
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")              --中间目录

    pchheader "ampch.h"                                        --预编译头文件
    pchsource "src/ampch.cpp"                                  --(实际上编译器不需要 但是VS需要这个.cpp文件)
    

    files                                                       --将源代码文件包含到项目中
    {
       "src/**.h",
       "src/**.cpp",
       "vendor/stb_image/**.h",
       "vendor/stb_image/**.cpp",
       "vendor/glm/glm/**.hpp",
       "vendor/glm/glm/**.inl",

       "vendor/ImGuizmo/ImGuizmo.h",
       "vendor/ImGuizmo/ImGuizmo.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
        "YAML_CPP_STATIC_DEFINE"
    }

    includedirs                                                 --库文件（包含库目录）
    {
        "src",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}",                                       --将IncludeDir表中GLFW键索引的值（地址）作为一个库文件路径
        "%{IncludeDir.Glad}",                                       --将IncludeDir表中Glad键索引的值（地址）作为一个库文件路径
        "%{IncludeDir.ImGui}",                                      --将IncludeDir表中ImGui键索引的值（地址）作为一个库文件路径
        "%{IncludeDir.glm}",                                        --将IncludeDir表中glm键索引的值（地址）作为一个库文件路径
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.ImGuizmo}"
    }

    links                                                       --为Almond项目(.dll)链接文件
    {
        "GLFW",                                                     --链接上方group中的项目GLFW
        "Glad",                                                     --链接上方group中的项目Glad
        "ImGui",                                                    --链接上方group中的项目ImGui
        "yaml-cpp",                                                 --链接根目录premake文件里group中的项目yaml-cpp
        "opengl32.lib"
    }

    filter "files:vendor/ImGuizmo/**.cpp"                       --?? why
    flags {"NoPCH"}

    filter "system:windows"                                     --过滤器(如果系统是windows)   >>> !!!  filter, not filters  !!!
        systemversion "latest"                                      --windows SDK 版本

        defines                                                 
        {
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