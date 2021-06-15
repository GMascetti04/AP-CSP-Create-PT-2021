project "GLEW"
    kind "StaticLib"
    language "C"

    --changes the char set
    characterset ("MBCS")

    targetdir ("out/bin/" .. output_directory .. "/%{prj.name}")
    objdir ("out/bin-int/" .. output_directory .. "/%{prj.name}")
    
    includedirs 
    {  
        --might need to remove:
        "include/"
    }

    defines
    {
        "GLEW_STATIC",
        "_CRT_SECURE_NO_WARNINGS"
    }
    files 
    {
        "include/GL/glew.h",
        "src/glew.c",
        "src/glewinfo.c",
        "src/visualinfo.c"
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