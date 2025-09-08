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

int main()
{

	//**************DECLARATIONS & DEFINITIONS***************//

	program_stage_t program_stage = INTRODUCTION;

	monitor_t monitor;

	visual_sim_type_t simVisualType = SIM_STANDBY;
	logical_sim_type_t simLogicalType = LOGIC_STANDBY;

	int fps = monitor.refresh_rate; // Frames per second

	View *view = constructView(&fps, &monitor);

	float timeMultiplier = 10 * SECONDS_PER_DAY; // Simulation speed: 10 days per simulation second
	float timeStep = 1 * timeMultiplier / fps;

	unsigned char gradient = 255;
	unsigned char text_gradient = 0;
	unsigned char text_gradient_2 = 0;

	long long timer_1;

	bool wait_for_text = 0;
	bool toggle_ship = 0;

	float blur_gradient = MAX_GRADIENT - 20;

	const char *view_options[2] = {"Planets Mode", "Pepsi Mode"};
	const char *math_options[2] = {"Gravity Mode", "Spring Mode"};
	const char *ship_options[2] = {"No", "Yes"};

	int subSteps;

	float &monitorwidth = monitor.width;
	float &monitorheight = monitor.height;

	//*******************************************************//

	//************************STARTUP************************//

	OrbitalSim *sim = constructOrbitalSim(timeStep);

	InitAudioDevice();

	HideCursor();

	resource_t *Master_resource = intro(&simVisualType, &simLogicalType, view, &monitor);

	//*******************************************************//

	//***********************PRE-LOOP************************//

	while (GetKeyPressed())
		;

	setup_3D_view(view);

	float x = monitorwidth * 0.7f;
	float y1 = monitorheight * 0.4f;
	float y2 = monitorheight * 0.5f;
	float y3 = monitorheight * 0.6f;
	float w = monitorwidth * 0.3f;
	float h = monitorheight * 0.05f;

	//*******************************************************//

	//*********************MAIN-LOOP*************************//

	while (isViewRendering(view))
	{

		switch (program_stage)
		{

			//********TITLE-SCREEN*********//

		case INTRODUCTION:
			subSteps = 5;
			for (int i = 0; i < subSteps; i++)
				updateOrbitalSim(sim, simLogicalType); // Updates multiple times per frame.

			renderView(view, sim, Master_resource, simVisualType, 0, toggle_ship);

			update_blur_shader(Master_resource, &monitor, blur_gradient);

			if (blur_gradient > 5)
				blur_gradient -= 3 * blur_gradient / (MAX_GRADIENT - blur_gradient);

			BeginDrawing_with_blurry_filter(Master_resource);

			if (fading_black_wall(&monitor))
			{
				if (text_gradient < 255)
				{
					text_gradient += 5;
				}
				else if (!wait_for_text && text_gradient == 255)
				{
					timer_1 = timestamp_millis();
					wait_for_text = 1;
				}
				else if (wait_for_text && text_gradient == 255 && text_gradient_2 < 255 && timestamp_millis() - timer_1 >= 2000)
				{
					text_gradient_2 += 5;
				}

				DrawTextEx(Master_resource->Font_Golden, "Planetarium",
						   (Vector2){monitorwidth * 0.5f, monitorheight * 0.3f} - MeasureTextEx(Master_resource->Font_Golden, "Planetarium", 128, 0.0) * 0.5,
						   128, 0.0, (Color){255, 203, 0, text_gradient});

				DrawTextEx(Master_resource->Font_Gothic,
						   "Press any key to continue",
						   (Vector2){monitorwidth * 0.5f, monitorheight * 0.7f} - MeasureTextEx(Master_resource->Font_Gothic, "Press any key to continue", 48, 0.0) * 0.5,
						   48, 0.0, (Color){255, 255, 255, text_gradient_2});
			}

			DrawFPS(0, 0);

			EndDrawing();

			if (text_gradient_2 == 255 && GetKeyPressed())
			{
				program_stage = SETTING_MENU;
			}

			break;

			//*****************************//

			//**********SETTINGS***********//

		case SETTING_MENU:

			ShowCursor();

			subSteps = 5;
			for (int i = 0; i < subSteps; i++)
				updateOrbitalSim(sim, simLogicalType); // Updates multiple times per frame.

			renderView(view, sim, Master_resource, simVisualType, 0, toggle_ship);

			BeginDrawing_with_blurry_filter(Master_resource);

			//-----Buttons display------//

			// Physics mode button

			DrawTextEx(Master_resource->Font_Gothic, "Physics Mode", (Vector2){monitorwidth * 0.75F, monitorheight * 0.35F}, 48, 0.0, WHITE);

			if (isMouseHere(GetMousePosition(), (Vector2){x, y1}, (Vector2){x + w, y1 + h}))
			{
				DrawRectangleGradientH(x, y1, w, h, BLACK, PURPLE);
				DrawTextEx(Master_resource->Font_Gothic, math_options[simLogicalType], (Vector2){x + w * 0.5F, y1}, 48, 0.0, WHITE);
			}
			else
			{
				DrawRectangleGradientH(x, y1, w, h, BLACK, DARKBLUE);
				DrawTextEx(Master_resource->Font_Gothic, math_options[simLogicalType], (Vector2){x + w * 0.5F, y1}, 48, 0.0, BLACK);
			}

			// View mode button

			DrawTextEx(Master_resource->Font_Gothic, "View Mode", (Vector2){monitorwidth * 0.75F, monitorheight * 0.45F}, 48, 0.0, WHITE);

			if (isMouseHere(GetMousePosition(), (Vector2){x, y2}, (Vector2){x + w, y2 + h}))
			{
				DrawRectangleGradientH(x, y2, w, h, BLACK, PURPLE);
				DrawTextEx(Master_resource->Font_Gothic, view_options[simVisualType], (Vector2){x + w * 0.5F, y2}, 48, 0.0, WHITE);
			}
			else
			{
				DrawRectangleGradientH(x, y2, w, h, BLACK, DARKBLUE);
				DrawTextEx(Master_resource->Font_Gothic, view_options[simVisualType], (Vector2){x + w * 0.5F, y2}, 48, 0.0, BLACK);
			}

			// Spaceship toggle button

			DrawTextEx(Master_resource->Font_Gothic, "Call SpaceShip", (Vector2){monitorwidth * 0.75F, monitorheight * 0.55F}, 48, 0.0, WHITE);

			if (isMouseHere(GetMousePosition(), (Vector2){x, y3}, (Vector2){x + w, y3 + h}))
			{
				DrawRectangleGradientH(x, y3, w, h, BLACK, PURPLE);
				DrawTextEx(Master_resource->Font_Gothic, ship_options[toggle_ship], (Vector2){x + w * 0.5F, y3}, 48, 0.0, WHITE);
			}
			else
			{
				DrawRectangleGradientH(x, y3, w, h, BLACK, DARKBLUE);
				DrawTextEx(Master_resource->Font_Gothic, ship_options[toggle_ship], (Vector2){x + w * 0.5F, y3}, 48, 0.0, BLACK);
			}

			//--------------------------//

			//----Mouse interaction-----//

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (isMouseHere(GetMousePosition(), (Vector2){x, y1}, (Vector2){x + w, y1 + h}))
				{
					if (simLogicalType == GRAVITATIONAL_SIMULATION)
						simLogicalType = SPRINGS_SIMULATION;
					else
						simLogicalType = GRAVITATIONAL_SIMULATION;
				}
				else if (isMouseHere(GetMousePosition(), (Vector2){x, y2}, (Vector2){x + w, y2 + h}))
				{
					if (simVisualType == PLANETS_SIMULATION)
						simVisualType = PEPSI_SIMULATION;
					else
						simVisualType = PLANETS_SIMULATION;
				}
				else if (isMouseHere(GetMousePosition(), (Vector2){x, y3}, (Vector2){x + w, y3 + h}))
				{
					toggle_ship = !toggle_ship;
				}
			}

			//--------------------------//

			if (IsKeyPressed(KEY_ENTER))
			{
				program_stage = FREEVIEW;
			}

			DrawFPS(0, 0);

			DrawTextEx(Master_resource->Font_Gothic, "[Enter] for freeview",
					   (Vector2){monitor.width * 0.3F, monitor.height * 0.9F}, 48, 0.0, WHITE);

			DrawTextEx(Master_resource->Font_Gothic, "[ESC] to exit Planetarium",
					   (Vector2){monitor.width * 0.6F, monitor.height * 0.9F}, 48, 0.0, WHITE);

			EndDrawing();

			break;

			//*****************************//

			//*********FREEVIEW************//

		case FREEVIEW:
			HideCursor();
			subSteps = 5;
			for (int i = 0; i < subSteps; i++)
				updateOrbitalSim(sim, simLogicalType); // Updates multiple times per frame.

			BeginDrawing_without_blurry_filter(Master_resource);

			renderView(view, sim, Master_resource, simVisualType, 1, toggle_ship);

			DrawText(getISODate(sim->totalTime), 0, 25, 20, RED);

			DrawFPS(0, 0);

			if (IsKeyPressed(KEY_BACKSPACE))
			{
				program_stage = SETTING_MENU;
			}

			DrawTextEx(Master_resource->Font_Gothic, "[Backspace] to display settings",
					   (Vector2){monitor.width * 0.3F, monitor.height * 0.9F}, 48, 0.0, WHITE);

			DrawTextEx(Master_resource->Font_Gothic, "[ESC] to exit Planetarium",
					   (Vector2){monitor.width * 0.6F, monitor.height * 0.9F}, 48, 0.0, WHITE);

			EndDrawing();

			break;

			//*****************************//

		default:
			break;
		}
	}

	//*******************************************************//

	//************************CLEANUP************************//

	destroyView(view);
	destroyOrbitalSim(sim);
	CloseAudioDevice();

	kill_resources(Master_resource);

	return 0;

	//*******************************************************//
}