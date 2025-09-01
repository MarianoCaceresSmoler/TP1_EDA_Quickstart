/**
 * @brief Orbital simulation main module
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include "configuration.h"
#include "menu.h"
#include "orbitalSim.h"
#include "view.h"
#include <iostream>

#define SECONDS_PER_DAY 86400

int main() {

  monitor_t monitor;

  visual_sim_type_t simVisualType = SIM_STANDBY;
  logical_sim_type_t simLogicalType = LOGIC_STANDBY;

  int fps = monitor.refresh_rate; // Frames per second

  View *view = constructView(&fps, &monitor);

  float timeMultiplier = 10 * SECONDS_PER_DAY; // Simulation speed: 10 days per simulation second
  float timeStep = timeMultiplier / fps;

  OrbitalSim *sim = constructOrbitalSim(timeStep);

  InitAudioDevice();

  menu(&simVisualType, &simLogicalType, view, &monitor);

  setup_3D_view(view);

  int subSteps = 10;

  while ( isViewRendering(view) )
  {
    for ( int i = 0; i < subSteps; i++ ) // multiple updates per frame
      updateOrbitalSim(sim, simLogicalType);

    renderView(view, sim, simVisualType);
  }

  destroyView(view);
  destroyOrbitalSim(sim);
  CloseAudioDevice();

  std ::cout << monitor.current << "\t" << monitor.height << "\t" << monitor.refresh_rate << "\t" << monitor.width;

  return 0;
}
