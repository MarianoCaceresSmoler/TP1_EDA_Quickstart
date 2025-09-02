# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* Mariano Cáceres Smoler: armado de algoritmo optimizado y documentacion
* Francisco Chiusaroli: completado de modelos y realizacion de testeos
* Enzo Nicolás Rosa Fernández: manejo de graficos y renderizado con raylib

[completar]

## Verificación del timestep

Probamos cambiando el timeMultiplier multiplicando por números cada mes más grandes, así cambiando el timeStep: timeMultiplier = 10* SECONDS_PER_DAY, 100 * SECONDS_PER_DAY y 1000 * SECONDS_PER_DAY.

Tambien probamos ejecutando múltiples timeStep por frame para acelerar la simulacion:
for (int i = 0; i < 5,10,n; i++) // n  updates por frame
    updateOrbitalSim(sim);

Asi vimos que para valores de timeStep muy grandes, se pierden muchos cálculos intermedios, con lo que las órbitas se desconfiguran 
y los planetas salen volando.
Con 10 días por segundo, los planetas mantienen órbitas bastante estables.
Con 100 o 1000 días por segundo y bastantes actualizaciones, ya apreciamos que algunos planetas salen volando, ya que el metodo y la aproximacion
utilizada es sensible a pasos grandes.

Para simular más años más rápido, vimos así que era preferible usar múltiples updates por frame en vez de aumentar timeStep, porque así reducimos 
el error acumulado. Con 10 updates por frame la simulacion va rapido y no pierde precisión. Sin embargo, de esta forma se pierden FPS.

## Verificación del tipo de datos float

Para guardar posiciones, velocidades y aceleraciones usamos Vector3 de raylib, que internamente usa floats para cada componente. Y luego, para las masas, radios y tiempos tambien usamos floats.

Float tiene una precision de aproximadamente 6 a 9 dígitos decimales (aproximadamente 7 digitos), y requiere 32 bits (4 bytes) de memoria. De esta forma, toma valores en un rango de ±3,4e38.

- Los datos de las masas son menores o iguales a aproximadamente 2e30 (masa del sol)
- Los radios de los cuerpos son menores o iguales a aproximadamente 7e8 (radio del sol).
- Las posiciones están en un rango de ±9e11 metros. 

Podemos observar como estos 3 datos mencionados entran bien dentro de un float.

Por otro lado, en cuanto al calculo de la aceleracion gravitatoria:

- gravAcc = ((sim->bodiesList[i].position - sim->bodiesList[j].position) * (-GRAVITATIONAL_CONSTANT * sim->bodiesList[j].mass)) / (norm * norm * norm);

Sabiendo que GRAVITATIONAL_CONSTANT 6.6743E-11F, y teniendo en cuenta los rangos de los datos que obtuvimos anteriormente, podemos observar que cada termino del cociente entra dentro del rango de float.

Asi, concluimos que para visualización simplificada float es suficiente. Sin embargo, para alta precisión, estaría bien usar double.

## Complejidad computacional con asteroides

El primer programa que planteamos hacía:
for (i = 0; i < bodyCount; i++)
    for (j = 0; j < bodyCount; j++)

Esto da una complejidad computacional O(n²).
Con los 9 cuerpos originales esto no supone mucho (81 iteraciones), pero sumando 500 asteorides alcanzamos 510² ≈ 260.000 iteraciones,
y con 1000 ya superamos el millón.

## Mejora de la complejidad computacional

En primer lugar, notamos que el cuello de botella en los graficos viene dado por la funcion de DrawSphere. Lo que hicimos para mejorar esto fue definir 3 rangos de vision, teniendo en cuenta la distancia de la camara a cada objeto. Para la distancia mas corta, seguimos usando DrawSphere. Para una distancia media, usamos 
DrawSphereEx, que reduce bastante la resolucion de la esfera dibujada pero todavia queda algo definida. Y por ultimo, para la distancia larga, dibujamos los cuerpos solo como puntos y no como esferas.

Por otro lado, para recudir la complejidad computacional, hicimos dos simplificaciones:
- Para los planetas y el sol: calculamos la atraccion gravitatoria unicamente entre ellos mismos.
- Para los asteroides: calculamos la atraccion gravitatoria unicamente con el sol y el resto de planetas

De esta forma, pasamos de un O(n²) a aproximadamente O(n) (1 for de n repeticiones con otro fo interno de solo 9 iteraciones, dando como resultado 9*n iteraciones). Asi, se pierde alg de precision para los asteroides sobre todo, pero que no es tan tan relevante por la diferencia de masa de los cuerpos.

## Bonus points

[completar]
