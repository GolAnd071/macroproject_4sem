#include "Point.h"

class Params
{
private:
    const char* path = "../params.json";

    float m_initial_T;  // initial tempurature
    float m_initial_n;  // initial humidity

    float m_freeze_P0;  // base probability of freezing
    float m_melt_P0;    // base probability of melting
    float m_ice_N;      // density of ice in relation to water vapor density

    float m_vapor_D0;   // base of diffusion coefficient for vapor
    float m_vapor_K0;   // base of thermal conductivity coefficient for vapor
    float m_trans_K0;   // base of thermal conductivity coefficient for vapor-ice surface
    float m_ice_K0;     // base of thermal conductivity coefficient for ice

    float m_vapor_C0;   // base of heat capacity for vapor
    float m_ice_C0;     // base of heat capacuty for ice

    Params();

    static Params* This();

public:
    // initial tempurature
    static float T();
    // initial humidity
    static float n();

    // local probability of freezing while neighbouring one freezed point
    static float FreezeP(Point* p);
    // local probability of melting
    static float MeltP(Point* p);
    // density of ice in relation to water vapor density
    static float IceN();

    // local flow of concentration due to diffusion from p1 to p2
    static float NFlow(Point* p1, Point* p2);
    // local heat flow due to thermal conductivity from p1 to p2
    static float HFlow(Point* p1, Point* p2);

    // heat point, chaning its tempurature
    static void Heat(Point* p, float heat);
};