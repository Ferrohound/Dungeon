#pragma once

#include <cmath>
#include <iostream>

/*
    macro from the Perlin Noise wiki
 as an alternative, this slightly faster equivalent function (macro) can be used:
 * #define lerp(a0, a1, w) ((a0) + (w)*((a1) - (a0))) 
 */
// math function that I will need to move somewhere else later on
//lerp a to b over t
//https://www.geeksforgeeks.org/inline-functions-cpp/
inline float lerp(float a, float b, float t) {
    return ( 1.0f - t ) * a + (t * b);
}

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