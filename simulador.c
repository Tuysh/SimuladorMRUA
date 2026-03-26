#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOLERANCIA 0.1
#define ARCHIVO_RESULTADOS "datos.csv"

int main()
{
    float aceleracion;
    printf("Ingrese la aceleración (en m/s^2): ");
    while (scanf("%f", &aceleracion) != 1)
    {
        printf("Entrada inválida. Por favor, ingrese un número para la aceleración: ");
        while (getchar() != '\n');
    }

    float velocidadInicial;
    printf("Ingrese la velocidad inicial (en m/s): ");
    while (scanf("%f", &velocidadInicial) != 1)
    {
        printf("Entrada inválida. Por favor, ingrese un número para la velocidad inicial: ");
        while (getchar() != '\n');
    }

    float posicionInicial;
    printf("Ingrese la posición incial (en m): ");
    while (scanf("%f", &posicionInicial) != 1 || posicionInicial < 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para la posición inicial: ");
        while (getchar() != '\n');
    }

    float tiempoSimulacion;
    printf("Ingrese el tiempo total de simulación (en segundos): ");
    while (scanf("%f", &tiempoSimulacion) != 1 || tiempoSimulacion <= 0)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para el tiempo de simulación: ");
        while (getchar() != '\n');
    }

    float tiempoPaso;
    printf("Ingrese el tiempo de paso para la simulación (en segundos): ");
    while (scanf("%f", &tiempoPaso) != 1 || tiempoPaso >= tiempoSimulacion)
    {
        printf("Entrada inválida. Por favor, ingrese un número positivo para el tiempo de paso: ");
        while (getchar() != '\n');
    }

    int numPasos = (int)(tiempoSimulacion / tiempoPaso);

    float *velocidadesNumerica = malloc(numPasos * sizeof(float));
    float *velocidades = malloc(numPasos * sizeof(float));
    float *posiciones = malloc(numPasos * sizeof(float));
    float *tiempos = malloc(numPasos * sizeof(float));

    if (velocidadesNumerica == NULL || velocidades == NULL || posiciones == NULL || tiempos == NULL)
    {
        fprintf(stderr, "Error al asignar memoria.\n");
        free(velocidadesNumerica);
        free(velocidades);
        free(posiciones);
        free(tiempos);

        return EXIT_FAILURE;
    }

    if (velocidadInicial > 0 && aceleracion < 0)
    {
        printf("\nTu movimiento es tiro vertical hacia arriba.\n");

        float tiempoAlturaMaxima = -velocidadInicial / aceleracion;
        float alturaMaxima = posicionInicial + velocidadInicial * tiempoAlturaMaxima + 0.5 * aceleracion * tiempoAlturaMaxima * tiempoAlturaMaxima;

        printf("Tiempo para alcanzar la altura máxima: %.4f segundos\n", tiempoAlturaMaxima);
        printf("Altura máxima alcanzada: %.4f metros\n", alturaMaxima);
    }

    for (int i = 0; i < numPasos; i++)
    {
        tiempos[i] = i * tiempoPaso;
        velocidades[i] = velocidadInicial + aceleracion * tiempos[i];
        posiciones[i] = posicionInicial + velocidadInicial * tiempos[i] + 0.5 * aceleracion * tiempos[i] * tiempos[i];
    }

    for (int i = 0; i < numPasos - 1; i++)
    {
        velocidadesNumerica[i] = (posiciones[i + 1] - posiciones[i]) / tiempoPaso;

        if (fabs(velocidadesNumerica[i] - velocidades[i]) > TOLERANCIA)
        {
            printf("Diferencia significativa en el paso %d: Velocidad analítica = %.4f, Velocidad numérica = %.4f\n", i, velocidades[i], velocidadesNumerica[i]);
        }
    }

    int ultimo = numPasos - 1;
    velocidadesNumerica[ultimo] = velocidades[ultimo];

    FILE *archivoCSV = fopen(ARCHIVO_RESULTADOS, "w");
    if (archivoCSV == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo.\n");
        free(tiempos);
        free(velocidades);
        free(posiciones);
        free(velocidadesNumerica);
        return EXIT_FAILURE;
    }

    fprintf(archivoCSV, "Tiempo,Velocidad,Posición,Velocidad Numérica\n");
    for (int i = 0; i < numPasos; i++)
    {
        fprintf(archivoCSV, "%.4f,%.4f,%.4f,%.4f\n", tiempos[i], velocidades[i], posiciones[i], velocidadesNumerica[i]);
    }

    fclose(archivoCSV);

    free(tiempos);
    free(velocidades);
    free(posiciones);
    free(velocidadesNumerica);

    return EXIT_SUCCESS;
}
