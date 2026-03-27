#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOLERANCIA 5
#define ARCHIVO_RESULTADOS "datos.csv"
#define DECIMALES_CSV "%.4lf"
#define PASOS_MAXIMOS 1000000
#define GRAVEDAD_TERRESTRE -9.81

double velocidadesNumericas[PASOS_MAXIMOS];
double velocidades[PASOS_MAXIMOS];
double posiciones[PASOS_MAXIMOS];
double tiempos[PASOS_MAXIMOS];

int main()
{
    printf("--- Simulador de Movimiento Rectilíneo Uniformemente Acelerado (MRUA) ---\n");
    printf("Seleccione el escenario a simular:\n");
    printf("1. Caída Libre\n");
    printf("2. Tiro Vertical Hacia Arriba\n");
    printf("Ingrese su opción: ");

    int opcion;
    while (scanf("%d", &opcion) != 1 || (opcion != 1 && opcion != 2))
    {
        printf("Entrada inválida. Por favor, ingrese 1 para Caída Libre o 2 para Tiro Vertical: ");
        while (getchar() != '\n');
    }

    float aceleracion;
    printf("Ingrese la aceleración en m/s^2 (ingrese 0 para la gravedad terrestre): ");
    while (scanf("%f", &aceleracion) != 1)
    {
        printf("Entrada inválida. Por favor, ingrese un número para la aceleración: ");
        while (getchar() != '\n');
    }

    // Generamos un valor por defecto para la aceleración si el usuario ingresa 0, asumiendo que se trata de una caída libre bajo la gravedad terrestre.
    if (aceleracion == 0)
        aceleracion = GRAVEDAD_TERRESTRE;

    float velocidadInicial = 0;
    if (opcion != 1)
    {
        printf("Ingrese la velocidad inicial en m/s: ");
        while (scanf("%f", &velocidadInicial) != 1)
        {
            printf("Entrada inválida. Por favor, ingrese un número para la velocidad inicial: ");
            while (getchar() != '\n');
        }
    }

    float posicionInicial;
    printf("Ingrese la posición inicial en metros: ");
    while (scanf("%f", &posicionInicial) != 1 || posicionInicial < 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para la posición inicial: ");
        while (getchar() != '\n');
    }

    float tiempoSimulacion;
    printf("Ingrese el tiempo total de simulación en segundos: ");
    while (scanf("%f", &tiempoSimulacion) != 1 || tiempoSimulacion <= 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para el tiempo de simulación: ");
        while (getchar() != '\n');
    }

    float tiempoPaso;
    printf("Ingrese el tiempo de paso para la simulación en segundos: ");
    while (scanf("%f", &tiempoPaso) != 1 || tiempoPaso >= tiempoSimulacion || tiempoPaso <= 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para el tiempo de paso: ");
        while (getchar() != '\n');
    }

    int numPasos = (int)(tiempoSimulacion / tiempoPaso);
    if (numPasos > PASOS_MAXIMOS)
    {
        printf("El número de pasos excede el máximo permitido (%d). Ajustando a %d pasos.\n", PASOS_MAXIMOS, PASOS_MAXIMOS);
        numPasos = PASOS_MAXIMOS;
    }

    if (velocidadInicial > 0 && aceleracion < 0)
    {
        printf("\nTu movimiento es tiro vertical hacia arriba.\n");

        float tiempoAlturaMaxima = -velocidadInicial / aceleracion;
        float alturaMaxima = posicionInicial + velocidadInicial * tiempoAlturaMaxima + 0.5 * aceleracion * tiempoAlturaMaxima * tiempoAlturaMaxima;

        printf("Tiempo para alcanzar la altura máxima: " DECIMALES_CSV " segundos\n", tiempoAlturaMaxima);
        printf("Altura máxima alcanzada: " DECIMALES_CSV " metros\n", alturaMaxima);
    }

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

    velocidadesNumericas[0] = (posiciones[1] - posiciones[0]) / tiempoPaso;

    for (int i = 0; i < numPasos - 1; i++)
    {
        velocidadesNumericas[i] = (posiciones[i + 1] - posiciones[i]) / (tiempoPaso);

        if (fabs(velocidadesNumericas[i] - velocidades[i]) > TOLERANCIA)
        {
            printf("Diferencia significativa en el paso %d: Velocidad analítica = " DECIMALES_CSV ", Velocidad numérica = " DECIMALES_CSV "\n", i, velocidades[i], velocidadesNumericas[i]);
        }
    }

    int ultimo = numPasos - 1;
    if (ultimo > 0)
    {
        velocidadesNumericas[ultimo] = (posiciones[ultimo] - posiciones[ultimo - 1]) / tiempoPaso;
    }

    FILE *archivoCSV = fopen(ARCHIVO_RESULTADOS, "w");
    if (archivoCSV == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo.\n");
        return EXIT_FAILURE;
    }

    fprintf(archivoCSV, "Tiempo,Velocidad,Posición\n");
    for (int i = 0; i < numPasos; i++)
    {
        fprintf(archivoCSV, DECIMALES_CSV "," DECIMALES_CSV "," DECIMALES_CSV "\n", tiempos[i], velocidades[i], posiciones[i]);
    }

    fclose(archivoCSV);

    return EXIT_SUCCESS;
}
