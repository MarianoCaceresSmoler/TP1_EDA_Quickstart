/**
 * @brief Menu program header
 * @author Enzo Nicolas Rosa Fernandez
 * @author Mariano Caceres Smoler
 * @author Francisco Chiusaroli
 *
 */

#include "configuration.h"
#include "view.h"

resource_t *intro(visual_sim_type_t *simVisualType, logical_sim_type_t *simLogicalType, View *view, monitor_t *monitor);

void BeginDrawing_with_blurry_filter(resource_t *Master_resource);

void BeginDrawing_without_blurry_filter(resource_t *Master_resource);

void update_blur_shader(resource_t *Master_resource, monitor_t *monitor, float factor);

bool fading_black_wall(monitor_t *monitor);

void kill_resources(resource_t *Master_resource);