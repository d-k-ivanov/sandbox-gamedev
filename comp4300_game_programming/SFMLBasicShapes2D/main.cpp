#include "main.h"

#include "Math.h"

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
    app.width = 1500;
    app.height = 900;
    app.fontColor = sf::Color::White;
    app.fontFileName = "arial.ttf";
    app.fontSize = 18;

    if (!app.font.loadFromFile(app.path.string() + "/" + app.fontFileName))
    {
        std::cerr << "Couldn't load font!\n";
    }

    sf::RenderWindow window(sf::VideoMode(app.width, app.height), "SFMLBasicShapes2D", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    // points
    sf::VertexArray points(sf::Points, 10000);
    for (int i = 0; i < 10000; i++)
    {
        points[i].position = sf::Vector2f(
            Math::randomNumber<float>(1.0f, 1499.0f),
            Math::randomNumber<float>(1.0f, 899.0f));
    }

    // lines
    sf::VertexArray lines(sf::Lines, 140);
    {
        float y = 0.0f;
        for (int i = 0; i < 140; i += 2)
        {
            lines[i].position = sf::Vector2f(100.0f, 100.0f + y);
            lines[i + 1].position = sf::Vector2f(1400.0f, 100.0f + y);
            y += 10.0f;
        }
    }

    // strip lines
    sf::VertexArray stripLines(sf::LineStrip, 30);
    {
        float y = 0.0f;
        for (int i = 0; i < 30; i += 2)
        {
            stripLines[i].position = sf::Vector2f(100.0f, 100.0f + y);
            stripLines[i + 1].position = sf::Vector2f(1400.0f, 100.0f + y);
            y += 50.0f;
        }
    }

    // triangles
    sf::VertexArray triangles(sf::Triangles, 9);
    triangles[0].position = sf::Vector2f(150, 350);
    triangles[1].position = sf::Vector2f(250, 250);
    triangles[2].position = sf::Vector2f(350, 350);
    triangles[0].color = sf::Color(255, 0, 74);
    triangles[1].color = sf::Color(255, 0, 74);
    triangles[2].color = sf::Color(255, 0, 74);

    triangles[3].position = sf::Vector2f(400, 200);
    triangles[4].position = sf::Vector2f(600, 300);
    triangles[5].position = sf::Vector2f(400, 400);
    triangles[3].color = sf::Color(153, 255, 70);
    triangles[4].color = sf::Color(153, 255, 70);
    triangles[5].color = sf::Color(153, 255, 70);

    triangles[6].position = sf::Vector2f(600, 400);
    triangles[7].position = sf::Vector2f(800, 100);
    triangles[8].position = sf::Vector2f(800, 400);
    triangles[6].color = sf::Color(255, 255, 0);
    triangles[7].color = sf::Color(255, 255, 0);
    triangles[8].color = sf::Color(255, 255, 0);

    // stripped triangles
    sf::VertexArray trianglesStripped(sf::TriangleStrip, 10);
    trianglesStripped[0].position = sf::Vector2f(100, 200);
    trianglesStripped[1].position = sf::Vector2f(100, 400);
    trianglesStripped[2].position = sf::Vector2f(300, 200);
    trianglesStripped[0].color = sf::Color(0, 181, 99);
    trianglesStripped[1].color = sf::Color(0, 181, 99);
    trianglesStripped[2].color = sf::Color(102, 0, 99);

    trianglesStripped[3].position = sf::Vector2f(300, 400);
    trianglesStripped[4].position = sf::Vector2f(500, 300);
    trianglesStripped[5].position = sf::Vector2f(500, 500);
    trianglesStripped[3].color = sf::Color(102, 0, 99);
    trianglesStripped[4].color = sf::Color(255, 255, 0);
    trianglesStripped[5].color = sf::Color(255, 255, 0);

    trianglesStripped[6].position = sf::Vector2f(700, 300);
    trianglesStripped[7].position = sf::Vector2f(700, 500);
    trianglesStripped[8].position = sf::Vector2f(900, 400);
    trianglesStripped[9].position = sf::Vector2f(900, 600);
    trianglesStripped[6].color = sf::Color(126, 221, 54);
    trianglesStripped[7].color = sf::Color(126, 221, 54);
    trianglesStripped[8].color = sf::Color(0, 0, 205);
    trianglesStripped[9].color = sf::Color(0, 0, 205);

    // triangles with the one same vertex
    sf::VertexArray triangleFan(sf::TriangleFan, 12);
    triangleFan[0].position = sf::Vector2f(250, 200);
    triangleFan[1].position = sf::Vector2f(350, 250);
    triangleFan[2].position = sf::Vector2f(300, 300);
    triangleFan[0].color = sf::Color::Yellow;
    triangleFan[1].color = sf::Color::White;
    triangleFan[2].color = sf::Color::White;

    triangleFan[3].position = sf::Vector2f(250, 315);
    triangleFan[4].position = sf::Vector2f(200, 300);
    triangleFan[5].position = sf::Vector2f(140, 250);
    triangleFan[3].color = sf::Color::Red;
    triangleFan[4].color = sf::Color::Red;
    triangleFan[5].color = sf::Color::Magenta;

    triangleFan[6].position = sf::Vector2f(130, 200);
    triangleFan[7].position = sf::Vector2f(140, 150);
    triangleFan[8].position = sf::Vector2f(200, 100);
    triangleFan[9].position = sf::Vector2f(250, 80);
    triangleFan[10].position = sf::Vector2f(300, 100);
    triangleFan[11].position = sf::Vector2f(350, 150);
    triangleFan[6].color = sf::Color::Magenta;
    triangleFan[7].color = sf::Color::Green;
    triangleFan[8].color = sf::Color::Green;
    triangleFan[9].color = sf::Color::Cyan;
    triangleFan[10].color = sf::Color::Cyan;
    triangleFan[11].color = sf::Color::Yellow;

    // quads
    sf::VertexArray quads(sf::Quads, 12);
    quads[0].position = sf::Vector2f(400, 300);
    quads[1].position = sf::Vector2f(400, 200);
    quads[2].position = sf::Vector2f(500, 200);
    quads[3].position = sf::Vector2f(500, 300);
    quads[0].color = sf::Color(255, 102, 255);
    quads[1].color = sf::Color(255, 102, 255);
    quads[2].color = sf::Color(255, 102, 255);
    quads[3].color = sf::Color(255, 102, 255);

    quads[4].position = sf::Vector2f(600, 300);
    quads[5].position = sf::Vector2f(600, 200);
    quads[6].position = sf::Vector2f(800, 200);
    quads[7].position = sf::Vector2f(800, 300);
    quads[4].color = sf::Color(102, 0, 99);
    quads[5].color = sf::Color(102, 0, 99);
    quads[6].color = sf::Color(102, 0, 99);
    quads[7].color = sf::Color(102, 0, 99);

    quads[8].position = sf::Vector2f(500, 500);
    quads[9].position = sf::Vector2f(600, 400);
    quads[10].position = sf::Vector2f(800, 400);
    quads[11].position = sf::Vector2f(700, 500);
    quads[8].color = sf::Color(255, 88, 0);
    quads[9].color = sf::Color(255, 88, 0);
    quads[10].color = sf::Color(255, 88, 0);
    quads[11].color = sf::Color(255, 88, 0);

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

        window.clear();
        // window.draw(points);
        // window.draw(lines);
        // window.draw(stripLines);
        // window.draw(triangles);
        // window.draw(trianglesStripped);
        // window.draw(triangleFan);
        window.draw(quads);
        window.display();
    }

    return 0;
}
