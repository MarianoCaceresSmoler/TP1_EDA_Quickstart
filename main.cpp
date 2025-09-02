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
#define MAX_GRADIENT 255

int main() {

  monitor_t monitor;

  visual_sim_type_t simVisualType = SIM_STANDBY;
  logical_sim_type_t simLogicalType = LOGIC_STANDBY;

  int fps = monitor.refresh_rate; // Frames per second

  bool temp_skip = 0;

  unsigned char gradient = 255;

  View *view = constructView(&fps, &monitor);

  float timeMultiplier = 10 * SECONDS_PER_DAY; // Simulation speed: 10 days per simulation second
  float timeStep = timeMultiplier / fps;

  float blur_gradient = MAX_GRADIENT - 20;

  OrbitalSim *sim = constructOrbitalSim(timeStep);

  InitAudioDevice();

  resource_t *Master_resource = intro(&simVisualType, &simLogicalType, view, &monitor);

  //   float blurStrength = 1.0f;
  //   SetShaderValue(Master_resource->Shader_blur, Master_resource->Shader_blur_intensity_location, &blurStrength, SHADER_UNIFORM_FLOAT);

  setup_3D_view(view);

  int subSteps = 1;

  while ( isViewRendering(view) )
  {
    for ( int i = 0; i < subSteps; i++ ) // multiple updates per frame
      updateOrbitalSim(sim, simLogicalType);

    update_blur_shader(Master_resource, &monitor, blur_gradient);

    if ( blur_gradient > 5 ) blur_gradient -= 2 * blur_gradient / (MAX_GRADIENT - blur_gradient);

    renderView(view, sim, Master_resource, simVisualType);

    //     if (temp_skip) BeginDrawing_with_blurry_filter(Master_resource);
    //     else
    BeginDrawing_without_blurry_filter(Master_resource);

    if ( GetKeyPressed() ) temp_skip = 1;

    //     DrawFPS(0, 0);
    //     DrawText(getISODate(sim->totalTime), 0, 25, 20, RED);

    if ( fading_black_wall(&monitor) )
    {
      DrawTextEx(Master_resource->Font_Golden, "Space Invaders", (Vector2) {monitor.width * 0.5f, monitor.height * 0.3f} - MeasureTextEx(Master_resource->Font_Golden, "Space Invaders", 128, 0.0) * 0.5, 128, 0.0, GOLD);
    }

    EndDrawing();
  }

  destroyView(view);
  destroyOrbitalSim(sim);
  CloseAudioDevice();

  std ::cout << monitor.current << "\t" << monitor.height << "\t" << monitor.refresh_rate << "\t" << monitor.width;

  kill_resources(Master_resource);

  return 0;
}
