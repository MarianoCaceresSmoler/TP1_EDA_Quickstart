/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#define NORM(x, y, z) (sqrt(((x) * (x)) + ((y) * (y)) + ((z) * (z))))

#include <stdlib.h>
#include <math.h>

#include "orbitalSim.h"
#include "ephemerides.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F
#define ASTEROIDS_BODYNUM 100

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(OrbitalBody *body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
    float phi = getRandomFloat(0, 2.0F * (float)M_PI);

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    body->radius = 2E3F; // Typical asteroid radius: 2km
    body->color = GRAY;
    body->position = {r * cosf(phi), 0, r * sinf(phi)};
    body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim *constructOrbitalSim(float timeStep)
{
    OrbitalSim * simulation = (OrbitalSim *) malloc(sizeof(OrbitalSim));

    if(simulation)
    {
        simulation->timeStep = timeStep;
        simulation->totalTime = 0;
        simulation->bodyCount = SOLARSYSTEM_BODYNUM + ASTEROIDS_BODYNUM;
        simulation->bodiesList = (OrbitalBody *) calloc(SOLARSYSTEM_BODYNUM + ASTEROIDS_BODYNUM, sizeof(OrbitalBody));

        if(simulation->bodiesList)
        {
            for(int i = 0; i < SOLARSYSTEM_BODYNUM; i++)
            {
                simulation->bodiesList[i].position = solarSystem[i].position;
                simulation->bodiesList[i].velocity = solarSystem[i].velocity;
                simulation->bodiesList[i].mass = solarSystem[i].mass;
                simulation->bodiesList[i].radius = solarSystem[i].radius;
                simulation->bodiesList[i].color = solarSystem[i].color;
            }

            for(int i = SOLARSYSTEM_BODYNUM; i < SOLARSYSTEM_BODYNUM + ASTEROIDS_BODYNUM; i++)
            {
                configureAsteroid(&simulation->bodiesList[i], simulation->bodiesList[0].mass);
            }

            return simulation;
        }
    }

    return NULL;
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{
    free(sim->bodiesList);
    free(sim);
}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim)
{
    Vector3 gravAcc, acceleration;
    double norm;

    sim->totalTime += sim->timeStep;

    for(int i = 0; i < sim->bodyCount; i++)
    {
        acceleration = {0, 0, 0};
        for(int j = 0; j < sim->bodyCount; j++)
        {
            if(i != j)
            {
                norm = NORM(sim->bodiesList[i].position.x - sim->bodiesList[j].position.x, sim->bodiesList[i].position.y - sim->bodiesList[j].position.y, sim->bodiesList[i].position.z - sim->bodiesList[j].position.z);
                if (norm != 0)
                {
                    gravAcc = ((sim->bodiesList[i].position - sim->bodiesList[j].position) * (-GRAVITATIONAL_CONSTANT * sim->bodiesList[j].mass)) / (norm * norm * norm);
                    acceleration += gravAcc;   
                }            
            }
        }

        sim->bodiesList[i].velocity += acceleration * sim->timeStep;
    }

    for(int i = 0; i < sim->bodyCount; i++)
    {
        sim->bodiesList[i].position += sim->bodiesList[i].velocity * sim->timeStep;
    }
}
