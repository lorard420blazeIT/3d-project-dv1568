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

        includedirs{"include/directXtk/"}

        links{
            "d3d11"
        }
        	vpaths{["Shaders"] = "**.hlsl", ["Headers"] = "**.h", ["Sources"] = {"**.c", "**.cpp"}, ["PreCompiledHeaders"] = "**pch", ["Main"] = "**main.cpp" }

        filter "configurations:Debug"
            libdirs{"lib/directXtk/x64/Debug/"}
            links{"DirectXTK"}

            defines{"_DEBUG", "_UNICODE", "UNICODE"}
            symbols "On"
        filter "configurations:Release"
            libdirs{"lib/directXtk/x64/Release/"}
            links{"DirectXTK"}
            defines{"NDEBUG", "_UNICODE", "UNICODE"}
            optimize "On"


        filter("files:**.hlsl")
            shadermodel("5.0")

        filter("files:**_vs.hlsl")
            shadertype("Vertex")

        filter("files:**_ps.hlsl")
            shadertype("Pixel")
            shadertype("Pixel")


