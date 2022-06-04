project "Intro"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    files
    {
        "**.c",
        "**.cc",
        "**.cpp",
        "**.cxx",
        "**.h",
        "**.hpp",
        "**.hxx",
        "**.lua",
        "**.md",
        "**.ttf",
        "**.txt",
    }

    vpaths
    {
        ["Archive/*"]   = {"**.md"},
        ["Build/*"]     = {"**.lua"},
        ["Headers/*"]   = { "**.h", "**.hpp" },
        ["Resources/*"] = { "**.ttf", "**.txt" },
        ["Sources/*"]   = {"**.c", "**.cpp"},
    }

    filter ({})

    postbuildcommands
    {
        "{COPY} config.txt %{cfg.targetdir}",
        "{COPY} *.ttf %{cfg.targetdir}"
    }
