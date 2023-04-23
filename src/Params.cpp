#include "Params.h"

#include "json/json.hpp"

#include <fstream>
#include <cmath>

Params::Params()
{
    std::ifstream f(m_path);
    nlohmann::json params = nlohmann::json::parse(f);
    
    //simulation parameters
    m_initial_T = params["INITIAL_T"];
    m_initial_n = params["INITIAL_n"];

    m_freeze_P0 = params["FREEZE_P0"];
    m_melt_P0 = params["MELT_P0"];

    m_vapor_D0 = params["VAPOR_D0"];
    m_vapor_K0 = params["VAPOR_K0"];
    m_trans_K0 = params["TRANS_K0"];
    m_ice_K0 = params["ICE_K0"];

    m_vapor_C0 = params["VAPOR_C0"];
    m_ice_C0 = params["ICE_C0"];

    m_cell_side_length = params["CELL_SIDE_LENGTH"];
    m_cell_area_side = sqrt(3) * std::pow(m_cell_side_length, 2);
    m_cell_area_top = (3 * sqrt(3)) / 4 * std::pow(m_cell_side_length, 2);
    m_cell_volume = (3 * sqrt(3)) / 4 * std::pow(m_cell_side_length, 3);

    m_ice_N = params["ICE_N"];

    m_terrace_step_energy = params["TERRACE_STEP_ENERGY"];

    m_k_boltzmann = params["K_BOLTZMANN"];
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

    float prob;
    /*prob = (p->n) / (This()->m_ice_N) * (This()->m_cell_area_side * This()->m_terrace_step_energy * This()->m_terrace_step_energy) 
    / (std::pow(This()->m_k_boltzmann, 2) * std::pow(p->T, 2));*/
    prob = This()->m_freeze_P0 * std::pow(This()->m_terrace_step_energy, 2) / std::pow(p->T, 2);
    return prob;
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

void Params::Diffuse(Point* p, float conc) {
    p->n += conc;
}