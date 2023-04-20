#pragma once

class Point
{
public:
    Point(float x, float y, float z, float n, float T) : x(x), y(y), z(z), n(n), T(T) {}

    bool IsFreezed();

    void Freeze();

    void Melt();

    void Seed();

    float Dist(Point* other);

public:
    float x, y, z; // coordinates

    float n, T; // physical params

private:
    // state params
    // "meltable" control what is represented: water particle or heterogeneous crystallization center (seed)
    // to prevent seed from being melt "meltable" and "freezed" are incapsulated with methods below
    bool m_freezed = false, m_meltable = false;
};
