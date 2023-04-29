#include "Simulation.h"

#include "Params.h"
#include "Utility.h"

#include <algorithm>
#include <unordered_set>
#include <vector>

Simulation::Simulation(int argc, char** argv) : m_IterNum(0)
{
#ifdef ENABLE_PROFILING
	std::cout << "Started Simulation initialization.\n";
#endif

	InitMesh();

	std::random_device rd;
	m_Gen = std::mt19937(rd());
	m_Dist = std::uniform_real_distribution<>(0, 1);

	if (argc > 1)
		m_IterNum = atoi(argv[1]);

#ifdef ENABLE_PROFILING
	std::cout << "Initialized Simulation with " << m_IterNum << " iterations.\n\n";
#endif
}

void Simulation::Run()
{
#ifdef ENABLE_PROFILING
	std::cout << "\nSimulation started.\n";

	if (m_IterNum == 0)
		std::cout << "No iterations for simulation, only snapshot Mesh.\n";
#endif

	Utility::Snapshot(0, m_Mesh);

#ifdef ENABLE_PROFILING
	std::cout << "Snapshoted Mesh.\n";
#endif

	for (int iter = 1; iter <= m_IterNum; ++iter) {
	#ifdef ENABLE_PROFILING
		std::cout << "\nIteration " << iter << " started.\n";
	#endif

		FreezeMelt();

		//FulfillConcentration();

		CalculateFlows();

		Update();

		ClearVariables();

		Utility::Snapshot(iter, m_Mesh);

	#ifdef ENABLE_PROFILING
		std::cout << "Snapshoted iteration " << iter << ".\n";
	#endif
	#if defined(ENABLE_PROFILING) || defined(ENABLE_PERCENTAGE_OUTPUT)
		std::cout << "Iteration " << iter << " ended.\n";
	#endif
	}

#ifdef ENABLE_PROFILING
	std::cout << "Simulation ended.\n";
#endif
}

void Simulation::InitMesh()
{
#ifdef ENABLE_PROFILING
	std::cout << "Started Mesh construction.\n";
#endif

	m_Mesh = new Mesh(5, 5, Params::n(), Params::T());

	// TODO: Make freezed point closest to (0, 0, 0)
	auto freeze = m_Mesh->points.at(m_Mesh->size / 2);
	m_Mesh->points.at(m_Mesh->size / 3)->Seed();

#ifdef ENABLE_PROFILING
	std::cout << "Constructed Mesh, freezed point (" <<
		freeze->x << ", " << freeze->y << ", " << freeze->z << ").\n";
#endif
}

void Simulation::FreezeMelt()
{
#ifdef ENABLE_PROFILING
	std::cout << "Started mesh traversal to freeze and melt.\n";
#endif

	std::vector<Point*> pointsToFreeze, pointsToMelt;

	for (Point* point : m_Mesh->points) {
		float prob = m_Dist(m_Gen);

		if (point->IsFreezed()) {
			int countVapor = 0;

			for (auto neighbor : m_Mesh->neighbors[point])
				if (!neighbor->IsFreezed())
					++countVapor;

			if (prob < Params::MeltP(point) && countVapor > 0)
				pointsToMelt.push_back(point);

		}
		else {
			int countIce = 0;

			for (auto neighbor : m_Mesh->neighbors[point])
				if (neighbor->IsFreezed())
					++countIce;

			if (prob < Params::FreezeP(point) * countIce)
				pointsToFreeze.push_back(point);
		}
	}

	for (auto point : pointsToFreeze) {
		std::vector<Point*> neighborsToPump;

		// only vapor points that wont be freezed but will be neighbors to freezed
		for (auto neighbor : m_Mesh->neighbors[point]) {
			if (std::find(pointsToMelt.begin(), pointsToMelt.end(), neighbor) != pointsToMelt.end()
				or (!neighbor->IsFreezed() and 
				std::find(pointsToFreeze.begin(), pointsToFreeze.end(), neighbor) == pointsToFreeze.end())) {
					
				neighborsToPump.push_back(neighbor);
			}
		}

		for (auto neighbor : neighborsToPump)
			neighbor->n -= (Params::IceN() - point->n) / neighborsToPump.size();
	}

	for (auto point : pointsToFreeze) {
		point->Freeze();
		point->n = 0;
	}

	for (auto point : pointsToMelt) {
		point->Melt();
		point->n = Params::IceN();
	}

#ifdef ENABLE_PROFILING
	std::cout << "Ended mesh traversal to freeze and melt.\n";
#endif
}

void Simulation::FulfillConcentration()
{
#ifdef ENABLE_PROFILING
	std::cout << "Started mesh traversal to fulfill concentration loan after freezing.\n";
#endif

	// code has been changed and moved to Simlutation::FreezeMelt()

#ifdef ENABLE_PROFILING
	std::cout << "Ended mesh traversal to fulfill concentration loan after freezing.\n";
#endif
}

void Simulation::CalculateFlows()
{
#ifdef ENABLE_PROFILING
	std::cout << "Started mesh traversal to calculate concentration and temperature flow for each edge.\n";
#endif

	for (Point* point : m_Mesh->points) {
		float dN = 0, dH = 0;

		for (Point* neighbor : m_Mesh->neighbors[point]) {
			dN += Params::NFlow(neighbor, point);
			dH += Params::HFlow(neighbor, point);
		}

		m_DeltaN[point] = dN;
		m_DeltaH[point] = dH;
	}

#ifdef ENABLE_PROFILING
	std::cout << "Ended mesh traversal to calculate concentration and temperature flow for each edge.\n";
#endif
}

void Simulation::Update()
{
#ifdef ENABLE_PROFILING
	std::cout << "Started mesh traversal to update concentration and temperature.\n";
#endif

	for (Point* point : m_Mesh->points) {
		Params::Diffuse(point, m_DeltaN[point]);
		Params::Heat(point, m_DeltaH[point]);
	}

#ifdef ENABLE_PROFILING
	std::cout << "Ended mesh traversal to update concentration and temperature.\n";
#endif
}

void Simulation::ClearVariables()
{
	m_DeltaH.clear();
	m_DeltaN.clear();
}
