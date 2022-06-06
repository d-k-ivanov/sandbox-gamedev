#pragma once

#include "Vec2.h"

#include <SFML/Graphics.hpp>

class CTransform
{
public:
    Vec2 pos      = {0.0, 0.0};
    Vec2 velocity = {0.0, 0.0};
    float angle   = 0.0f;

    CTransform(const Vec2& p, const Vec2& s, const float a) : pos(p), velocity(s), angle(a)
    {
    }
};


class CShape
{
public:
    sf::CircleShape circle;

    CShape(const float radius, const int points, const sf::Color& fill, const sf::Color& outline, const float thickness)
        : circle(radius, points)
    {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
        circle.setOrigin(radius, radius);
    }
};

class CCollision
{
public:
    float radius = 0.0f;

    explicit CCollision(const float r) : radius(r)
    {
    }
};

class CScore
{
public:
    int score = 0;

    explicit CScore(const int s) : score(s)
    {
    }
};

class CLifeSpan
{
public:
    int remaining = 0;
    int total     = 0;

    explicit CLifeSpan(const int total) : remaining(total), total(total)
    {
    }
};

class CInput
{
public:
    bool up         = false;
    bool left       = false;
    bool right      = false;
    bool down       = false;
    bool leftMouse  = false;
    bool rightMouse = false;

    CInput() = default;
};
