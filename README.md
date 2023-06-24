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
