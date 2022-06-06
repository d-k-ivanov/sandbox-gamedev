#pragma once

#include <iostream>

class Vec2
{
public:
    float x = 0;
    float y = 0;

    Vec2() = default;
    Vec2(float xin, float yin);

    bool operator ==(Vec2 lhs) const;
    bool operator !=(Vec2 lhs) const;

    Vec2 operator +(Vec2 lhs) const;
    Vec2 operator -(Vec2 lhs) const;
    Vec2 operator *(Vec2 lhs) const;
    Vec2 operator /(Vec2 lhs) const;

    void operator +=(const Vec2& lhs);
    void operator -=(const Vec2& lhs);
    void operator *=(float lhs);
    void operator /=(float lhs);

    void normalize();
    static Vec2 normalize(Vec2 vector);
    [[nodiscard]] float lenght() const;

    friend std::ostream& operator <<(std::ostream& stream, const Vec2& lhs);
};
