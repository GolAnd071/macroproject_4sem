#include "Point.h"

#include <cmath>

bool Point::IsFreezed()
{
    return m_freezed;
}

void Point::Freeze()
{
    m_freezed = true;
}

void Point::Melt()
{
    if (!m_meltable)
        m_freezed = false;
}

void Point::Seed()
{
    m_meltable = true;
    m_freezed = true;
    n = 0;
}

float Point::Dist(Point* other)
{
    float dx = this->x - other->x;
    float dy = this->y - other->y;
    float dz = this->z - other->z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
