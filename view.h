/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIMVIEW_H
#define ORBITALSIMVIEW_H

#include "configuration.h"
#include "orbitalSim.h"

/**
 * The view data
 */
struct View
{
	Camera3D camera;
};

View *constructView(int *fps, monitor_t *monitor);
void setup_3D_view(View *view);
void destroyView(View *view);

bool isViewRendering(View *view);
void renderView(View *view, OrbitalSim *sim, resource_t *Master_resource, int simType, bool camera_movement, bool ship_enable);

const char *getISODate(float timestamp);


#endif
