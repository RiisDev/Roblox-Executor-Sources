#pragma once

struct vec2 {
    union {
        float data[2];
        struct {
            float x;
            float y;
        };
    };

    vec2()
    {
        x = 0;
        y = 0;
    }

    vec2(float ax, float ay)
    {
        x = ax;
        y = ay;
    }
};

struct vec4 {
    float data[4];
};

struct ViewMatrix_t {
    float matrix[16];
};

class vec3 {
public:
    union {
        float data[3];
        struct {
            float x;
            float y;
            float z;
        };
    };

    // Constructors

    // Vectors default to 0, 0, 0.
    vec3()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    // Construct with values, 3D
    vec3(float ax, float ay, float az)
    {
        x = ax;
        y = ay;
        z = az;
    }

    // Construct with values, 2D
    vec3(float ax, float ay)
    {
        x = ax;
        y = ay;
        z = 0;
    }

    // Copy constructor
    vec3(const vec3& o)
    {
        x = o.x;
        y = o.y;
        z = o.z;
    }

    // Addition

    vec3 operator+(const vec3& o) { return vec3(x + o.x, y + o.y, z + o.z); }

    vec3& operator+=(const vec3& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }

    // Subtraction

    vec3 operator-() { return vec3(-x, -y, -z); }

    vec3 operator-(const vec3 o) { return vec3(x - o.x, y - o.y, z - o.z); }

    vec3& operator-=(const vec3 o)
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }

    // Multiplication by scalars

    vec3 operator*(const float s) { return vec3(x * s, y * s, z * s); }

    vec3& operator*=(const float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    // Division by scalars

    vec3 operator/(const float s) { return vec3(x / s, y / s, z / s); }

    vec3& operator/=(const float s)
    {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }

    // Dot product

    float operator*(const vec3 o) { return (x * o.x) + (y * o.y) + (z * o.z); }

    // An in-place dot product does not exist because
    // the result is not a vector.

    // Cross product

    vec3 operator^(const vec3 o)
    {
        float nx = y * o.z - o.y * z;
        float ny = z * o.x - o.z * x;
        float nz = x * o.y - o.x * y;
        return vec3(nx, ny, nz);
    }

    vec3& operator^=(const vec3 o)
    {
        float nx = y * o.z - o.y * z;
        float ny = z * o.x - o.z * x;
        float nz = x * o.y - o.x * y;
        x = nx;
        y = ny;
        z = nz;
        return *this;
    }
};
