/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#define NORM(x, y, z) (sqrt(((x) * (x)) + ((y) * (y)) + ((z) * (z))))

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "configuration.h"
#include "ephemerides.h"
#include "orbitalSim.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ELASTIC_CONSTANT_PLANETS 5e13
#define ELASTIC_CONSTANT_ASTEROIDS 5e-2
#define ASTEROIDS_MEAN_RADIUS 4E11F
#define ASTEROIDS_BODYNUM 1000
#define ULTRA_MASSIVE 1988500E24F

/**
 * @brief Updates simulation using gravitational force model
 * @param sim The orbital simulation
 */
static void updateUsingGravity(OrbitalSim *sim);

/**
 * @brief Updates simulation using springs elastic force model
 * @param sim The orbital simulation
 */
static void updateUsingSprings(OrbitalSim *sim);

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
    float x = getRandomFloat(0.1, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = 4 * ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
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
    body->initialPosition = body->position;
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
    OrbitalSim *simulation = new OrbitalSim();

    if (simulation)
    {
        simulation->timeStep = timeStep;
        simulation->totalTime = 0;
        simulation->bodyCount = SOLARSYSTEM_BODYNUM + ASTEROIDS_BODYNUM;
        simulation->bodiesList = new OrbitalBody[simulation->bodyCount];

        if (simulation->bodiesList)
        {
            for (int i = 0; i < SOLARSYSTEM_BODYNUM; i++)
            {
                simulation->bodiesList[i].position = solarSystem[i].position;
                simulation->bodiesList[i].initialPosition = solarSystem[i].position;
                simulation->bodiesList[i].velocity = solarSystem[i].velocity;
                simulation->bodiesList[i].mass = solarSystem[i].mass;
                simulation->bodiesList[i].radius = solarSystem[i].radius;
                simulation->bodiesList[i].color = solarSystem[i].color;
            }

            for (int i = SOLARSYSTEM_BODYNUM; i < SOLARSYSTEM_BODYNUM + ASTEROIDS_BODYNUM; i++)
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
    delete[] sim->bodiesList;
    delete sim;
}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim, int simType)
{
    if (simType == GRAVITATIONAL_SIMULATION)
    {
        updateUsingGravity(sim);
    }
    else
    {
        updateUsingSprings(sim);
    }
}

static void updateUsingGravity(OrbitalSim *sim)
{
    Vector3 gravAcc;
    Vector3 acceleration;
    float norm;
    float biggestMass = 0;
    int indexOfMostMassiveBody;

    sim->totalTime += sim->timeStep;

    for (int i = 0; i < sim->bodyCount; i++)
    {
        acceleration = {0, 0, 0};
        sim->bodiesList[i].position += sim->bodiesList[i].velocity * sim->timeStep;

        if (i < SOLARSYSTEM_BODYNUM)
        {
            if (sim->bodiesList[i].mass > biggestMass)
            {
                biggestMass = sim->bodiesList[i].mass;
                indexOfMostMassiveBody = i;
            }

            for (int j = 0; j < SOLARSYSTEM_BODYNUM; j++)
            {
                if (i != j)
                {
                    norm = NORM(sim->bodiesList[i].position.x - sim->bodiesList[j].position.x, sim->bodiesList[i].position.y - sim->bodiesList[j].position.y, sim->bodiesList[i].position.z - sim->bodiesList[j].position.z);

                    if (norm != 0)
                    {
                        gravAcc = ((sim->bodiesList[i].position - sim->bodiesList[j].position) * (-GRAVITATIONAL_CONSTANT * sim->bodiesList[j].mass)) / (norm * norm * norm);
                        acceleration += gravAcc;
                    }
                }
            }
        }
        else
        {
            int j = indexOfMostMassiveBody;

            norm = NORM(sim->bodiesList[i].position.x - sim->bodiesList[j].position.x, sim->bodiesList[i].position.y - sim->bodiesList[j].position.y, sim->bodiesList[i].position.z - sim->bodiesList[j].position.z);

            if (norm != 0)
            {
                gravAcc = ((sim->bodiesList[i].position - sim->bodiesList[j].position) * (-GRAVITATIONAL_CONSTANT * sim->bodiesList[j].mass)) / (norm * norm * norm);
                acceleration += gravAcc;
            }
        }

        sim->bodiesList[i].velocity += acceleration * sim->timeStep;
    }
}

static void updateUsingSprings(OrbitalSim *sim)
{
    Vector3 gravAcc;
    float acceleration;
    float norm;
    int j;

    sim->totalTime += sim->timeStep;

    for (int i = 0; i < sim->bodyCount; i++)
    {
        j = 0;
        acceleration = 0;
        Vector3 dist = sim->bodiesList[i].position - sim->bodiesList[j].position;
        Vector3 distRelative = sim->bodiesList[i].initialPosition - sim->bodiesList[j].position;

        Vector3 versor = dist / NORM(dist.x, dist.y, dist.z);
        double distMag = sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);

        if (distMag == 0)
            continue;

        int j = 0;

        if (i < SOLARSYSTEM_BODYNUM)
        {
            acceleration = -((NORM(dist.x, dist.y, dist.z) - NORM(distRelative.x, distRelative.y, distRelative.z)) * ELASTIC_CONSTANT_PLANETS) / sim->bodiesList[i].mass;
        }
        else
        {
            acceleration = -((NORM(dist.x, dist.y, dist.z) - NORM(distRelative.x, distRelative.y, distRelative.z)) * ELASTIC_CONSTANT_ASTEROIDS) / sim->bodiesList[i].mass;
        }

        sim->bodiesList[i].velocity += versor * acceleration * sim->timeStep;
    }

    for (int i = 0; i < sim->bodyCount; i++)
    {
        sim->bodiesList[i].position += sim->bodiesList[i].velocity * sim->timeStep;
    }

}
