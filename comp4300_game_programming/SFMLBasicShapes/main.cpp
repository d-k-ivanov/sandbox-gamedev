#include "main.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace fs = std::filesystem;

struct Application
{
    unsigned width, height, fontSize;
    fs::path path;
    sf::Font font;
    std::string fontFileName;
    sf::Color fontColor;
};

struct Font
{
    std::string F;
    int S, R, G, B;
};

fs::path getAppPath()
{
#if defined(_MSC_VER)
    wchar_t path[FILENAME_MAX] = { 0 };
    GetModuleFileNameW(nullptr, path, FILENAME_MAX);
    return fs::path(path).parent_path();
#else
    char path[FILENAME_MAX];
    ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
    return fs::path(std::string(path, (count > 0) ? count : 0)).parent_path()
#endif
}

int main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);

    Application app{};


    app.path = getAppPath();
    app.width = 1024;
    app.height = 1024;
    app.fontColor = sf::Color::White;
    app.fontFileName = "arial.ttf";
    app.fontSize = 18;

    if (!app.font.loadFromFile(app.path.string() + "/" + app.fontFileName)) {
        std::cerr << "Couldn't load font!\n";
    }

    sf::RenderWindow window(sf::VideoMode(app.width, app.height), "SFMLBasicShapes");

    sf::CircleShape shape(100.f, 3);
    shape.setPosition(100, 100);
    shape.setFillColor(sf::Color::Magenta);

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        // rendering
        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
