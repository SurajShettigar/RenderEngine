#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include "math_utils.h"

namespace math
{
    class Vector3
    {
    private:
        double v[3]{0, 0, 0};

    public:
        // Constructors
        Vector3() = default;
        Vector3(double x = 0.0, double y = 0.0, double z = 0.0) : v{x, y, z} {}

        // Access and assigments
        Vector3(const Vector3 &w);
        Vector3 &operator=(const Vector3 &w);

        double &x = v[0];
        double &y = v[1];
        double &z = v[2];

        double operator[](int i) const { return v[i]; }
        double &operator[](int i) { return v[i]; }

        // Current Vector operations
        Vector3 operator-() const;
        Vector3 &operator+=(const Vector3 &w);
        Vector3 &operator-=(const Vector3 &w);
        Vector3 &operator*=(const double a);
        Vector3 &operator/=(const double a);

        // Vector Specific operataions
        double length() const;
        double lengthSquared() const;
        Vector3 normalize() const;

        static double dot(const Vector3 &v, const Vector3 &w);
        static Vector3 cross(const Vector3 &v, const Vector3 &w);
        static Vector3 normalize(const Vector3 &v);
        static Vector3 lerp(const Vector3 &v, const Vector3 &w, double t);
        static Vector3 reflect(const Vector3 &v, const Vector3 &normal);

        // Arithmetic operations
        friend Vector3 operator+(const Vector3 &v, const Vector3 &w);
        friend Vector3 operator-(const Vector3 &v, const Vector3 &w);
        friend Vector3 operator*(const Vector3 &v, const Vector3 &w);
        friend Vector3 operator*(const Vector3 &v, double a);
        friend Vector3 operator*(double a, const Vector3 &v);
        friend Vector3 operator/(const Vector3 &v, double a);

        // Other operations
        static Vector3 random();
        static Vector3 random(double min, double max);
        static Vector3 randomCircular();
        static Vector3 randomSpherical();
        static Vector3 randomHemiSpherical(const Vector3 &normal);
        friend std::ostream &operator<<(std::ostream &out, const Vector3 &v);

        // Comparison operations
        friend bool operator==(const Vector3 &v, const Vector3 &w);

        // static values
        static const Vector3 zero;
        static const Vector3 one;
        static const Vector3 up;
        static const Vector3 down;
        static const Vector3 left;
        static const Vector3 right;
        static const Vector3 forward;
        static const Vector3 back;
    };

    using Point = Vector3;
    using Color = Vector3;
}

namespace std
{
    template <>
    struct hash<math::Vector3>
    {
        size_t operator()(math::Vector3 const &vector3) const
        {
            return ((hash<double>()(vector3.x) ^
                     (hash<double>()(vector3.y) << 1)) >>
                    1) ^
                   (hash<double>()(vector3.z) << 1);
        }
    };
}
#endif