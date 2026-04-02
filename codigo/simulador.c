// Importamos las bibliotecas necesarias para la entrada/salida, manejo de memoria y funciones matemáticas.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Definimos constantes que usaremos en el programa
#define ARCHIVO_RESULTADOS "datos.csv" // Nombre del archivo CSV donde se guardarán los resultados
#define DECIMALES_CSV "%.4lf"          // Formato para imprimir números con 4 decimales en el CSV
#define PASOS_MAXIMOS 1000000          // Número máximo de pasos para la simulación, ajustable según las necesidades
#define GRAVEDAD_TERRESTRE -9.81       // Aceleración debida a la gravedad terrestre en m/s^2, negativa porque actúa hacia abajo

// TODO: Actualizar documetanción
// TODO: Corregir la derivada numérica :)
// TODO: Mejorar el formato de salida en consola.

int main()
{
    static double velocidadesNumericas[PASOS_MAXIMOS];
    static double velocidades[PASOS_MAXIMOS];
    static double posiciones[PASOS_MAXIMOS];
    static double tiempos[PASOS_MAXIMOS];

#ifdef _WIN32
    system("cls"); // Limpiamos la consola en Windows
#else
    system("clear"); // Limpiamos la consola en sistemas Unix
#endif

    /**
     * Sección de ingreso de datos por parte del usuario.
     *
     * Intamos validar cada entrada para asegurarnos de que el programa no falle por datos incorrectos
     * Además de darle un formato amigable para que el usuario pueda entender qué se le está pidiendo
     * y cómo debe ingresar los datos.
     */
    printf("-------------------------------------------");
    printf("\nSimulador MRUA\n");
    printf("-------------------------------------------\n");
    printf("Seleccione el escenario a simular:\n");
    printf("1. Caída Libre\n");
    printf("2. Tiro Vertical Hacia Arriba\n");
    printf("\nIngrese su opción: ");

    /**
     * Validación general de la entrada del usuario :)
     *
     * - scanf: Retorna 1 si leyó un número correctamente.
     * - Condición: Si scanf falla OR el número no es 1, pide el dato de nuevo.
     * - Limpieza de Búfer: El segundo "while" extrae del sistema cualquier texto
     *     sobrante que el usuario haya escrito por error (ej. "abc"), dejando la
     *     entrada limpia para el siguiente intento.
     *
     * Eso es un pequeño resumen de cómo funciona este validación que usaremos por cada dato.
     */
    int opcion;
    while (scanf("%d", &opcion) != 1 || (opcion != 1 && opcion != 2))
    {
        printf("Entrada inválida. Por favor, ingrese 1 para Caída Libre o 2 para Tiro Vertical: ");
        while (getchar() != '\n');
    }

    double aceleracion;
    printf("Ingrese la aceleración en m/s^2 (ingrese 0 para la gravedad terrestre): ");
    while (scanf("%lf", &aceleracion) != 1 || aceleracion > 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número para la aceleración (negativo): ");
        while (getchar() != '\n');
    }

    // Generamos un valor por defecto para la aceleración si el usuario ingresa 0, asumiendo que se trata de una caída libre bajo la gravedad terrestre.
    if (aceleracion == 0)
        aceleracion = GRAVEDAD_TERRESTRE;

    // Si el usuario seleccionó la opción de tiro vertical pedimos la velocidad inicial, sino entoces lo dejamos en 0 para la caída libre.
    double velocidadInicial = 0;
    if (opcion != 1)
    {
        printf("Ingrese la velocidad inicial en m/s: ");
        while (scanf("%lf", &velocidadInicial) != 1)
        {
            printf("Entrada inválida. Por favor, ingrese un número para la velocidad inicial: ");
            while (getchar() != '\n');
        }
    }

    double posicionInicial;
    printf("Ingrese la posición inicial en metros: ");
    while (scanf("%lf", &posicionInicial) != 1 || posicionInicial < 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para la posición inicial: ");
        while (getchar() != '\n');
    }

    double tiempoSimulacion;
    printf("Ingrese el tiempo total de simulación en segundos: ");
    while (scanf("%lf", &tiempoSimulacion) != 1 || tiempoSimulacion <= 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para el tiempo de simulación: ");
        while (getchar() != '\n');
    }

    double tiempoPaso;
    printf("Ingrese el tiempo de paso para la simulación en segundos: ");
    while (scanf("%lf", &tiempoPaso) != 1 || tiempoPaso >= tiempoSimulacion || tiempoPaso <= 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para el tiempo de paso: ");
        while (getchar() != '\n');
    }

    // Volvemos a limpiar la consola antes de mostrar los resultados de la simulación
#ifdef _WIN32
    system("cls");  // Limpiamos la consola en Windows
#else
    system("clear");// Limpiamos la consola en sistemas Unix
#endif

    /**
     * Calculamos el número de pasos que se realizarán en la simulación o el numero de veces que se repetirá el ciclo
     * Esto para que sea nuestro limite en el ciclo for y no excedamos el tamaño de los arreglos. Tambien tenemos una
     * condicional para asegurarnos de que el número de pasos no exceda un límite máximo definido por PASOS_MAXIMOS.
     *
     * Aunque sigo pensando que hubiera sido mejor usar malloc...
     */
    int numPasos = (int)(tiempoSimulacion / tiempoPaso);
    if (numPasos > PASOS_MAXIMOS)
    {
        printf("El número de pasos excede el máximo permitido (%d). Ajustando a %d pasos.\n", PASOS_MAXIMOS, PASOS_MAXIMOS);
        numPasos = PASOS_MAXIMOS;
    }

    // Si el usuario seleccionó la opción de tiro vertical hacia arriba, calculamos el tiempo para alcanzar la altura máxima y la altura máxima alcanzada.
    if (velocidadInicial > 0 && aceleracion < 0)
    {
        printf("\nTu movimiento es tiro vertical hacia arriba.\n");

        double tiempoAlturaMaxima = -velocidadInicial / aceleracion;
        double alturaMaxima = posicionInicial + velocidadInicial * tiempoAlturaMaxima + 0.5 * aceleracion * tiempoAlturaMaxima * tiempoAlturaMaxima;

        printf("Tiempo para alcanzar la altura máxima: " DECIMALES_CSV " segundos\n", tiempoAlturaMaxima);
        printf("Altura máxima alcanzada: " DECIMALES_CSV " metros\n", alturaMaxima);
    }
    else
    {
        printf("\nTu movimiento es caída libre.\n");
    }

    /**
     * En este ciclo for calculamos la posición y velocidad en cada paso de tiempo usando las fórmulas muy conocidas de MRUA
     *
     * También verificamos si la posición es menor que 0, lo que quiere decir que el objeto ha tocado el suelo. Si es el caso,
     * le avisamos al usuario y ajustamos detenemos la simulación para que no siga calculando posiciones y velocidades.
     */
    for (int i = 0; i < numPasos; i++)
    {
        tiempos[i] = i * tiempoPaso;
        velocidades[i] = velocidadInicial + aceleracion * tiempos[i];
        posiciones[i] = posicionInicial + velocidadInicial * tiempos[i] + 0.5 * aceleracion * tiempos[i] * tiempos[i];

        if (posiciones[i] < 0)
        {
            printf("El objeto ha tocado el suelo en el tiempo " DECIMALES_CSV ".\n", tiempos[i]);

            numPasos = i + 1;

            break;
        }
    }

    /**
     * Este ciclo es para calcular la velocidad númerica usando la derivada numérica de la posición
     * Ademas de comprobar si es que conciden las velocidades con una tolerancia. Y se preguntaran.
     *
     * porque se hace esto, pues es que por como menejan los flotantes siempre va a ver un margen de error.
     * Por ejemplo para las maquinas el 0.1 en binario es como para nosotros el 1/3 o sea 0.3333... y la maquina
     * no puede guardar decimales infinitos por lo que redondea a un número cercano.
     */
    for (int i = 0; i < numPasos - 1; i++)
    {
        velocidadesNumericas[i] = (posiciones[i + 1] - posiciones[i]) / (tiempoPaso);
    }

    /**
     * Calculamos la velocidad numérica para el último paso porque en el ciclo anterior solo calculamos hasta numPasos - 1
     * esto para evitar acceder a posiciones fuera del arreglo.
     */
    velocidadesNumericas[numPasos - 1] = velocidades[numPasos - 1];

    // Finalmente llegamos a la parte de guardar los resultados en un archivo
    printf("\nSimulación completada. Guardando resultados en " ARCHIVO_RESULTADOS "...\n");

    // Abrimos el archivo en modo escritura, si no se puede abrir mostramos un mensaje de error y salimos del programa
    FILE *archivoCSV = fopen(ARCHIVO_RESULTADOS, "w");
    if (archivoCSV == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo.\n");
        return EXIT_FAILURE;
    }

    // Escribimos la cabecera del archivo CSV y luego los datos de tiempo, velocidad y posición en cada paso de la simulación
    fprintf(archivoCSV, "Tiempo,Velocidad,Posición,Velocidad Numérica, Diferencia Vel. vs Vel. Num.\n");
    for (int i = 0; i < numPasos; i++)
    {
        fprintf(archivoCSV, DECIMALES_CSV "," DECIMALES_CSV "," DECIMALES_CSV "," DECIMALES_CSV "," DECIMALES_CSV "\n", tiempos[i], velocidades[i], posiciones[i], velocidadesNumericas[i], fabs(velocidadesNumericas[i] - velocidades[i]));
    }

    // Cerramos el archivo después de escribir los datos
    fclose(archivoCSV);

    // Imprimimos una vista previa en la consola
    printf("\nResumen de la simulación en consola:\n");
    printf("%-12s | %-12s | %-12s | %-15s | %-12s\n", "Tiempo (s)", "Vel. (m/s)", "Pos. (m)", "Vel. Num. (m/s)", "Diferencia Vel. vs Vel. Num.");
    printf("----------------------------------------------------------------\n");

    if (numPasos <= 20)
    {
        // Si hay 20 pasos o menos, imprimimos la tabla completa para no omitir datos
        for (int i = 0; i < numPasos; i++)
        {
            printf("%-12.4lf | %-12.4lf | %-12.4lf | %-15.4lf | %-12.4lf\n", tiempos[i], velocidades[i], posiciones[i], velocidadesNumericas[i], fabs(velocidadesNumericas[i] - velocidades[i]));
        }
    }
    else
    {
        // Imprimimos los primeros 10 pasos
        for (int i = 0; i < 10; i++)
        {
            printf("%-12.4lf | %-12.4lf | %-12.4lf | %-15.4lf | %-12.4lf\n", tiempos[i], velocidades[i], posiciones[i], velocidadesNumericas[i], fabs(velocidadesNumericas[i] - velocidades[i]));
        }

        // Indicador visual de que hay datos omitidos
        printf("...          | ...          | ...          | ...             | ...          \n");

        // Imprimimos los últimos 10 pasos
        for (int i = numPasos - 10; i < numPasos; i++)
        {
            printf("%-12.4lf | %-12.4lf | %-12.4lf | %-15.4lf | %-12.4lf\n", tiempos[i], velocidades[i], posiciones[i], velocidadesNumericas[i], fabs(velocidadesNumericas[i] - velocidades[i]));
        }
    }
    printf("----------------------------------------------------------------\n");

    // Aviso importante para el usuario
    printf("Nota: Esta es una vista previa. Para analizar los %d pasos con detalle,\n", numPasos);
    printf("por favor revisa el archivo generado: '%s'.\n\n", ARCHIVO_RESULTADOS);

    // Informamos al usuario que los resultados se han guardado correctamente
    return EXIT_SUCCESS; // Si preguntan por qué EXIT_SUCCESS, es una macro que representa un valor de retorno exitoso para el programa es más legible que retornar 0 directamente.
}
