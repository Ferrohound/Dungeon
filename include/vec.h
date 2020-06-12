#pragma once

#include <cmath>
#include <iostream>

struct vec2
{
    union
    {
        struct { float x, y; };
        float axes[2];
    };

    float magnitude() { return ( sqrt( pow(x, 2) + pow(y, 2) ) ); }
    vec2 normalize() { vec2 out; out.x = x / magnitude(); out.y = y/magnitude(); return out; }
    void scale(float factor) {x*=factor; y*=factor;}

    static vec2 VecMidpoint(vec2 a, vec2 b)
    {
        vec2 out;
        out.x = a.x + b.x / 2;
        out.y = a.y + b.y / 2;

        return out;
    }

    static float VecDistance(vec2 a, vec2 b)
    {
        float val = pow(b.x - a.x, 2) + pow( b.y - a.y, 2 );
        float result =  sqrt( val );
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const vec2& vec)
    {
        os << "( " << vec.x << "," << vec.y << " )";
        return os;
    }

    vec2 operator+ (vec2 const& other)
    {
        vec2 out = {0, 0};

        out.x = x + other.x;
        out.y = y + other.y;

        return out;
    }

    vec2 operator- (vec2 const& other)
    {
        vec2 out = {0, 0};

        out.x = x - other.x;
        out.y = y - other.y;
        
        return out;
    }
};