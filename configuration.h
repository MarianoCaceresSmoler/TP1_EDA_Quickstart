/**
 * @brief Constants, defines and types for orbital simulation
 * @author Mariano Caceres Smoler
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <raylib.h>

#define ASSETS_SOURCE(x) "./Assets/" x
#define MODELS_LOCATE(model_file) ASSETS_SOURCE("Models/" model_file)
#define FONTS_LOCATE(font_file) ASSETS_SOURCE("Fonts/" font_file)
#define AUDIO_LOCATE(audio_file) ASSETS_SOURCE("Audio/" audio_file)
#define SHADER_LOCATE(shader_file) ASSETS_SOURCE("Shaders/" shader_file)
#define TYPE_COUNTER 3

typedef enum { SIM_STANDBY,
               PLANETS_SIMULATION,
               PEPSI_SIMULATION } visual_sim_type_t;
typedef enum { LOGIC_STANDBY,
               GRAVITATIONAL_SIMULATION,
               SPRINGS_SIMULATION } logical_sim_type_t;

typedef struct {
  float current;
  float width;
  float height;
  float refresh_rate;
} monitor_t;

typedef struct {
  Font Font_Gothic;
  Font Font_Golden;
  Font Font_Typerwriter;

  Sound Typewriter_forward[TYPE_COUNTER];
  Sound Typewriter_backward[TYPE_COUNTER];

  Model Model_PepsiCan;
  Model Model_SpaceShip;

  Shader Shader_blur_h;
  int Shader_blur_h_intensity_location;
  int Shader_blur_h_renderWidth;
  int Shader_blur_h_renderHeight;

  Shader Shader_blur_v;
  int Shader_blur_v_intensity_location;
  int Shader_blur_v_renderWidth;
  int Shader_blur_v_renderHeight;

  RenderTexture2D Texture_Buffer;
} resource_t;

#endif
