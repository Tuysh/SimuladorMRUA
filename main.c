#include <stdio.h>
#include <stdlib.h>

int main()
{
    float aceleracion, velocidadInicial, posicionInicial, tiempoAlturaMaxima, alturaMaxima;
    float tiempoSimulacion, tiempoPaso;  

    // Calculos para el MRUA
    printf("Ingrese la aceleración (en m/s^2): ");
    scanf("%f", &aceleracion);

    printf("Ingrese la velocidad inicial (en m/s): ");
    scanf("%f", &velocidadInicial);

    printf("Ingrese la posición inicial (en m): ");
    scanf("%f", &posicionInicial);

    do
    {
        printf("Ingrese el tiempo de simulación (en segundos): ");
        scanf("%f", &tiempoSimulacion);
    } while (tiempoSimulacion <= 0);

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

        return 1;
    }

    /* ========= Tiro vertical hacia arriba ========= */
    if (velocidadInicial > 0 && aceleracion < 0)
    {
        printf("\nTu movimiento es tiro vertical hacia arriba.\n");
        
        tiempoAlturaMaxima = -velocidadInicial / aceleracion;
        alturaMaxima = posicionInicial + velocidadInicial * tiempoAlturaMaxima + 0.5 * aceleracion * tiempoAlturaMaxima * tiempoAlturaMaxima;

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
    }

    // Archivo CSV
    FILE *file = fopen("datos.csv", "w");

    if (file == NULL)
    {
        printf("Error al abrir el archivo.\n");

        free(tiempos);
        free(velocidades);
        free(posiciones);
        free(velocidadesNumerica);
        return 1;
    }

    fprintf(file, "Tiempo,Velocidad,Posición,Velocidad Numérica\n");
    for (int i = 0; i < numPasos; i++)
    {
        fprintf(file, "%.2f,%.2f,%.2f,%.2f\n", tiempos[i], velocidades[i], posiciones[i], velocidadesNumerica[i]);
    }

    fclose(file);

    free(tiempos);
    free(velocidades);
    free(posiciones);
    free(velocidadesNumerica);

    return 0;
}