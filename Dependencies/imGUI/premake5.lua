project "imGUI"
    kind "StaticLib"
    language "C++"

    targetdir ("out/bin/" .. output_directory .. "/%{prj.name}")
    objdir ("out/bin-int/" .. output_directory .. "/%{prj.name}")
    
    

    files
    {
        "imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
    }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    --might add linux

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"