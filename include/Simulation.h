#pragma once

#include "Mesh.h"
#include "Point.h"

#include <map>
#include <queue>
#include <random>

class Simulation
{
public:
	Simulation(int argc, char** argv);
	~Simulation() = default;

	void Run();

private:
	void InitMesh();

	void FreezeMelt();

	void FulfillConcentration();

	void CalculateFlows();

	/// @brief updates n and T for each point
	/// @return true if there are enough vapor (aka sum(n) > 0)
	bool Update();

	void ClearVariables();

private:
	Mesh* m_Mesh;

	std::mt19937 m_Gen;
	std::uniform_real_distribution<> m_Dist;

	int m_IterNum;
	int m_Snapshot_freq = 1;

	// std::queue<Point*> m_HasLoan;

	std::map<Point*, float> m_DeltaN, m_DeltaH;
};