/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 *
 * Sources:
 * https://akselmo.dev/posts/how-to-rotate-an-actor-with-raylib/ ; Inspiration for rotating models
 * https://chatgpt.com/share/68be4269-8e2c-800e-aa0f-0acf02e21886 ; How to use quaternions to rotate models
 */

#include <iostream>
#include <string>
#include <time.h>

#include "configuration.h"
#include "orbitalSim.h"
#include "view.h"

// Macros and constant definitions
#define SETUP_WINDOW_WIDTH 640
#define SETUP_WINDOW_HEIGHT 480
#define CAMERA_SHORT_RANGE 50
#define CAMERA_MEDIUM_RANGE 250
#define ADJUSTMENT_FACTOR 5E-12F

static void renderStandardSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource, bool ship_enable);
static void renderPepsiSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource, bool ship_enable);
static void renderSpaceShip(View *view, OrbitalSim *sim, resource_t *Master_resource);

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *        to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
const char *getISODate(float timestamp)
{
	// Timestamp epoch: 1/1/2022
	struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

	// Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
	time_t unixEpoch = mktime(&unichEpochTM);
	time_t unixTimestamp = unixEpoch + (time_t)timestamp;

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
View *constructView(int *fps, monitor_t *monitor)
{
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

/**
 * @brief Sets the 3D View up
 *
 * @param view pointer to the view
 */
void setup_3D_view(View *view)
{
	DisableCursor();

	view->camera.position = {50.0f, 20.0f, 50.0f};
	view->camera.target = {0.0f, 0.0f, 0.0f};
	view->camera.up = {0.0f, 1.0f, 0.0f};
	view->camera.fovy = 90.0f;
	view->camera.projection = CAMERA_PERSPECTIVE;
}

/**
 * @brief Destroys an orbital simulation view
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
 * @return returns whether the view should continue rendering (unless ESC is pressed)
 */
bool isViewRendering(View *view)
{
	return !WindowShouldClose();
}

/**
 * @brief Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */
void renderView(View *view, OrbitalSim *sim, resource_t *Master_resource, int simType, bool camera_movement, bool ship_enable)
{

	if (camera_movement)
		UpdateCamera(&view->camera, CAMERA_THIRD_PERSON);

	if (simType == PLANETS_SIMULATION)
	{
		renderStandardSimulation(view, sim, Master_resource, ship_enable);
	}
	else if (simType == PEPSI_SIMULATION)
	{
		renderPepsiSimulation(view, sim, Master_resource, ship_enable);
	}
}

/**
 * @brief Renders the standard simulation, with planets
 *
 * @param view The view
 * @param sim The orbital sim
 * @param Master_resource Pointer to the struct containing all graphical data
 * @param ship_enable Flag to enable space ship
 */
static void renderStandardSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource, bool ship_enable)
{

	BeginTextureMode(Master_resource->Texture_Buffer1);
	ClearBackground(BLACK);
	BeginMode3D(view->camera);

	static float rotation;

	for (int i = 0; i < sim->bodyCount; i++)
	{

		Vector3 scaledBodyPos = {5E-10F * (sim->bodiesList[i].position.x), 5E-10F * (sim->bodiesList[i].position.y), 5E-10F * (sim->bodiesList[i].position.z)};

		Vector3 &cameraPos = view->camera.position;

		if (i < Master_resource->Models_Solar_System.size())
		{
			switch (i)
			{
			case 0:
				DrawModelEx(Master_resource->Model_Sun, scaledBodyPos - (Vector3){0.0, 15.0, 0.0}, {0, 1, 0}, 0, {15.0F, 15.0F, 15.0F}, GOLD);
				break;
			case 1:
				DrawModelEx(Master_resource->Model_Mercury, scaledBodyPos, {0, 1, 0}, 0, {1.0F, 1.0F, 1.0F}, WHITE);
				break;
			case 2:
				DrawModelEx(Master_resource->Model_Venus, scaledBodyPos, {0, 1, 0}, 0, {2.0F, 2.0F, 2.0F}, WHITE);
				break;
			case 3:
				DrawModelEx(Master_resource->Model_Earth, scaledBodyPos, {0, 1, 0}, 0, {2.0F, 2.0F, 2.0F}, WHITE);
				break;
			case 4:
				DrawModelEx(Master_resource->Model_Mars, scaledBodyPos, {0, 1, 0}, 0, {1.0F, 1.0F, 1.0F}, WHITE);
				break;
			case 5:
				DrawModelEx(Master_resource->Model_Jupiter, scaledBodyPos, {0, 1, 0}, 0, {5.0F, 5.0F, 5.0F}, WHITE);
				break;
			case 6:
				DrawModelEx(Master_resource->Model_Saturn, scaledBodyPos, {0, 1, 0}, 0, {5.0F, 5.0F, 5.0F}, WHITE);
				break;
			case 7:
				DrawModelEx(Master_resource->Model_Uranus, scaledBodyPos, {0, 1, 0}, 0, {4.0F, 4.0F, 4.0F}, WHITE);
				break;
			case 8:
				DrawModelEx(Master_resource->Model_Neptune, scaledBodyPos, {0, 1, 0}, 0, {3.0F, 3.0F, 3.0F}, WHITE);
				break;
			}
		}
		else
		{
			Vector3 diff = {
				scaledBodyPos.x - cameraPos.x,
				scaledBodyPos.y - cameraPos.y,
				scaledBodyPos.z - cameraPos.z};

			double dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);

			if (dist < CAMERA_SHORT_RANGE)
			{
				DrawModelEx(Master_resource->Models_Asteroids[i % 4], scaledBodyPos, {0, 1, 0}, 0, {0.4F, 0.4F, 0.4F}, WHITE);
			}
			else if (dist < CAMERA_MEDIUM_RANGE)
			{
				int rings = 2;
				int slices = 3;

				DrawSphereEx(scaledBodyPos, 0.03F * cbrt(sim->bodiesList[i].radius), rings, slices, sim->bodiesList[i].color);
			}
			else
			{
				DrawPoint3D(scaledBodyPos, sim->bodiesList[i].color);
			}
		}
	}
	if (ship_enable)
	{
		renderSpaceShip(view, sim, Master_resource);
	}

	EndMode3D();

	EndTextureMode();
}

/**
 * @brief Renders the special simulation, with Pepsi cans B)
 *
 * @param view The view
 * @param sim The orbital sim
 * @param Master_resource Pointer to the struct containing all graphical data
 * @param ship_enable Flag to enable space ship
 */
static void renderPepsiSimulation(View *view, OrbitalSim *sim, resource_t *Master_resource, bool ship_enable)
{

	BeginTextureMode(Master_resource->Texture_Buffer1);

	ClearBackground(BLACK);
	BeginMode3D(view->camera);

	static float rotation;

	DrawModelEx(Master_resource->Model_PepsiCan, {5E-10F * (sim->bodiesList[0].position.x), 5E-10F * (sim->bodiesList[0].position.y) - 15.0F, 5E-10F * (sim->bodiesList[0].position.z)}, {0, 1, 0}, -100 + rotation, {0.5F, 0.5F, 0.5F}, WHITE);

	for (int i = 1; i < 9; i++)
	{
		DrawModelEx(Master_resource->Model_PepsiCan, {5E-10F * (sim->bodiesList[i].position.x), 5E-10F * (sim->bodiesList[i].position.y), 5E-10F * (sim->bodiesList[i].position.z)}, {0, 1, 0}, -100 + rotation, {0.1F, 0.1F, 0.1F}, WHITE);
	}
	for (int i = 9; i < sim->bodyCount; i++)
	{
		Vector3 scaledBodyPos = {5E-10F * (sim->bodiesList[i].position.x), 5E-10F * (sim->bodiesList[i].position.y), 5E-10F * (sim->bodiesList[i].position.z)};
		Vector3 &cameraPos = view->camera.position;

		Vector3 diff = {
			scaledBodyPos.x - cameraPos.x,
			scaledBodyPos.y - cameraPos.y,
			scaledBodyPos.z - cameraPos.z};

		double dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
		if (dist < CAMERA_SHORT_RANGE)
		{
			DrawModelEx(Master_resource->Models_Asteroids[i % 4], scaledBodyPos, {0, 1, 0}, 0, {0.4F, 0.4F, 0.4F}, WHITE);
		}
		else if (dist < CAMERA_MEDIUM_RANGE)
		{
			int rings = 2;
			int slices = 3;

			DrawSphereEx(scaledBodyPos, 0.03F * cbrt(sim->bodiesList[i].radius), rings, slices, sim->bodiesList[i].color);
		}
		else
		{
			DrawPoint3D(scaledBodyPos, sim->bodiesList[i].color);
		}
	}

	rotation += 0.5;

	if (ship_enable)
	{
		renderSpaceShip(view, sim, Master_resource);
	}
	EndMode3D();

	EndTextureMode();
}

/**
 * @brief Renders the spaceship
 *
 * @param view The view
 * @param sim The orbital sim
 * @param Master_resource Pointer to the struct containing all graphical data
 */
static void renderSpaceShip(View *view, OrbitalSim *sim, resource_t *Master_resource)
{
	static float rotation = 0.0f;
	static float tiltX = 0.0f;
	static float tiltZ = 0.0f;

	float maxTilt = 15.0f;
	float tiltSpeed = 50.0f;
	float recoverSpeed = 30.0f;

	// Camera direction
	Vector3 forward = Vector3Normalize(Vector3Subtract(view->camera.target, view->camera.position));
	Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, (Vector3){0, 1, 0}));

	// Input
	if (IsKeyDown(KEY_S))
		tiltX += tiltSpeed * GetFrameTime();
	if (IsKeyDown(KEY_W))
		tiltX -= tiltSpeed * GetFrameTime();
	if (IsKeyDown(KEY_D))
		tiltZ += tiltSpeed * GetFrameTime();
	if (IsKeyDown(KEY_A))
		tiltZ -= tiltSpeed * GetFrameTime();

	// Tilt limits
	if (tiltX > maxTilt)
		tiltX = maxTilt;
	if (tiltX < -maxTilt)
		tiltX = -maxTilt;
	if (tiltZ > maxTilt)
		tiltZ = maxTilt;
	if (tiltZ < -maxTilt)
		tiltZ = -maxTilt;

	// Goes back to the center
	if (!IsKeyDown(KEY_W) && !IsKeyDown(KEY_S))
	{
		if (tiltX > 0)
			tiltX -= recoverSpeed * GetFrameTime();
		if (tiltX < 0)
			tiltX += recoverSpeed * GetFrameTime();
	}
	if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
	{
		if (tiltZ > 0)
			tiltZ -= recoverSpeed * GetFrameTime();
		if (tiltZ < 0)
			tiltZ += recoverSpeed * GetFrameTime();
	}

	// Creates Quaternions
	Quaternion qYaw = QuaternionFromAxisAngle((Vector3){0, 1, 0}, DEG2RAD * rotation);
	Quaternion qPitch = QuaternionFromAxisAngle(right, DEG2RAD * tiltX);
	Quaternion qRoll = QuaternionFromAxisAngle(forward, DEG2RAD * tiltZ);
	Quaternion qFinal = QuaternionMultiply(qYaw, QuaternionMultiply(qPitch, qRoll));

	// Assign transformation to model
	Master_resource->Model_SpaceShip.transform = QuaternionToMatrix(qFinal);

	DrawModel(
		Master_resource->Model_SpaceShip,
		view->camera.target,
		0.1F,
		WHITE);
}