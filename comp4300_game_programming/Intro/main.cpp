#include "main.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    ((void)argc);
    ((void)argv);

    const int width = 1024;
    const int height = 768;

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML Works!");

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(300.0f, 300.0f);
    [[maybe_unused]] float circleMoveSpeed = -0.01f;

    sf::Font font;

    #if defined(_MSC_VER)
    wchar_t path[FILENAME_MAX] = { 0 };
    GetModuleFileNameW(nullptr, path, FILENAME_MAX);
    auto appPath = std::filesystem::path(path).parent_path();
    #else
    char path[FILENAME_MAX];
    ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
    auto appPath = std::filesystem::path(std::string(path, (count > 0) ? count : 0)).parent_path()
    #endif

    if (!font.loadFromFile(appPath.string() + "/arial.ttf"))
    {
        std::cerr << "Couldn't load font!\n";
    }

    sf::Text text("Sample Text", font, 24);
    text.setPosition(0, height - static_cast<float>(text.getCharacterSize()));

    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << "\n";
                if (event.key.code == sf::Keyboard::X)
                {
                    circleMoveSpeed *= -1.0f; // Reverse
                }
            }
        }

        // animations
        sf::Vector2f prevPosition = circle.getPosition();
        sf::Vector2f moveVector = sf::Vector2f(circleMoveSpeed, circleMoveSpeed);
        sf::Vector2f newPosition = prevPosition + moveVector;
        circle.setPosition(newPosition);

        // rendering
        window.clear();
        window.draw(circle);
        window.draw(text);
        window.display();
    }

    return 0;
}
