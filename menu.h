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

void kill_resources(resource_t *Master_resource);