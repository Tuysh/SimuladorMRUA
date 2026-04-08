================================================================================
                                SIMULADOR MRUA
================================================================================

    Un simulador interactivo de Movimiento Rectilíneo Uniformemente 
    Acelerado (MRUA) escrito en C.

=================================================================================
  DESCRIPCIÓN DEL PROGRAMA
================================================================================

El programa permite simular dos escenarios principales:
  - Caída Libre
  - Tiro Vertical Hacia Arriba

El resultado se mostrará de forma resumida en la consola y se exportará el 
registro completo a un archivo "datos.csv".

================================================================================
  REQUISITOS
================================================================================

Para compilar y ejecutar este programa necesitas:
  - Un compilador de C (GCC).
  - make (opcional)
  - gnuplot (opcional)

================================================================================
  COMPILAR
================================================================================

Si quiere compilar manualmente ejecute el siguiente comando en su terminal:

    gcc simulador.c -o simulador -lm

Si tiene make en su sistema ejecute el siguiente comando

    make

================================================================================
  Ejecutar
================================================================================

Si compilo manualmente o con make simplemente ejecute:

    ./simulador

O si quiere ejecutar el programa y generar las graficas (requiere make y gnuplot)
ejecute:

    sh run.sh

O en dado caso de querer generar manualmente las graficas con gnuplot ejecute 
el siguiente comando:

    gnuplot gnuplot/graficos.gp

O en dado caso de que quiera genera una sola grafica unica:
    
    gnuplot gnuplot/grafico.gp