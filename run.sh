#!/bin/bash

echo "Iniciando simulación de MRUA..."

if ! command -v make &> /dev/null
then
    echo "No fue posible encontrar 'make'. Por favor, instale 'make' para continuar."
    exit
fi

echo "Compilando el programa..."

make

./simulador

if ! command -v gnuplot &> /dev/null
then
    echo "gnuplot no está instalado. Por favor, instala gnuplot para generar los gráficos."
    exit
fi

echo "Generando gráficos..."

if [ "$1" == "unico" ]; then
    gnuplot gnuplot/grafico.gp
else
    gnuplot gnuplot/graficos.gp
fi