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

class PlanetPosition
{
public:
    PlanetPosition(float startx, float starty, float r, float v);
    PlanetPosition(float r, float v);
    float getx() const;
    float gety() const;
    void move(double s);
    void setposition(float startx, float starty);

private:
    float x0 = 0, y0 = 0, rad = 0, x = 0, y = 0, vector = 1;
    double a = 0;
    const double PI = acos(-1.0);
};

PlanetPosition::PlanetPosition(const float startx, const float starty, const float r, float v)
{
    if (v > 1) v = 1;
    if (v < -1 || v == 0) v = -1;
    vector = v;
    x0 = startx;
    y0 = starty;
    rad = r;
}

PlanetPosition::PlanetPosition(float r, float v)
{
    if (v > 1) v = 1;
    if (v < -1 || v == 0.0f) v = -1;
    vector = v;
    rad = r;
}

float PlanetPosition::getx() const
{
    return x;
}

float PlanetPosition::gety() const
{
    return y;
}

void PlanetPosition::move(const double s)
{
    a += s;
    if (a > 360) a = 0;
    x = x0 + static_cast<float>(round(rad * cos(a * PI / 180)));
    y = y0 + static_cast<float>(round(rad * sin(a * PI / 180))) * vector;
}

void PlanetPosition::setposition(const float startx, const float starty)
{
    x0 = startx;
    y0 = starty;
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

    sf::RenderWindow window(sf::VideoMode(app.width, app.height), "SFMLSolarSystem", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    // stars
    sf::VertexArray stars(sf::Points, 500);
    for (int i = 0; i < 500; i++)
    {
        stars[i].color = sf::Color(static_cast<sf::Uint8>(Math::randomNumber<int>(0, 255)), 200, 255);
        stars[i].position = sf::Vector2f(
            Math::randomNumber<float>(1.0f, 1499.0f),
            Math::randomNumber<float>(1.0f, 899.0f));
    }

    sf::CircleShape sun(100);
    sun.setPosition(750, 450);
    sun.setOrigin(100, 100);
    sun.setFillColor(sf::Color::Yellow);

    sf::CircleShape mercury(1);
    mercury.setFillColor(sf::Color::Green);
    PlanetPosition mercuryPos(750, 450, 150, -1);

    sf::CircleShape venus(6);
    venus.setOrigin(3, 3);
    venus.setFillColor(sf::Color::Magenta);
    PlanetPosition venusPos(750, 450, 200, -1);

    sf::CircleShape earth(10);
    earth.setOrigin(5, 5);
    earth.setFillColor(sf::Color::Blue);
    PlanetPosition earthPos(750, 450, 250, -1);

    sf::CircleShape moon(4);
    moon.setOrigin(2, 2);
    moon.setFillColor(sf::Color::White);
    PlanetPosition moonPos(30, -1);

    sf::CircleShape mars(4);
    mars.setOrigin(2, 2);
    mars.setFillColor(sf::Color::Red);
    PlanetPosition parsPos(750, 450, 300, -1);

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

        sun.rotate(-1);
        mercuryPos.move(1);
        mercury.setPosition(mercuryPos.getx(), mercuryPos.gety());

        venusPos.move(0.5);
        venus.setPosition(venusPos.getx(), venusPos.gety());

        earthPos.move(0.8);
        earth.setPosition(earthPos.getx(), earthPos.gety());

        moonPos.setposition(earth.getPosition().x, earth.getPosition().y);
        moonPos.move(3);
        moon.setPosition(moonPos.getx(), moonPos.gety());

        parsPos.move(1.5);
        mars.setPosition(parsPos.getx(), parsPos.gety());

        window.clear();
        window.draw(stars);
        window.draw(sun);
        window.draw(mercury);
        window.draw(venus);
        window.draw(earth);
        window.draw(mars);
        window.draw(moon);
        window.display();
    }

    return 0;
}
