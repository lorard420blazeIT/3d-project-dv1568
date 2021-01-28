workspace "3D prog project"
    architecture "x64"
    configurations {"Debug", "Release"}
    systemversion "latest"

    project "3D prog project"
        location "3D prog Project"
        files {"%{prj.location}/src/**.cpp", "%{prj.location}/src/**.h","%{prj.location}/src/**.hlsl"}
        kind "WindowedApp"
        language "C++"
        cppdialect "C++14"

        targetdir "bin/%{cfg.buildcfg}"
        objdir "objects/%{cfg.buildcfg}"

        links{
            "d3d11"
        }
	vpaths{["Headers"] = "**.h", ["Sources"] = {"**.c", "**.cpp"},["Shaders"] = "**.hlsl"}
        filter "configurations:Debug"
            defines{"_DEBUG", "_UNICODE", "UNICODE"}
            symbols "On"
        filter "configurations:Release"
            defines{"NDEBUG", "_UNICODE", "UNICODE"}
            optimize "On"
