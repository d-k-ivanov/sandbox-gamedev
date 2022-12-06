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

    sf::RenderWindow window(sf::VideoMode(app.width, app.height), "SFMLBasicShapes", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    sf::CircleShape shape(100.f, 3);
    shape.setPosition(50, 20);
    shape.setFillColor(sf::Color::Magenta);

    sf::RectangleShape rect1(sf::Vector2f(200, 100));
    sf::RectangleShape rect2(sf::Vector2f(200, 100));
    sf::RectangleShape rect3(sf::Vector2f(200, 100));

    rect1.setPosition(400, 20);
    rect2.setPosition(800, 20);
    rect3.setPosition(800, 150);

    rect1.setFillColor(sf::Color::Yellow);
    rect2.setFillColor(sf::Color::Red);
    rect3.setFillColor(sf::Color::Green);

    rect1.setOutlineThickness(3);
    rect2.setOutlineThickness(5);
    rect3.setOutlineThickness(7);

    rect1.setOutlineColor(sf::Color::White);
    rect2.setOutlineColor(sf::Color::White);
    rect3.setOutlineColor(sf::Color::White);

    rect3.setOrigin(sf::Vector2f(100, 50));

    rect1.setRotation(45);
    rect2.setRotation(90);
    rect3.setRotation(90);


    sf::RectangleShape rect4(sf::Vector2f(200, 100));
    rect4.setOrigin(100, 50);
    rect4.setPosition(100, 400);
    rect4.setFillColor(sf::Color::White);
    rect4.setOutlineThickness(3);
    rect4.setOutlineColor(sf::Color::White);

    sf::Texture texture;
    texture.loadFromFile("brick_s.jpg");
    rect4.setTexture(&texture);

    sf::ConvexShape convex;
    convex.setPointCount(8);
    convex.setPoint(0, sf::Vector2f(600, 600));
    convex.setPoint(1, sf::Vector2f(650, 750));
    convex.setPoint(2, sf::Vector2f(800, 800));
    convex.setPoint(3, sf::Vector2f(650, 850));
    convex.setPoint(4, sf::Vector2f(600, 1000));
    convex.setPoint(5, sf::Vector2f(550, 850));
    convex.setPoint(6, sf::Vector2f(400, 800));
    convex.setPoint(7, sf::Vector2f(550, 750));
    convex.setFillColor(sf::Color::Cyan);
    convex.setOutlineThickness(3);
    convex.setOutlineColor(sf::Color::White);

    float x_move = 3.0f;
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

        rect4.rotate(1.0f);
        rect4.move(x_move, 0);
        if (rect4.getPosition().x > 800)
        {
            x_move = -3.0f;
        }
        if (rect4.getPosition().x < 200)
        {
            x_move = 3.0f;
        }

        window.clear();
        window.draw(shape);
        window.draw(rect1);
        window.draw(rect2);
        window.draw(rect3);
        window.draw(rect4);
        window.draw(convex);
        window.display();
    }

    return 0;
}
