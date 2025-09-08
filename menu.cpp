/**
 * @EDA TP1 - Warm Up
 * @brief Menu program source
 * @author Enzo Nicolas Rosa Fernandez
 * @author Mariano Caceres Smoler
 * @author Francisco Chiusaroli
 *
 */
#include "menu.h"

#include <chrono>
#include <stdio.h>

#define TOGGLE_LIMIT 6 // Maximum times the end symbol toggles during intro animation

// Declarations of static/private functions
static void intialize_resources(resource_t *Master_resource, monitor_t *monitor);
static void animation_intro(resource_t *Master_resource, monitor_t *monitor);

/**
 * @brief Entry point for the intro sequence. Initializes resources and plays intro animation.
 * @param simVisualType Pointer to visual_sim_type_t indicating the visual simulation type.
 * @param simLogicalType Pointer to logical_sim_type_t indicating the logical simulation type.
 * @param view Pointer to View object used for graphical interface (not used here, reserved for future).
 * @param monitor Pointer to monitor_t containing monitor resolution and refresh rate.
 * @return Pointer to a resource_t structure holding all loaded resources.
 */
resource_t *intro(visual_sim_type_t *simVisualType, logical_sim_type_t *simLogicalType, View *view, monitor_t *monitor)
{

	resource_t *Master_resource = new resource_t;

	intialize_resources(Master_resource, monitor);

	animation_intro(Master_resource, monitor);

	*simVisualType = PLANETS_SIMULATION;
	*simLogicalType = GRAVITATIONAL_SIMULATION;

	return Master_resource;
}

/**
 * @brief Loads all fonts, models, shaders, and audio required for the intro sequence and later stages.
 * @param Master_resource Pointer to resource_t structure where resources will be stored.
 * @param monitor Pointer to monitor_t for determining rendering dimensions.
 */
static void intialize_resources(resource_t *Master_resource, monitor_t *monitor)
{
	// Load fonts for different text styles
	Master_resource->Font_Gothic = LoadFontEx(FONTS_LOCATE("Gothic_regular.ttf"), 48, 0, 250);
	Master_resource->Font_Golden = LoadFontEx(FONTS_LOCATE("Golden.otf"), 128, 0, 250);
	Master_resource->Font_Typerwriter = LoadFontEx(FONTS_LOCATE("TypeWriter.ttf"), 64, 0, 250);

	// Load models (Pepsi can and UFO)
	Master_resource->Model_PepsiCan = LoadModel(MODELS_LOCATE("Pepsi_Basic/Pepsi_Can.obj")); // Demo model
	Master_resource->Model_SpaceShip = LoadModel(MODELS_LOCATE("UFO/Low_poly_UFO.obj"));
	Master_resource->Model_Sun = LoadModel(MODELS_LOCATE("Solar_System/sun.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Sun);
	Master_resource->Model_Mercury = LoadModel(MODELS_LOCATE("Solar_System/mercury.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Mercury);
	Master_resource->Model_Venus = LoadModel(MODELS_LOCATE("Solar_System/venus.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Venus);
	Master_resource->Model_Earth = LoadModel(MODELS_LOCATE("Solar_System/earth.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Earth);
	Master_resource->Model_Mars = LoadModel(MODELS_LOCATE("Solar_System/mars.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Mars);
	Master_resource->Model_Jupiter = LoadModel(MODELS_LOCATE("Solar_System/jupiter.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Jupiter);
	Master_resource->Model_Saturn = LoadModel(MODELS_LOCATE("Solar_System/saturn.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Saturn);
	Master_resource->Model_Uranus = LoadModel(MODELS_LOCATE("Solar_System/uranus.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Uranus);
	Master_resource->Model_Neptune = LoadModel(MODELS_LOCATE("Solar_System/neptune.obj"));
	Master_resource->Models_Solar_System.push_back(&Master_resource->Model_Neptune);

	Master_resource->Models_Asteroids[0] = LoadModel(MODELS_LOCATE("Solar_System/asteroid1.obj"));
	Master_resource->Models_Asteroids[1] = LoadModel(MODELS_LOCATE("Solar_System/asteroid2.obj"));
	Master_resource->Models_Asteroids[2] = LoadModel(MODELS_LOCATE("Solar_System/asteroid3.obj"));
	Master_resource->Models_Asteroids[3] = LoadModel(MODELS_LOCATE("Solar_System/asteroid4.obj"));

	// Load horizontal and vertical blur shaders
	Master_resource->Shader_blur_h = LoadShader(0, SHADER_LOCATE("Shader_Blur_h.fs"));
	Master_resource->Shader_blur_h_intensity_location = GetShaderLocation(Master_resource->Shader_blur_h, "blurStrength");
	Master_resource->Shader_blur_h_renderHeight = GetShaderLocation(Master_resource->Shader_blur_h, "renderHeight");
	Master_resource->Shader_blur_h_renderWidth = GetShaderLocation(Master_resource->Shader_blur_h, "renderWidth");

	Master_resource->Shader_blur_v = LoadShader(0, SHADER_LOCATE("Shader_Blur_v.fs"));
	Master_resource->Shader_blur_v_intensity_location = GetShaderLocation(Master_resource->Shader_blur_v, "blurStrength");
	Master_resource->Shader_blur_v_renderHeight = GetShaderLocation(Master_resource->Shader_blur_v, "renderHeight");
	Master_resource->Shader_blur_v_renderWidth = GetShaderLocation(Master_resource->Shader_blur_v, "renderWidth");

	update_blur_shader(Master_resource, monitor, 5.0);

	// Load typewriter sound effects
	for (int i = 0; i < TYPE_COUNTER; i++)
	{
		Master_resource->Typewriter_forward[i] = LoadSound(AUDIO_LOCATE("Typewriter_forward.wav"));
		Master_resource->Typewriter_backward[i] = LoadSound(AUDIO_LOCATE("Typewriter_backward.wav"));
	}

	// Create render textures for applying blur
	Master_resource->Texture_Buffer1 = LoadRenderTexture(monitor->width, monitor->height);
	Master_resource->Texture_Buffer2 = LoadRenderTexture(monitor->width, monitor->height);
}

/**
 * @brief Releases all previously loaded resources to prevent memory leaks.
 * @param Master_resource Pointer to resource_t structure to be cleaned up.
 */
void kill_resources(resource_t *Master_resource)
{

	// Unload fonts
	UnloadFont(Master_resource->Font_Gothic);
	UnloadFont(Master_resource->Font_Golden);
	UnloadFont(Master_resource->Font_Typerwriter);

	// Unload models
	UnloadModel(Master_resource->Model_PepsiCan);
	UnloadModel(Master_resource->Model_SpaceShip);
	UnloadModel(Master_resource->Model_Sun);
	UnloadModel(Master_resource->Model_Mercury);
	UnloadModel(Master_resource->Model_Venus);
	UnloadModel(Master_resource->Model_Earth);
	UnloadModel(Master_resource->Model_Mars);
	UnloadModel(Master_resource->Model_Jupiter);
	UnloadModel(Master_resource->Model_Saturn);
	UnloadModel(Master_resource->Model_Uranus);
	UnloadModel(Master_resource->Model_Neptune);

	for (int i = 0; i < 4; i++)
	{
		UnloadModel(Master_resource->Models_Asteroids[i]);
	}

	// Unload shaders
	UnloadShader(Master_resource->Shader_blur_h);
	UnloadShader(Master_resource->Shader_blur_v);

	// Unload render textures
	UnloadRenderTexture(Master_resource->Texture_Buffer1);
	UnloadRenderTexture(Master_resource->Texture_Buffer2);

	// Unload sounds
	for (int i = 0; i < TYPE_COUNTER; i++)
	{
		UnloadSound(Master_resource->Typewriter_forward[i]);
		UnloadSound(Master_resource->Typewriter_backward[i]);
	}

	delete Master_resource;
}

/**
 * @brief Returns the number of milliseconds since the program started, similar to Arduino's millis().
 * @return Current timestamp in milliseconds as a long long.
 */
long long timestamp_millis()
{
	auto now = std::chrono::steady_clock::now();
	auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	return ms.time_since_epoch().count();
}

/**
 * @brief Function to check if the mouse cursor is within set coordinates.
 * @param mouse Mouse cursor position in XY coords
 * @param minXY minimum XY values
 * @param maxXY maximum XY values
 * @return Returns whether the mouse cursor is within set coordinates.
 */
bool isMouseHere(Vector2 mouse, Vector2 minXY, Vector2 maxXY)
{
	return (mouse.x >= minXY.x && mouse.x < maxXY.x &&
			mouse.y >= minXY.y && mouse.y < maxXY.y);
}

/**
 * @brief Displays an intro animation with a typewriter effect for the text "Lionel Messi Studios present".
 * @param Master_resource Pointer to resource_t structure containing loaded fonts and sounds.
 * @param monitor Pointer to monitor_t for centering the text on screen.
 */
static void animation_intro(resource_t *Master_resource, monitor_t *monitor)
{

	long long timer_1, timer_2, timer_3;
	Vector2 text_position;

	short int buffer_length = 0;
	short int toggle_counter = 0;
	short int sound_counter = 0;

	char end_symbol = '|';
	char present_1[] = "Lionel Messi Studios present";
	char buffer[sizeof(present_1) + 1] = {0};

	bool finish = 0;

	timer_1 = timestamp_millis();
	timer_2 = timer_1;

	while (finish == 0)
	{
		BeginDrawing();
		ClearBackground(BLACK);

		timer_3 = timestamp_millis();

		// Type characters with blinking cursor
		if (toggle_counter < TOGGLE_LIMIT && buffer_length < sizeof(present_1) && timer_3 - timer_1 >= 100)
		{
			sprintf(buffer, "%.*s%c", buffer_length, present_1, end_symbol);
			timer_1 = timestamp_millis();
			buffer_length++;
			PlaySound(Master_resource->Typewriter_forward[sound_counter]);

			sound_counter = (sound_counter >= TYPE_COUNTER - 1) ? 0 : sound_counter + 1;
		}

		// Toggle cursor after full text appears
		else if (toggle_counter < TOGGLE_LIMIT && buffer_length >= sizeof(present_1) && timer_3 - timer_1 >= 500)
		{
			end_symbol = (end_symbol != 0) ? 0 : '|';
			sprintf(buffer, "%s%c", present_1, end_symbol);
			timer_1 = timestamp_millis();
			toggle_counter++;
		}

		// Erase text backwards
		else if (toggle_counter >= TOGGLE_LIMIT && buffer_length > 0 && timer_3 - timer_1 >= 100)
		{
			buffer_length--;
			sprintf(buffer, "%.*s%c", buffer_length, present_1, end_symbol);
			timer_1 = timestamp_millis();

			PlaySound(Master_resource->Typewriter_backward[sound_counter]);
			sound_counter = (sound_counter >= TYPE_COUNTER - 1) ? 0 : sound_counter + 1;

			if (buffer_length <= 0)
				buffer[0] = 0;
		}

		// Wait before finishing
		else if (toggle_counter >= TOGGLE_LIMIT && !buffer_length && timer_3 - timer_1 >= 2000)
		{
			finish = 1;
		}

		// Calculate text position only when first or last phase
		if (toggle_counter == 0 || toggle_counter >= TOGGLE_LIMIT)
			text_position = (Vector2){monitor->width * 0.5f, monitor->height * 0.5f} - MeasureTextEx(Master_resource->Font_Typerwriter, buffer, 64, 0.0) * 0.5;

		DrawTextEx(Master_resource->Font_Typerwriter, buffer, text_position, 64, 0.0, WHITE);

		EndDrawing();

		if (GetKeyPressed() != 0) // Skip intro if any key is pressed
			finish = 1;
	}
}

/**
 * @brief Begins a new drawing frame applying a two-pass blur effect using shaders.
 * @param Master_resource Pointer to resource_t containing shader programs and render textures.
 */
void BeginDrawing_with_blurry_filter(resource_t *Master_resource)
{
	// Render scene into Texture_Buffer2 applying horizontal blur
	BeginTextureMode(Master_resource->Texture_Buffer2);
	ClearBackground(BLACK);

	BeginShaderMode(Master_resource->Shader_blur_h);
	DrawTextureRec(Master_resource->Texture_Buffer1.texture,
				   (Rectangle){0, 0, (float)Master_resource->Texture_Buffer1.texture.width, (float)-Master_resource->Texture_Buffer1.texture.height},
				   (Vector2){0, 0}, WHITE);
	EndShaderMode();

	EndTextureMode();

	// Draw final result to the screen applying vertical blur
	BeginDrawing();
	ClearBackground(BLACK);

	BeginShaderMode(Master_resource->Shader_blur_v);
	DrawTextureRec(Master_resource->Texture_Buffer2.texture,
				   (Rectangle){0, 0, (float)Master_resource->Texture_Buffer2.texture.width, (float)-Master_resource->Texture_Buffer2.texture.height},
				   (Vector2){0, 0}, WHITE);
	EndShaderMode();
}

/**
 * @brief Begins a new drawing frame without applying blur, rendering the original scene.
 * @param Master_resource Pointer to resource_t containing Texture_Buffer1 to be displayed.
 */
void BeginDrawing_without_blurry_filter(resource_t *Master_resource)
{
	BeginDrawing();
	ClearBackground(BLACK);

	// Draw scene from Texture_Buffer1 without blur
	DrawTextureRec(Master_resource->Texture_Buffer1.texture,
				   (Rectangle){0, 0, (float)Master_resource->Texture_Buffer1.texture.width, (float)-Master_resource->Texture_Buffer1.texture.height},
				   (Vector2){0, 0}, WHITE);
}

/**
 * @brief Updates shader uniforms for horizontal and vertical blur shaders.
 * @param Master_resource Pointer to resource_t containing shader programs.
 * @param monitor Pointer to monitor_t providing render width and height for shader calculations.
 * @param factor Float value representing blur intensity.
 */
void update_blur_shader(resource_t *Master_resource, monitor_t *monitor, float factor)
{
	float blurStrength = factor;

	// Update horizontal blur shader values
	SetShaderValue(Master_resource->Shader_blur_h, Master_resource->Shader_blur_h_renderWidth, &monitor->width, SHADER_UNIFORM_FLOAT);
	SetShaderValue(Master_resource->Shader_blur_h, Master_resource->Shader_blur_h_renderHeight, &monitor->height, SHADER_UNIFORM_FLOAT);
	SetShaderValue(Master_resource->Shader_blur_h, Master_resource->Shader_blur_h_intensity_location, &blurStrength, SHADER_UNIFORM_FLOAT);

	// Update vertical blur shader values
	SetShaderValue(Master_resource->Shader_blur_v, Master_resource->Shader_blur_v_renderWidth, &monitor->width, SHADER_UNIFORM_FLOAT);
	SetShaderValue(Master_resource->Shader_blur_v, Master_resource->Shader_blur_v_renderHeight, &monitor->height, SHADER_UNIFORM_FLOAT);
	SetShaderValue(Master_resource->Shader_blur_v, Master_resource->Shader_blur_v_intensity_location, &blurStrength, SHADER_UNIFORM_FLOAT);
}

/**
 * @brief Draws a fading black overlay on the screen, gradually making the scene visible.
 * @param monitor Pointer to monitor_t providing screen dimensions.
 * @return true when the fade effect is complete (fully transparent), false otherwise.
 */
bool fading_black_wall(monitor_t *monitor)
{
	static unsigned char gradient = 255; // Alpha starts fully opaque

	// Draw black rectangle with decreasing transparency
	DrawRectangle(0, 0, monitor->width, monitor->height, {0, 0, 0, gradient});
	if (gradient > 0)
		gradient--;

	return !gradient;
}
