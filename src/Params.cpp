#include "Params.h"

#include "json/json.hpp"

#include <fstream>

Params::Params()
{
    std::ifstream f(path);
    nlohmann::json params = nlohmann::json::parse(f);

    m_initial_T = params["INITIAL_T"];
    m_initial_n = params["INITIAL_n"];

    m_freeze_P0 = params["FREEZE_P0"];
    m_melt_P0 = params["MELT_P0"];
    m_ice_N = params["ICE_N"];

    m_vapor_D0 = params["VAPOR_D0"];
    m_vapor_K0 = params["VAPOR_K0"];
    m_trans_K0 = params["TRANS_K0"];
    m_ice_K0 = params["ICE_K0"];

    m_vapor_C0 = params["VAPOR_C0"];
    m_ice_C0 = params["ICE_C0"];
}

Params* Params::This()
{
    static Params inst;
    return &inst;
}

float Params::T()
{
    return This()->m_initial_T;
}

float Params::n()
{
    return This()->m_initial_n;
}

float Params::FreezeP(Point* p)
{
    return This()->m_freeze_P0;
}

float Params::MeltP(Point* p)
{
    return This()->m_melt_P0;
}

float Params::IceN() {
    return This()->m_ice_N;
}

float Params::NFlow(Point* p1, Point* p2)
{
    if (p1->IsFreezed() || p2->IsFreezed())
        return 0;
    else
        return -(This()->m_vapor_D0) * (p2->n - p1->n);
}

float Params::HFlow(Point* p1, Point* p2)
{
    if (p1->IsFreezed() && p2->IsFreezed())
        return -(This()->m_ice_K0) * (p2->T - p1->T);
    else if (!p1->IsFreezed() && !p2->IsFreezed())
        return -(This()->m_vapor_K0) * (p2->T - p1->T);
    else
        return -(This()->m_trans_K0) * (p2->T - p1->T);
}

void Params::Heat(Point* p, float heat) {
    if (p->IsFreezed())
        p->T += This()->m_ice_C0 * heat;
    else
        p->T += This()->m_vapor_C0 * p->n * heat;
}