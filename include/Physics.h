#include "Point.h"

#include <fstream>
#include "json.hpp"

class Physics
{
private:
    const char * path = "../params.json";

    float INITIAL_T;  // initial tempurature
    float INITIAL_n;  // initial humidity

    float FREEZE_P0;  // base probability of freezing
    float MELT_P0;    // base probability of melting
    float ICE_N;      // density of ice in relation to water vapor density

    float VAPOR_D0;   // base of diffusion coefficient for vapor
    float VAPOR_K0;   // base of thermal conductivity coefficient for vapor
    float TRANS_K0;   // base of thermal conductivity coefficient for vapor-ice surface
    float ICE_K0;     // base of thermal conductivity coefficient for ice

    float VAPOR_C0;   // base of heat capacity for vapor
    float ICE_C0;     // base of heat capacuty for ice

    Physics() {
        std::ifstream f(path);
        nlohmann::json params = nlohmann::json::parse(f);

        INITIAL_T = params["INITIAL_T"];
        INITIAL_n = params["INITIAL_n"];

        FREEZE_P0 = params["FREEZE_P0"];
        MELT_P0 = params["MELT_P0"];
        ICE_N = params["ICE_N"];

        VAPOR_D0 = params["VAPOR_D0"];
        VAPOR_K0 = params["VAPOR_K0"];
        TRANS_K0 = params["TRANS_K0"];
        ICE_K0 = params["ICE_K0"];

        VAPOR_C0 = params["VAPOR_C0"];
        ICE_C0 = params["ICE_C0"];
    }

    static Physics* This() {
        static Physics inst;
        return &inst;
    }

public:
    // initial tempurature
    static float T() { return This()->INITIAL_T; }
    // initial humidity
    static float n() { return This()->INITIAL_n; }

    // local probability of freezing while neighbouring one freezed point
    static float FreezeP(Point* p) {
        return This()->FREEZE_P0;
    }
    // local probability of melting
    static float MeltP(Point* p) {
        return This()->MELT_P0;
    }
    // density of ice in relation to water vapor density
    static float IceN() {
        return This()->ICE_N;
    }

    // local flow of concentration due to diffusion from p1 to p2
    static float NFlow(Point* p1, Point* p2) {
        if (p1->IsFreezed() or p2->IsFreezed())
            return 0;
        else
            return -(This()->VAPOR_D0)*(p2->n - p1->n);
    }
    // local heat flow due to thermal conductivity from p1 to p2
    static float HFlow(Point* p1, Point* p2) {
        if (p1->IsFreezed() and p2->IsFreezed())
            return -(This()->ICE_K0)*(p2->T - p1->T);
        else if (!p1->IsFreezed() and !p2->IsFreezed())
            return -(This()->VAPOR_K0)*(p2->T - p1->T);
        else 
            return -(This()->TRANS_K0)*(p2->T - p1->T);
    }

    // heat point, chaning its tempurature
    static float Heat(Point* p, float heat) {
        if (p->IsFreezed())
            p->T += This()->ICE_C0 * heat;
        else 
            p->T += This()->VAPOR_C0 * p->n * heat;
    }
};