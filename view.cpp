/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include <iostream>
#include <string>
#include <time.h>

#include "view.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define CAMERA_RANGE 100

Model Pepsi_can;

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
  return TextFormat("%04d-%02d-%02d",
                    1900 + localTM->tm_year,
                    localTM->tm_mon + 1,
                    localTM->tm_mday);
}

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
View *constructView(int fps) {
  View *view = new View();

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "EDA Orbital Simulation");
  ToggleFullscreen();
  SetTargetFPS(fps);
  DisableCursor();

  view->camera.position = {10.0f, 10.0f, 10.0f};
  view->camera.target = {0.0f, 10.0f, 0.0f};
  view->camera.up = {0.0f, 1.0f, 0.0f};
  view->camera.fovy = 45.0f;
  view->camera.projection = CAMERA_PERSPECTIVE;

  Pepsi_can = LoadModel("./Models/Pepsi_Basic/Pepsi_Can.obj");

  return view;
}

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(View *view) {
  UnloadModel(Pepsi_can);
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
void renderView(View *view, OrbitalSim *sim) {
  double norm;
  static float rotation;
  UpdateCamera(&view->camera, CAMERA_FREE);

  BeginDrawing();

  ClearBackground(BLACK);
  BeginMode3D(view->camera);

  // DrawModelEx(Pepsi_can, {0, -5, 0}, {0, 1, 0}, -100 + rotation, {0.4, 0.4, 0.4}, WHITE);

  DrawModelEx(Pepsi_can, sim->bodiesList[0].position * (1E-11), {0, 1, 0}, -100 + rotation, {0.05, 0.05, 0.05}, WHITE);

  for ( int i = 1; i < 9; i++ ) {
    if ( (view->camera.position.y < CAMERA_RANGE) && (view->camera.position.y > -CAMERA_RANGE) ) {
      // DrawSphereEx(sim->bodiesList[i].position * (1E-11), 0.005F * logf(sim->bodiesList[i].radius), 4, 5, sim->bodiesList[i].color);
      DrawModelEx(Pepsi_can, sim->bodiesList[i].position * (1E-11), {0, 1, 0}, -100 + rotation, {0.01, 0.01, 0.01}, WHITE);
    } else {
      DrawPoint3D(sim->bodiesList[i].position * 1E-11, sim->bodiesList[i].color);
    }
  }
  for ( int i = 9; i < sim->bodyCount; i++ ) {
    if ( (view->camera.position.y < CAMERA_RANGE) && (view->camera.position.y > -CAMERA_RANGE) ) {
      // DrawSphereEx(sim->bodiesList[i].position * (1E-11), 0.005F * logf(sim->bodiesList[i].radius), 4, 5, sim->bodiesList[i].color);
      DrawModelEx(Pepsi_can, sim->bodiesList[i].position * (1E-11), {0, 1, 0}, -100 + rotation, {0.002, 0.002, 0.002}, WHITE);
    } else {
      DrawPoint3D(sim->bodiesList[i].position * 1E-11, sim->bodiesList[i].color);
    }
  }

  // DrawGrid(10, 10.0f);
  EndMode3D();

  DrawFPS(0, 0);
  DrawText(getISODate(sim->totalTime), 0, 25, 20, RED);
  char buffer[10];
  sprintf(buffer, "%f", view->camera.position.y);
  DrawText(buffer, 0, 45, 20, WHITE);
  sprintf(buffer, "%.1f", -100 + rotation);
  DrawText(buffer, 0, 65, 20, WHITE);
  rotation += 0.5;

  EndDrawing();
}
