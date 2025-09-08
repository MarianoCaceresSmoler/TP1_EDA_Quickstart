# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* Mariano Cáceres Smoler: armado de algoritmo optimizado y documentacion
* Francisco Chiusaroli: completado de modelos y realizacion de testeos
* Enzo Nicolás Rosa Fernández: manejo de graficos y renderizado con raylib

## Verificación del timestep

Probamos cambiando el timeMultiplier multiplicando por números cada mes mas grandes, así cambiando el timeStep: timeMultiplier = 10* SECONDS_PER_DAY, 100 * SECONDS_PER_DAY y 1000 * SECONDS_PER_DAY.

Tambien probamos ejecutando multiples timeStep por frame para acelerar la simulacion:
for (int i = 0; i < 5,10,n; i++) // n  updates por frame
    updateOrbitalSim(sim);

Asi vimos que para valores de timeStep muy grandes, se pierden muchos calculos intermedios, con lo que las orbitas se desconfiguran y los planetas salen volando.
Con 10 dias por segundo, los planetas mantienen orbitas bastante estables. Con 100 o 1000 días por segundo y bastantes actualizaciones, ya apreciamos que algunos planetas salen volando, ya que el metodo y la aproximacion utilizada es sensible a pasos grandes.

Para simular mas años más rápido, vimos asi que era preferible usar multiples updates por frame en vez de aumentar timeStep, porque asi reducimos 
el error acumulado. Con 5 updates por frame la simulacion va "rapido" y no pierde precisión. Sin embargo, de esta forma se pierden FPS.

## Verificación del tipo de datos float

Para guardar posiciones, velocidades y aceleraciones, usamos Vector3 de raylib, que internamente usa floats para cada componente. Y luego, para las masas, radios y tiempos tambien usamos floats.

Float tiene una precision de aproximadamente 7 digitos significativos, y requiere 32 bits (4 bytes) de memoria. De esta forma, toma valores en un rango de ±3,4e38.

- Los datos de las masas son menores o iguales a aproximadamente 2e30 (masa del sol)
- Los radios de los cuerpos son menores o iguales a aproximadamente 7e8 (radio del sol).
- Las posiciones están en un rango de ±9e11 metros. 

Podemos observar como estos 3 datos mencionados entran bien dentro de un float.

Por otro lado, en cuanto al calculo de la aceleracion gravitatoria:

- gravAcc = ((sim->bodiesList[i].position - sim->bodiesList[j].position) * (-GRAVITATIONAL_CONSTANT * sim->bodiesList[j].mass)) / (norm * norm * norm);

Sabiendo que GRAVITATIONAL_CONSTANT 6.6743E-11F, y teniendo en cuenta los rangos de los datos que obtuvimos anteriormente, podemos observar que cada termino del cociente entra dentro del rango de float.

Sin embargo, y aunque los valores entran dentro del rango, float solo ofrece unos 7 digitos significativos. Esto significa que por ejemplo, para las posiciones del orden de 10e11 metros, la resolucion minima es de 10e11 x 10x-7 = 10e4 metros (10km). Es decir, se pierden detalles menores a unos kilometros. Los errores de redondeo ademas se acumulan a cada iteracion, con lo que se acumula inestabilidad en las orbitas a largo plazzo

Asi, concluimos que para visualizacion simplificada y a corto plazo float es suficiente. Sin embargo, para alta precision o simulaciones de larga duracion, estaría bien usar double (que ofrece 15 digitos significativos y reduce la acumulacion de errores).

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
- Para los asteroides: calculamos la atraccion gravitatoria unicamente con el cuerpo mas masivo

De esta forma, pasamos de un O(n²) a aproximadamente O(n) (1 for de n repeticiones con otro fo interno de solo 9 iteraciones, dando como resultado 9*n iteraciones). Asi, se pierde algo de precision para los asteroides sobre todo, pero que no es tan tan relevante por la diferencia de masa de los cuerpos.

## Bonus points
Para los bonus points, optamos por tocar mas las cosas graficas y esteticas del programa:
- Agregamos una nave espacial que sigue la camara y el movimiento del jugador.
- Incluimos modelos 3d para representar los diferentes cuerpos del programa: la nave, los planetas. (ver build/Assets).
- Cambiamos el reescalado de los cuerpos (su posicion y tamaño), usando raices en lugar de lugaritmos. Y junto a esto, espaciamos la disposicion de los asteroides (lo que mejora la visualizacion y la complejidad grafica).

Tambien, incoporamos un modelo fisico propio: una aproximacion del modelo masa-resorte para simular el comportamiento de los cuerpos. Usamos esto para poder correr la simulacion usando un comportamiento oscilatorio elastico. Por esto ademas incluimos un menu para hacer configurable la simulacion (el usuario puede seleccionar en que modelo ejecutar el programa).
Para ver el menu, usa la tecla BACKSPACE. Para ingresar a la simulacion, toca ENTER. Para salir de la simulacion, usa ESC.

Por ultimo, en cuanto al easter egg, creemos que tiene que ver con la configuracion inicial de los asteroides en phi = 0 en lugar de en un angulo random (lo que hace que los asteroides partan todos desde el mismo angulo sin distincion).

Adicionalmente, decidimos implementar nuestro propio "easter egg" ;).
