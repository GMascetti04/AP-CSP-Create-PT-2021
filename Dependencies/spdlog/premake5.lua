project "spdlog"
    kind "StaticLib"
    language "C++"

    targetdir ("out/bin/" .. output_directory .. "/%{prj.name}")
    objdir ("out/bin-int/" .. output_directory .. "/%{prj.name}")

    includedirs
    {
        "include/"
    }
    
    defines 
    {
        "_CRT_SECURE_NO_WARNINGS",
        "SPDLOG_COMPILED_LIB"
    }

    files
    {
        "src/*.cpp",
        "include/spdlog/spdlog.h"
     
    }

    --make a filter for linux maybe
    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"