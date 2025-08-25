# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* [Mariano Cáceres Smoler]: [contribución]
* [Francisco Chiusaroli]: [contribución]
* [Enzo Nicolás Rosa Fernández]: [contribución]

[completar]

## Verificación del timestep

Probamos cambiando el timeMultiplier multiplicando por números cada mes más grandes, así cambiando el timeStep: timeMultiplier = 10* SECONDS_PER_DAY, 100 * SECONDS_PER_DAY y 1000 * SECONDS_PER_DAY.

Tambien probamos ejecutando múltiples timeStep por frame para acelerar la simulacion:
for (int i = 0; i < 5,10,n; i++) // n  updates por frame
    updateOrbitalSim(sim);

Asi vimos que para valores de timeStep muy grandes, se pierden muchos cálculos intermedios, con lo que las órbitas se desconfiguran 
y los planetas salen volando.
Con 10 o 100 días/ por segundo, los planetas mantienen órbitas estables.
Con 1000 días por segundo, ya apreciamos que algunos planetas salen volando, ya que el metodo y la aproximacion
utilizada es sensible a pasos grandes.

Para simular más años más rápido, vimos así que era preferible usar múltiples updates por frame en vez de aumentar timeStep, porque así reducimos 
el error acumulado. Con 5 o 10 updates por frame la simulacion va rapido y no pierde precisión.

## Verificación del tipo de datos float

[completar]

## Complejidad computacional con asteroides

El primer programa que planteamos hacía:
for (i = 0; i < bodyCount; i++)
    for (j = 0; j < bodyCount; j++)

Esto da una complejidad computacional O(n²).
Con los 9 cuerpos originales esto no supone mucho (81 iteraciones), pero sumando 500 asteorides alcanzamos 510² ≈ 260.000 iteraciones,
y con 1000 ya superamos el millón.

## Mejora de la complejidad computacional

En primer lugar, notamos que el cuello de botella en los graficos viene dado por la funcion de DrawSphere. Sacando esta y cambiando el algoritmo de actualizacion
fisica con los puntos detallados a continuacion, ya alcanzamos los 60FPS. 
En primer lugar, para los asteroides calculamos solo la atracción con el sol, y no con los asteroides entre sí ni con los planetas.
Luego, para


## Bonus points

[completar]

## COMENTAR TODO AL FINAL