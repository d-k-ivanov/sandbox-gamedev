project "AssignmentTwo"
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
        "**.png",
        "**.ttf",
        "**.txt",
    }

    vpaths
    {
        ["Archive/*"]   = { "**.md" },
        ["Build/*"]     = { "**.lua" },
        ["Resources/*"] = { "**.png",  "**.ttf", "**.txt" },
        ["Sources/*"]   = { "**.h", "**.cpp" },
    }

    filter ({})

    postbuildcommands
    {
        "{COPYFILE} config.txt %{cfg.targetdir}",
        "{COPYFILE} *.ttf %{cfg.targetdir}",
        "{COPYFILE} grid.png %{cfg.targetdir}"
    }
