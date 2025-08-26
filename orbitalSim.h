/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib.h"
#include "raymath.h"

/**
 * @brief Orbital body definition
 */
struct OrbitalBody
{
    Vector3 position;
    Vector3 initialPosition; // [m]
    Vector3 velocity;
    float mass;
    float radius;
    Color color;
};

/**
 * @brief Orbital simulation definition
 */
struct OrbitalSim
{
    float timeStep;
    float totalTime;
    unsigned short int bodyCount;
    OrbitalBody * bodiesList;
};

OrbitalSim *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void updateOrbitalSim(OrbitalSim *sim, int simType);

#endif
