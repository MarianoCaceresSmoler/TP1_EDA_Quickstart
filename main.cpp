/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "configuration.h"
#include "orbitalSim.h"
#include "view.h"
#include "menu.h"

#define SECONDS_PER_DAY 86400

int main() {
  visual_sim_type_t simVisualType = SIM_STANDBY;
  logical_sim_type_t simLogicalType = LOGIC_STANDBY;

  int fps = 60;                                // Frames per second
  float timeMultiplier = 10 * SECONDS_PER_DAY; // Simulation speed: 10 days per simulation second
  float timeStep = timeMultiplier / fps;

  OrbitalSim *sim = constructOrbitalSim(timeStep);
  View *view = constructView(fps);

  InitAudioDevice();

  menu (&simVisualType, &simLogicalType, view);

  setup_3D_view(view);

  int subSteps = 10;

  while ( isViewRendering(view) ) {
    for ( int i = 0; i < subSteps; i++ ) // multiple updates per frame
      updateOrbitalSim(sim, simLogicalType);

    renderView(view, sim, simVisualType);
  }

  destroyView(view);
  destroyOrbitalSim(sim);
  CloseAudioDevice();

  return 0;
}
