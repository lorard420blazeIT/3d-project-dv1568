workspace "3D prog project"
    architecture "x64"
    configurations {"Debug", "Release"}
    systemversion "latest"

    project "3D prog project"
        location "3D prog Project"
        files {"%{prj.location}/src/**.cpp", "%{prj.location}/src/**.h"}
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++14"

        targetdir "bin/%{cfg.buildcfg}"
        objdir "objects/%{cfg.buildcfg}"

        filter "configurations:Debug"
            defines{"_DEBUG"}
            symbols "On"
        filter "configurations:Release"
            defines{"NDEBUG"}
            optimize "On"