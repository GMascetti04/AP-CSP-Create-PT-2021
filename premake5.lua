workspace "CreateProjectAPCSP"
    architecture "x86_64"
    startproject "Sandbox"

    configurations 
    {
        "Debug",
        "Release",
        "Distribution"
    
    }

    flags
    {
        "MultiProcessorCompile"
    }

    output_directory = "%{cfg.buildcfg}/%{cfg.architecture}"

    --relative to the root folder, solution directory
    Includes = {}
    Includes["GLFW"] = "Dependencies/GLFW/include"
    Includes["GLEW"] = "Dependencies/GLEW/include"
    Includes["imGUI"] = "Dependencies/imGUI"
    Includes["spdlog"] = "Dependencies/spdlog/include"
    Includes["glm"] = "Dependencies/glm/"


    group "Dependencies"
        include "Dependencies/GLFW"
        include "Dependencies/GLEW"
        include "Dependencies/imGUI"
        include "Dependencies/spdlog"

    group ""

    project "Sandbox"
        location "Sandbox"
        --kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        
      


        targetdir ("out/bin/" .. output_directory .. "/%{prj.name}")
        objdir ("out/bin-int/" .. output_directory .. "/%{prj.name}")
        --pchheader
        --pchsource
        
        pchheader "xpch.h"
        pchsource "%{prj.name}/src/xpch.cpp"
        files 
        {
        
            "%{prj.name}/src/Engine/**.cpp",
            "%{prj.name}/src/Engine/**.h",
            "%{prj.name}/src/Engine/**.hpp",

            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/src/Game/**.h",
            "%{prj.name}/src/Game/**.cpp",
            --remove the bottom two eventually
            "Dependencies/imGUI/backends/imgui_impl_glfw.cpp",
            "Dependencies/imGUI/backends/imgui_impl_opengl3.cpp",
            --might remove: glm files
            --"Dependencies/glm/glm/**.hpp",
            --"Dependencies/glm/glm/**.inl"
        }

        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "GLEW_STATIC"
        }

        includedirs
        {
           "%{Includes.GLFW}",
           "%{Includes.GLEW}",
           "%{Includes.imGUI}",
           "%{Includes.spdlog}",
           "%{Includes.glm}",
           "%{prj.name}/src",
           "%{prj.name}/src/Engine/vendor/glm"
        }

        links
        {
            "GLFW",
            "GLEW",
            "imGUI",
            "spdlog"
        }

        --might edit these
        filter "system:windows"
            systemversion "latest"
            links
            {
                "opengl32.lib"
            }
            
        filter "system:linux"
            links
            {
                "pthread",
                "X11",
                "GL",
                "dl"
            }

        filter "configurations:Debug"
            defines "DEBUG"
            runtime "Debug"
            symbols "on"
            kind "ConsoleApp"
            targetname("%{prj.name}-DEBUG")
            

        filter "configurations:Release"
            defines "RELEASE"
            runtime "Release"
            optimize "on"
            kind "WindowedApp"
            targetname("%{prj.name}-RELEASE")

        filter "configurations:Distribution"
            defines "DISTRIBUTION"
            runtime "Release"
            optimize "on"
            kind "WindowedApp"
            targetname("%{prj.name}-DISTRIBUTION")
            --targetname("%{prj.name} Release")
       