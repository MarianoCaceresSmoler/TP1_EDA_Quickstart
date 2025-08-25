/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "orbitalSim.h"
#include "view.h"

#define SECONDS_PER_DAY 86400

int main()
{
    int fps = 60;                                 // Frames per second
    float timeMultiplier = 10 * SECONDS_PER_DAY; // Simulation speed: 100 days per simulation second
    float timeStep = timeMultiplier / fps;

    OrbitalSim *sim = constructOrbitalSim(timeStep);
    View *view = constructView(fps);

    int substeps = 10;

    while (isViewRendering(view))
    {
        // updateOrbitalSim(sim);s
        for (int i = 0; i < substeps; i++) // multiple updates per frame
            updateOrbitalSim(sim);

        renderView(view, sim);
    }

    destroyView(view);
    destroyOrbitalSim(sim);

    return 0;
}
