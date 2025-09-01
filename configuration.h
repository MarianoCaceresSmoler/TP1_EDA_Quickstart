/**
 * @brief Constants, defines and types for orbital simulation
 * @author Mariano Caceres Smoler
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

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
}monitor_t;

#endif
