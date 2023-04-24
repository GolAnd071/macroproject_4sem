#include "Simulation.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
    auto app = new Simulation(argc, argv);

    try {
        app->Run();
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }

    delete app;

    return 0;
}
