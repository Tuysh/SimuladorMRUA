#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TOLERANCIA 0.1
#define ARCHIVO_RESULTADOS "datos.csv"

int main()
{
    float aceleracion;
    printf("Ingrese la aceleración (en m/s^2): ");
    scanf("%f", &aceleracion);

    float velocidadInicial;
    printf("Ingrese la velocidad inicial (en m/s): ");
    scanf("%f", &velocidadInicial);

    float posicionInicial;
    do
    {
        printf("Ingrese la posición inicial (en m): ");
        scanf("%f", &posicionInicial);
    } while (posicionInicial < 0);

    float tiempoSimulacion;
    do
    {
        printf("Ingrese el tiempo de simulación (en segundos): ");
        scanf("%f", &tiempoSimulacion);
    } while (tiempoSimulacion <= 0);

    float tiempoPaso;
    do
    {
        printf("Ingrese el tiempo de paso (en segundos): ");
        scanf("%f", &tiempoPaso);
    } while (tiempoPaso >= tiempoSimulacion);

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

        printf("Tiempo para alcanzar la altura máxima: %.2f segundos\n", tiempoAlturaMaxima);
        printf("Altura máxima alcanzada: %.2f metros\n", alturaMaxima);
    }

    for (int i = 0; i < numPasos; i++)
    {
        tiempos[i] = i * tiempoPaso;
        velocidades[i] = velocidadInicial + aceleracion * tiempos[i];
        posiciones[i] = posicionInicial + velocidadInicial * tiempos[i] + 0.5 * aceleracion * tiempos[i] * tiempos[i];
    }

    for (int i = 0; i < numPasos; i++)
    {
        if (i < numPasos - 1)
        {
            velocidadesNumerica[i] = (posiciones[i + 1] - posiciones[i]) / tiempoPaso;
        }
        else
        {
            velocidadesNumerica[i] = velocidades[i];
        }

        if (fabs(velocidadesNumerica[i] - velocidades[i]) > TOLERANCIA)
        {
            printf("Diferencia significativa en el paso %d: Velocidad analítica = %.2f, Velocidad numérica = %.2f\n", i, velocidades[i], velocidadesNumerica[i]);
        }
    }

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