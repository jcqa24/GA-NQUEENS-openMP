#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

#include "definitions.h"
#include "initialization.h"
#include "crossover.h"

void ip(Chromo *population, int p, int N)
{
    for (int j = 0; j < p; j++)
    {
        printf("Gen: %d [ ", j);
        for (int i = 0; i < N; i++)
        {
            printf(" %d ", population[j].config[i] + 1);
        }
        printf("] fitnes ==> %d \n", population[j].fitness);
    }
}

void Insertion_sort(Chromo *population, int p)
{

    int i, j;
    Chromo temp;

    for (i = 1; i < p; i++)
    {
        temp = population[i];
        for (j = i; j > 0 && population[j - 1].fitness > temp.fitness; j--)
        {
            population[j] = population[j - 1];
        }
        population[j] = temp;
    }
}



void shuffle(Chromo *population, int p, int N)
{
    int i, j;
    Chromo temp;
    temp.config = (int *)malloc(sizeof(int) * N);
    int c = 0;                     //contador de cambios;
    int nc = rand() % (p - 2) + 2; //numero de cambios
    while (c <= nc)
    {

        i = rand() % (p);
        j = rand() % (p);
        if (j != i)
        {
            temp = population[i];
            population[i] = population[j];
            population[j] = temp;
            c++;
        }
    }
}

int BuscaMin(Chromo *population, int p)
{
    int i, pos = 0;

    for (i = 1; i < p; i++)
    {

        if (population[i].fitness < population[pos].fitness)
        {
            pos = i;
        }
    }

    return pos;
}



void selectChampionship(Chromo *parents, Chromo *population, int N, int p)
{

    int j, i, c = 0;

    //shuffle(population, p, N); // check

    for (i = 0; i < p; i = i + 2)
    {
        if (population[i].fitness <= population[i + 1].fitness)
        {
            //parents[c] = population[i];
            for (j = 0; j < N; j++)
            {
                parents[c].config[j] = population[i].config[j];
            }
        }
        else
        {
            //parents[c] = population[i + 1];
            for (j = 0; j < N; j++)
            {
                parents[c].config[j] = population[i + 1].config[j];
            }
        }
        c++;
    }
}

void printConf(int *conf, int N)
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (conf[j] == i)
            {
                printf(" %c ", 'X');
            }
            else
            {
                printf(" %c ", '_');
            }
        }
        printf("\n");
    }
}
void calFit(Chromo *population, int N, int p)
{
    int errores;
    int k, i, j;
    int dx, dy;

    for (k = 0; k < p; k++)
    {
        errores = 0;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (i != j)
                {
                    dx = i-j;
                    dy = population[k].config[j] -population[k].config[i];
                    if (abs(dx) == abs(dy))
                    {
                        errores++;
                    }
                }
            }
        }

        population[k].fitness = errores;
    }
}

void mutation(Chromo *population, int prob, int N, int p)
{
    int aux, i, p1 = 0, p2 = 0;
#pragma omp parallel for num_threads(4) private(aux, p1, p2)
    for (i = 0; i < p; i++)
    {
        srand(time(NULL));
        if (rand() % (101) <= prob)
        {
            do
            {
                p1 = rand() % (N + 1);
                p2 = rand() % (N + 1);
            } while (p1 == p2);
            aux = population[i].config[p1];
            population[i].config[p1] = population[i].config[p2];
            population[i].config[p2] = aux;
        }
    }
}

void copyBest(Chromo *Best, Chromo local, int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        Best->config[i] = local.config[i];
    }
    Best->fitness = local.fitness;
}



void confFinal(Chromo Best, int N, clock_t start)
{
    printf("\n\n=============================================\n");
    printf("La mejor solucion tiene un fit de %d \n", Best.fitness);
    printf("La mejor solucion es: [ ");
    for (int i = 0; i < N; i++)
    {
        printf(" %d ", Best.config[i] + 1);
    }
    printf("]\n");
    printConf(Best.config, N);
    printf("\n===============================================\n");

    printf("Tiempo transcurrido: %2.10f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
}

int algoritmoGenetico(int N, int p, int np, Chromo *Best, int prob, int numMaxGen, clock_t start)
{

    int posmin;
    int countGen = 0; //Contador de Generaciones
    Chromo *parents = (Chromo *)malloc(sizeof(Chromo) * np);
    Chromo *population = (Chromo *)malloc(sizeof(Chromo) * p);
    reservaMemoria(population, parents, p, np, N);

    //Generamos la poblacion incial
    InitConf(population, N, p); //check

    //Calculamos el fit de la poblacion inicial
    calFit(population, N, p); //check

    posmin = BuscaMin(population, p);

    //Caso donde se encuentra un optimo en la primer generacion
    if (population[posmin].fitness == 0){
        confFinal(population[posmin], N, start);
        return 1;
    }else
    {
        // En caso contrario se guarda el candidato mas optimo
        copyBest(Best, population[0], N);

        do
        {
            //Seleccion de padres
            selectChampionship(parents, population, N, p); //check

            //Cruza
            Crossover(parents, population, N, np); //check

            //Mutacion
            mutation(population, prob, N, np);

            //Calculo del Fit
            calFit(population, N, p);

            //Ordenamos
            //Insertion_sort(population, p);

            posmin = BuscaMin(population, p);

            //Comprobamos si hay un mejor candidato

            if (population[posmin].fitness <= Best->fitness)
                copyBest(Best, population[posmin], N);

            countGen++;

        } while ((Best->fitness != 0) && (countGen < numMaxGen));
    }
    return countGen;
}

int main()
{
    srand(time(NULL));

    int N = 8;            // reinas
    int p = 100;           //poplacion incial
    int np = p / 2;        // numero de padres
    int prob = 10;         //probabilidad de mutacion
    int numMaxGen = 10000; // Numero Maximo de Generaciones

    Chromo Best;
    Best.fitness = 0;
    Best.config = (int *)malloc(sizeof(int) * N);

    printf("Agoritmo genetico para N reinias \n");
    printf("Numero de Reinas -> %d\n", N);
    printf("Poblacion inicial -> %d\n", p);

    clock_t start = clock();

    algoritmoGenetico(N, p, np, &Best, prob, numMaxGen, start);

    confFinal(Best, N, start);

    return 0;
}
