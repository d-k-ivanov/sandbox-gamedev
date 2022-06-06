#include "Vec2.h"

#include <cmath>

Vec2::Vec2(const float xin, const float yin) : x(xin), y(yin)
{
}

bool Vec2::operator==(const Vec2 lhs) const
{
    return (x == lhs.x) && (y == lhs.y);
}

bool Vec2::operator!=(const Vec2 lhs) const
{
    return (x != lhs.x) && (y != lhs.y);
}

Vec2 Vec2::operator+(const Vec2 lhs) const
{
    return {x + lhs.x, y + lhs.y};
}

Vec2 Vec2::operator-(const Vec2 lhs) const
{
    return {x - lhs.x, y - lhs.y};
}

Vec2 Vec2::operator*(const Vec2 lhs) const
{
    return {x * lhs.x, y * lhs.y};
}

Vec2 Vec2::operator/(const Vec2 lhs) const
{
    return {x / lhs.x, y / lhs.y};
}

void Vec2::operator+=(const Vec2& lhs)
{
    x += lhs.x;
    y += lhs.y;
}

void Vec2::operator-=(const Vec2& lhs)
{
    x -= lhs.x;
    y -= lhs.y;
}

void Vec2::operator*=(const float lhs)
{
    x *= lhs;
    y *= lhs;
}

void Vec2::operator/=(const float lhs)
{
    x /= lhs;
    y /= lhs;
}

void Vec2::normalize()
{
    const float lenght = std::sqrt(x * x + y * y);

    x = x / lenght;
    y = y / lenght;
}

Vec2 Vec2::normalize(const Vec2 vector)
{
    const float lenght = std::sqrt(vector.x * vector.x + vector.y * vector.y);

    return {vector.x / lenght, vector.x / lenght};
}

float Vec2::lenght() const
{
    return std::sqrt(x * x + y * y);
}

std::ostream& operator<<(std::ostream& stream, const Vec2& lhs)
{
    return stream << lhs.x << " " << lhs.y << '\n';
}
