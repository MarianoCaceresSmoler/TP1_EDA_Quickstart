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
    Vector3 initialPosition;
    Vector3 velocity;
    float mass;
    float radius;
    Color color;
};

/**
 * @brief Space ship definition
 */
struct SpaceShip
{
    Vector3 position;
    Vector3 initialPosition;
    Vector3 velocity;
    Vector3 orientation;
    float mass;
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
    SpaceShip * ship;
};

/**
 * @brief Keyboard inputs data type
 */
typedef struct
{
    int keyUp;
    int keyDown;
    int keyLeft;
    int keyRight;
} keyboartInputs_t;

OrbitalSim *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void updateOrbitalSim(OrbitalSim *sim, int simType);
void updateSpaceShip(SpaceShip *ship, keyboartInputs_t inputs, float timeStep);

#endif
