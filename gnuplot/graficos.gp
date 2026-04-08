# Configuración general para las graficas
set terminal pngcairo size 800,600
set datafile separator ","

# Grafica posición vs tiempo :D
set output 'posicion_vs_tiempo.png'
set title "Posición vs Tiempo"
set xlabel "Tiempo (s)"
set ylabel "Posición (m)"
set grid
plot "datos.csv" using 1:3 smooth csplines with lines

# Grafica velocidad vs tiempo :D
set output 'velocidad_vs_tiempo.png'
set title "Velocidad vs Tiempo"
set xlabel "Tiempo (s)"
set ylabel "Velocidad (m/s)"
set grid
plot "datos.csv" using 1:2 smooth csplines with lines

# Cerramos la última salida
set output