# PdM_2_traffic_light

Semáforo Modularizado con 2 Botones

Implementación mediante el modelo de Máquina de Estados Finitos (MEF) el antirrebote de  teclas y la lógica de funcionamiento de un semáforo.

Este proyecto es un semáforo modularizado que utiliza dos botones para controlar su funcionamiento. El semáforo consta de tres luces: roja, amarilla y verde con in tiempo estándar inicial de 3000, 500, 2000 milisegundos respectivamente.

## Botón 1 (SW1) 
Se utiliza para alternar entre los diferentes estados del semáforo:
i. Normal. Secuencia de leds: verde 3 s, verde y amarillo 500 ms, rojo 2 s, verde nuevamente....
ii. Desconectado. Led amarillo intermitente cada 500 ms.
iii. Alarma. Led rojo intermitente cada 1 s

## Botón 2 (SW2)
Al hacer clic en este botón se podrá aumentar la rapidez de la secuencia (0.5x, 1x, 2x)

# Hardware
En esta practica se montó 2 pulsantes denominados como SW1 y SW2, tambien para la implementación del semáforo se uso 4 leds siendo 3 leds usados para la demostración del funcionamiento (verde, amarillo y rojo) y un led azul usado como indicador de que se ha activado los pulsantes.

# Bucle principal
En el bucle de loop se ubicara las 3 funciones princiaples de nuestro programa:
FsmButtonInit(): Que iniciara la maquina de estados
FsmButtonUpdate: Que actualizara la maquina de estados y estará ligada a los 2 botones para controlar el programa
Sequence: Que será la funcion encargada de realizar la transición entre cada uno de los diferentes estados del semaforo

# Funciones implementadas
ButtonPressed: Esta función estará integrada dentro de la maquina de estados antirebotes para el botón 1, nos indicara que se presiono el botón y ademas estará acompañada de el encendido del led azul.
ButtonReleased: Apagará el led azul indicando que se terminó de presionar el primer botón y se seleccionó uno de los 3 siguientes modos: NORMAL, DISCONNECTED, ALARM.
FsmButtonError: Hace uso de la máquina de estados para antirebotes dejando el estado del boton por defecto como si no se hubiera presionado.
/NOTA: Las funciones marcadas al final con el numero 2 tienen la misma funcion que las descritas anteriormente con la diferencia que actuan sobre la maquina de estados que acelera el tiempo de transiciones entres los leds del semáforo/

## Función para retraso no bloqueante
NonBlockingDelay: Esta función le da la libertad al programa de poder manejar varias funciones sin que se tenga que interrrumpir otros procesos.

## Funciones de semáforo
Sequence: Esta función se encargará de ejecutar los 3 modos de operación del semaforo asociadas al primer botón.
SelectTime:Esta función estará asociada con el segundo botón para modificar el tiempo con el que transcurre la secuencia del semáforo.
TurnOffLeds: Esta función se encargará de ir haciendo que se vaya apagando las luces del semáforo en orden secuencial.
