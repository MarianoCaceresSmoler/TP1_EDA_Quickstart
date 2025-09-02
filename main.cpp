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

  resource_t *Master_resource = intro(&simVisualType, &simLogicalType, view, &monitor);

//   float blurStrength = 1.0f;
//   SetShaderValue(Master_resource->Shader_blur, Master_resource->Shader_blur_intensity_location, &blurStrength, SHADER_UNIFORM_FLOAT);

  setup_3D_view(view);

  int subSteps = 10;

  while ( isViewRendering(view) )
  {
    for ( int i = 0; i < subSteps; i++ ) // multiple updates per frame
      updateOrbitalSim(sim, simLogicalType);

    renderView(view, sim, Master_resource, simVisualType);

    BeginDrawing();

    ClearBackground(BLACK);
//     BeginShaderMode(Master_resource->Shader_blur);
     DrawTextureRec(Master_resource->Texture_Buffer.texture, (Rectangle) {0, 0, (float) Master_resource->Texture_Buffer.texture.width, (float) -Master_resource->Texture_Buffer.texture.height}, (Vector2) {0, 0}, WHITE);

//     EndShaderMode();
    EndDrawing();
  }

  destroyView(view);
  destroyOrbitalSim(sim);
  CloseAudioDevice();

  std ::cout << monitor.current << "\t" << monitor.height << "\t" << monitor.refresh_rate << "\t" << monitor.width;

  kill_resources(Master_resource);

  return 0;
}
