project "SFMLSolarSystem"
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
        ["Resources/*"] = { "**.ttf", "**.txt", "**.jpg" },
        ["Sources/*"]   = {"**.c", "**.cpp"},
    }

    filter ({})

    postbuildcommands
    {
        "{COPYFILE} *.jpg %{cfg.targetdir}",
        "{COPYFILE} *.ttf %{cfg.targetdir}"
    }
