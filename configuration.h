/**
 * @EDA TP1 - Warm Up
 * @brief Constants, defines and types for orbital simulation
 * @author Mariano Caceres Smoler
 * @author Enzo Nicolas Rosa Fernandez
 * @author Francisco Chiusaroli
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <raylib.h>
#include <vector>



//Constant definitions and macros
#define NORM(x, y, z) (sqrt(((x) * (x)) + ((y) * (y)) + ((z) * (z))))

// Macros for resources locations
#define ASSETS_SOURCE(x) "./Assets/" x
#define MODELS_LOCATE(model_file) ASSETS_SOURCE("Models/" model_file)
#define FONTS_LOCATE(font_file) ASSETS_SOURCE("Fonts/" font_file)
#define AUDIO_LOCATE(audio_file) ASSETS_SOURCE("Audio/" audio_file)
#define SHADER_LOCATE(shader_file) ASSETS_SOURCE("Shaders/" shader_file)

#define TYPE_COUNTER 3	   // Times that the typewriter audio is loaded
#define GRID_SLOTS_COUNT 8 // Number of slots in the visual grid

// Enums/Ids for the visual and logical state of the simulations
enum visual_sim_type_t
{
	SIM_STANDBY = -1,
	PLANETS_SIMULATION,
	PEPSI_SIMULATION
};

enum logical_sim_type_t
{
	LOGIC_STANDBY = -1,
	GRAVITATIONAL_SIMULATION,
	SPRINGS_SIMULATION
};

// General states of the program
enum program_stage_t
{
	INTRODUCTION,
	SETTING_MENU,
	FREEVIEW
};

// Struct to save information of the monitor
typedef struct
{
	float current;
	float width;
	float height;
	float refresh_rate;
} monitor_t;

// Struct to save all the resources loaded
typedef struct
{
	// Fonts
	Font Font_Gothic;
	Font Font_Golden;
	Font Font_Typerwriter;

	// Sounds
	Sound Typewriter_forward[TYPE_COUNTER];
	Sound Typewriter_backward[TYPE_COUNTER];

	// 3D models
	Model Model_PepsiCan;
	Model Model_SpaceShip;
	Model Model_Earth;
	Model Model_Jupiter;
	Model Model_Mars;
	Model Model_Mercury;
	Model Model_Neptune;
	Model Model_Saturn;
	Model Model_Sun;
	Model Model_Uranus;
	Model Model_Venus;

	Model Models_Asteroids[4];

	std::vector<Model *> Models_Solar_System;

	// Shaders
	Shader Shader_blur_h;
	int Shader_blur_h_intensity_location;
	int Shader_blur_h_renderWidth;
	int Shader_blur_h_renderHeight;

	Shader Shader_blur_v;
	int Shader_blur_v_intensity_location;
	int Shader_blur_v_renderWidth;
	int Shader_blur_v_renderHeight;

	// Textures
	RenderTexture2D Texture_Buffer1;
	RenderTexture2D Texture_Buffer2;

} resource_t;

#endif
