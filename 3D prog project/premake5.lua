workspace "3D prog project"
    architecture "x64"
    configurations {"Debug", "Release"}
    systemversion "latest"

    project "3D prog project"
        location "3D prog Project"
        files {"%{prj.location}/**.cpp", "%{prj.location}/**.h","%{prj.location}/**.hlsl"}
        kind "WindowedApp"
        language "C++"
        cppdialect "C++14"

        targetdir "bin/%{cfg.buildcfg}"
        objdir "objects/%{cfg.buildcfg}"

        includedirs{"include/directXtk/"}

        links{
            "d3d11"
        }
	vpaths{["Headers"] = "**.h", ["Sources"] = {"**.c", "**.cpp"},["Shaders"] = "**.hlsl"}
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

    filter "*"
        local ws = "$(ProjectDir)%%(Filename).cso"
        files("*.hlsl")
            shadermodel("5.0")
            shaderobjectfileoutput(ws)

        filter("files:**_vs.hlsl")
            shadertype("Vertex")

        filter("files:**_ps.hlsl")
            shadertype("Pixel")
            



