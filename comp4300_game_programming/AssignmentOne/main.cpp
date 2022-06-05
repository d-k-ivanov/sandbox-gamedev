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

struct Rect
{
    std::string N;
    float X{}, Y{}, W{}, H{};
    float SX{}, SY{};
    sf::Uint8 R{}, G{}, B{};
    sf::Text name;
    sf::RectangleShape shape;
    sf::Vector2f speed;
};


struct Circ
{
    std::string N;
    float X{}, Y{}, S{};
    float SX{}, SY{};
    sf::Uint8 R{}, G{}, B{};
    sf::Text name;
    sf::CircleShape shape;
    sf::Vector2f speed;
};


fs::path getAppPath()
{
#if defined(_MSC_VER)
    wchar_t path[FILENAME_MAX] = {0};
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

    std::vector<Rect> rectangles;
    std::vector<Circ> circles;

    std::ifstream stream(app.path.string() + "/config.txt");
    std::string first;
    // while (stream >> first)
    // {
    //     std::cout << first << " ";
    // }
    // return 0;

    while (stream >> first)
    {
        if (first == "Window")
        {
            stream >> app.width >> app.height;
        }
        else if (first == "Font")
        {
            int R, G, B;
            stream >> app.fontFileName >> app.fontSize >> R >> G >> B;
            if (!app.font.loadFromFile(app.path.string() + "/" + app.fontFileName))
            {
                std::cerr << "Couldn't load font!\n";
            }
            app.fontColor = sf::Color(
                static_cast<sf::Uint8>(R),
                static_cast<sf::Uint8>(G),
                static_cast<sf::Uint8>(B));
        }
        else if (first == "Rectangle")
        {
            std::string n;
            int x, y, w, h;
            float sx, sy;
            int r, g, b;

            stream >> n >> x >> y >> sx >> sy >> r >> g >> b >> w >> h;
            Rect rect;
            rect.N = n;
            rect.X = static_cast<float>(x);
            rect.Y = static_cast<float>(y);
            rect.SX = sx;
            rect.SY = sy;
            rect.R = static_cast<sf::Uint8>(r);
            rect.G = static_cast<sf::Uint8>(g);
            rect.B = static_cast<sf::Uint8>(b);
            rect.W = static_cast<float>(w);
            rect.H = static_cast<float>(h);

            rect.shape = sf::RectangleShape(sf::Vector2f(rect.W, rect.H));
            rect.shape.setFillColor(sf::Color(rect.R, rect.G, rect.B));
            rect.shape.setPosition(rect.X, rect.Y);

            rect.name = sf::Text(rect.N, app.font, app.fontSize);
            auto nameW = rect.name.getLocalBounds().width;
            auto nameH = rect.name.getLocalBounds().height;
            rect.name.setPosition(
                rect.X - round(nameW / 2.0f) + round(rect.W / 2.0f),
                rect.Y - round(nameH) / 2.0f + round(rect.H / 2.0f));

            rectangles.push_back(rect);
        }
        else if (first == "Circle")
        {
            std::string n;
            int x, y, s;
            float sx, sy;
            int r, g, b;

            stream >> n >> x >> y >> sx >> sy >> r >> g >> b >> s;

            Circ circ;
            circ.N = n;
            circ.X = static_cast<float>(x);
            circ.Y = static_cast<float>(y);
            circ.SX = sx;
            circ.SY = sy;
            circ.R = static_cast<sf::Uint8>(r);
            circ.G = static_cast<sf::Uint8>(g);
            circ.B = static_cast<sf::Uint8>(b);
            circ.S = static_cast<float>(s);

            circ.shape = sf::CircleShape(circ.S);
            circ.shape.setFillColor(sf::Color(circ.R, circ.G, circ.B));
            circ.shape.setPosition(circ.X, circ.Y);

            circ.name = sf::Text(circ.N, app.font, app.fontSize);
            auto nameW = circ.name.getLocalBounds().width;
            auto nameH = circ.name.getLocalBounds().height;
            circ.name.setPosition(circ.X - nameW/2 + circ.S, circ.Y - nameH/2 + circ.S);

            circles.push_back(circ);
        }
    }

    sf::RenderWindow window(sf::VideoMode(app.width, app.height), "Assignment 1!");


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
        for (auto& rectangle : rectangles)
        {
            sf::Vector2f position = rectangle.shape.getPosition();
            if (position.x < 0.0f || static_cast<float>(window.getSize().x) - rectangle.W < position.x)
            {
                rectangle.SX *= -1.0f;
            }

            if (position.y < 0.0f || static_cast<float>(window.getSize().y) - rectangle.H < position.y)
            {
                rectangle.SY *= -1.0f;
            }

            sf::Vector2f moveVector = sf::Vector2f(rectangle.SX, rectangle.SY);

            rectangle.shape.setPosition(position + moveVector);
            window.draw(rectangle.shape);


            sf::Vector2f namePosition = rectangle.name.getPosition();
            rectangle.name.setPosition(namePosition + moveVector);
            window.draw(rectangle.name);
        }

        for (auto& circle : circles)
        {
            sf::Vector2f position = circle.shape.getPosition();

            if (position.x < 0.0f || static_cast<float>(window.getSize().x) - circle.S * 2 < position.x)
            {
                circle.SX *= -1.0f;
            }

            if (position.y < 0.0f || static_cast<float>(window.getSize().y) - circle.S * 2 < position.y)
            {
                circle.SY *= -1.0f;
            }

            sf::Vector2f moveVector = sf::Vector2f(circle.SX, circle.SY);

            circle.shape.setPosition(position + moveVector);
            window.draw(circle.shape);

            sf::Vector2f namePosition = circle.name.getPosition();
            circle.name.setPosition(namePosition + moveVector);
            window.draw(circle.name);
        }
        window.display();
    }

    return 0;
}
