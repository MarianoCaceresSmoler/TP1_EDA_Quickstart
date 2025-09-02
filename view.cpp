/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include <iostream>
#include <string>
#include <time.h>

#include "configuration.h"
#include "view.h"

#define SETUP_WINDOW_WIDTH 640
#define SETUP_WINDOW_HEIGHT 480
#define CAMERA_SHORT_RANGE 5
#define CAMERA_MEDIUM_RANGE 150

static void renderStandardSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource);
static void renderPepsiSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource);

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *        to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
const char *getISODate(float timestamp) {
  // Timestamp epoch: 1/1/2022
  struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

  // Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
  time_t unixEpoch = mktime(&unichEpochTM);
  time_t unixTimestamp = unixEpoch + (time_t) timestamp;

  // Returns ISO date
  struct tm *localTM = localtime(&unixTimestamp);
  return TextFormat("%04d-%02d-%02d", 1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
}

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
View *constructView(int *fps, monitor_t *monitor) {
  View *view = new View();

  InitWindow(0, 0, "EDA Orbital Simulation");
  ToggleFullscreen();

  monitor->current = GetCurrentMonitor();
  monitor->height = GetMonitorHeight(monitor->current);
  monitor->refresh_rate = GetMonitorRefreshRate(monitor->current);
  monitor->width = GetMonitorWidth(monitor->current);

  *fps = monitor->refresh_rate;

	SetTargetFPS(*fps);

	return view;
}

void setup_3D_view(View *view) {
  DisableCursor();

  view->camera.position = {10.0f, 10.0f, 10.0f};
  view->camera.target = {0.0f, 0.0f, 0.0f};
  view->camera.up = {0.0f, 1.0f, 0.0f};
  view->camera.fovy = 45.0f;
  view->camera.projection = CAMERA_PERSPECTIVE;
}

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(View *view)
{
	CloseWindow();

  delete view;
}

/**
 * @brief Should the view still render?
 *
 * @return Should rendering continue?
 */
bool isViewRendering(View *view) {
  return !WindowShouldClose();
}

/**
 * Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */

void renderView(View *view, OrbitalSim *sim, resource_t *Master_resource, int simType) {
  if ( simType == PLANETS_SIMULATION )
  {
    renderStandardSimulation(view, sim, Master_resource);
  }
  else if ( simType == PEPSI_SIMULATION )
  {
    renderPepsiSimulation(view, sim, Master_resource);
  }
}

static void renderStandardSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource)
{
	UpdateCamera(&view->camera, CAMERA_FREE);

  BeginTextureMode(Master_resource->Texture_Buffer1);
  ClearBackground(BLACK);
  BeginMode3D(view->camera);

  static float rotation;

  for ( int i = 0; i < sim->bodyCount; i++ )
  {
    Vector3 scaledBodyPos = sim->bodiesList[i].position * 1E-11f;
    Vector3 cameraPos = view->camera.position;

    Vector3 diff = {
        scaledBodyPos.x - cameraPos.x,
        scaledBodyPos.y - cameraPos.y,
        scaledBodyPos.z - cameraPos.z};

    double dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

    if ( dist < CAMERA_SHORT_RANGE )
    {
      float scaledRadius = 0.005F * logf(sim->bodiesList[i].radius);
      int rings = 4;
      int slices = 5;
      DrawSphere(scaledBodyPos, 0.005F * logf(sim->bodiesList[i].radius), sim->bodiesList[i].color);
    }
    else if ( dist < CAMERA_MEDIUM_RANGE )
    {
      float scaledRadius = 0.005F * logf(sim->bodiesList[i].radius);
      int rings = 4;
      int slices = 5;
      DrawSphereEx(scaledBodyPos, 0.005F * logf(sim->bodiesList[i].radius), rings, slices, sim->bodiesList[i].color);
    }
    else
    {
      DrawPoint3D(scaledBodyPos, sim->bodiesList[i].color);
    }
  }

  DrawGrid(10, 10.0f);

  EndMode3D();

//   DrawFPS(0, 0);
//   DrawText(getISODate(sim->totalTime), 0, 25, 20, RED);

  EndTextureMode();
}

static void renderPepsiSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource)
{
	UpdateCamera(&view->camera, CAMERA_FREE);

  BeginTextureMode(Master_resource->Texture_Buffer1);

  ClearBackground(BLACK);
  BeginMode3D(view->camera);

  static float rotation;

	DrawModelEx(Master_resource->Model_PepsiCan, sim->bodiesList[0].position * (1E-11), {0, 1, 0}, -100 + rotation, {0.05, 0.05, 0.05}, WHITE);

	for (int i = 1; i < 9; i++)
	{
		DrawModelEx(Master_resource->Model_PepsiCan, sim->bodiesList[i].position * (1E-11), {0, 1, 0}, -100 + rotation, {0.01, 0.01, 0.01}, WHITE);
	}
	for (int i = 9; i < sim->bodyCount; i++)
	{
		Vector3 scaledBodyPos = sim->bodiesList[i].position * 1E-11f;
		Vector3 cameraPos = view->camera.position;

    Vector3 diff = {
        scaledBodyPos.x - cameraPos.x,
        scaledBodyPos.y - cameraPos.y,
        scaledBodyPos.z - cameraPos.z};

    double dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    if ( dist < CAMERA_MEDIUM_RANGE )
    {
      DrawSphereEx(sim->bodiesList[i].position * (1E-11), 0.005F * logf(sim->bodiesList[i].radius), 4, 5, sim->bodiesList[i].color);
    }
    else
    {
      DrawPoint3D(sim->bodiesList[i].position * 1E-11, sim->bodiesList[i].color);
    }
  }

  rotation += 0.5;

  DrawGrid(10, 10.0f);
  EndMode3D();

  EndTextureMode();
}
