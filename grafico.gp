# Configuración general para las graficas
set terminal pngcairo size 1200,1600
set datafile separator ","

set output 'graficas_mrua_completas.png'

set multiplot layout 2,1 title "Graficas MRUA"

# Grafica posición vs tiempo :D
set title "Posición vs Tiempo"
set xlabel "Tiempo (s)"
set ylabel "Posición (m)"
set grid
plot "datos.csv" using 1:3 smooth csplines with lines

# Grafica velocidad vs tiempo :D
set title "Velocidad vs Tiempo"
set xlabel "Tiempo (s)"
set ylabel "Velocidad (m/s)"
set grid
plot "datos.csv" using 1:2 smooth csplines with lines

# Cerramos el multiplot
unset multiplot
set output