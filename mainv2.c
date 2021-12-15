#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <omp.h>

struct Chromosome
{
    int fitness;
    int *config;
};
typedef struct Chromosome Chromo;

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

void InitConf(Chromo *population, int N, int inicio, int fin)
{

    int pos;
    int i, j, k;

    for (k = inicio; k < fin; k++)
    {
        population[k].config = (int *)malloc(sizeof(int) * N);
        for (j = 0; j < N; j++)
        {
            population[k].config[j] = -1;
        }
        i = 0;
        while (i < N)
        {
            pos = rand() % (N); // pos en el arreglo
            if ((population[k].config[pos] == -1) && (population[k].config[pos] != i))
            {
                population[k].config[pos] = i;
                i++;
            }
        }
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

int BuscaMin(Chromo *population, int inicio, int fin)
{
    int i, pos = inicio;

    for (i = inicio + 1; i < fin; i++)
    {

        if (population[i].fitness < population[pos].fitness)
        {
            pos = i;
        }
    }

    return pos;
}

void inicializaCruzaHijos(int *c1, int *c2, int N)
{
    for (int m = 0; m < N; m++)
    {
        c1[m] = -1;
        c2[m] = -1;
    }
}

void inicializaCruzaPadres(int *p1, int *p2, int *ind1, int *ind2, int N)
{
    for (int m = 0; m < N; m++)
    {
        p1[m] = ind1[m];
        p2[m] = ind2[m];
    }
}


void cruzaCopiaMedio(int *c1,int *c2,int *p1,int *p2, int inicio,int fin){
            for (int a = inicio; a < fin; a++)
        {
            c1[a] = p2[a];
            c2[a] = p1[a];
        }
}


//Partially Mapped Crossover
void Crossover(Chromo *parents, Chromo *population, int N, int np)
{

    printf("iniciando cruce\n");
    //hijos
    int *c1 = (int *)malloc(sizeof(int) * N);
    int *c2 = (int *)malloc(sizeof(int) * N);
    //padres
    int *p1 = (int *)malloc(sizeof(int) * N);
    int *p2 = (int *)malloc(sizeof(int) * N);
    int flag1;
    int flag2;
    int pos1, pos2;
    int k = N / 3;
    int posnp = np;

    for (int n = 0; (n + 1) < np; n = n + 2)
    {


        flag1 = 0;
        flag2 = 0;

        printf("incia hijos\n");
        inicializaCruzaHijos(c1, c2, N);

        //inicializo los padres
        printf("incia padres\n");
        inicializaCruzaPadres(p1,p2, parents[n].config, parents[n + 1].config,N);
        cruzaCopiaMedio(c1,c2,p1,p2,k,(N-k));



        for (int a = 0; a < k; a++)
        {
            flag1 = 0;
            flag2 = 0;
            pos1 = p1[a];
            pos2 = p2[a];
            for (int b = k; b < (N - k); b++)
            {
                if (pos1 == c1[b])
                {
                    flag1 = 1;
                }
                if (pos2 == c2[b])
                {
                    flag2 = 1;
                }
            }
            if (!flag1)
            {
                c1[a] = pos1;
            }
            if (!flag2)
            {
                c2[a] = pos2;
            }
        }

        for (int a = (N - k); a < N; a++)
        {
            pos1 = p1[a];
            pos2 = p2[a];
            flag1 = 0;
            flag2 = 0;
            for (int b = k; b < (N - k); b++)
            {
                if (pos1 == c1[b])
                {
                    flag1 = 1;
                }
                if (pos2 == c2[b])
                {
                    flag2 = 1;
                }
            }
            if (!flag1)
            {
                c1[a] = pos1;
            }
            if (!flag2)
            {
                c2[a] = pos2;
            }
        }

        int count, co;
        for (int a = 0; a < N; a++)
        {
            flag1 = 0;
            count = 0;
            co = 0;
            while ((!flag1) && count < N)
            {
                if (a == c1[count])
                {
                    flag1 = 1;
                }
                count++;
            }

            if (!flag1)
            {
                while ((c1[co] != -1) && (co < N))
                {
                    co = rand() % N;
                }
                c1[co] = a;
            }
            flag1 = 0;
            count = 0;
            co = 0;
            while ((!flag1) && count < N)
            {
                if (a == c2[count])
                {
                    flag1 = 1;
                }
                count++;
            }

            if (!flag1)
            {
                while ((c2[co] != -1) && (co < N))
                {
                    co = rand() % N;
                }
                c2[co] = a;
            }
        }

        for (int i = 0; i < N; i++)
        {
            population[posnp].config[i] = c1[i];
            population[posnp + 1].config[i] = c2[i];
        }

        posnp = posnp + 2;
    }
    printf("saliendo cruce\n");
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

void calFit(Chromo *population, int N, int inicio, int fin)
{
    int errores;
    int k, i, j;
    for (k = inicio; k < fin; k++)
    {
        errores = 0;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (i != j)
                {
                    if ((population[k].config[i] - i) == (population[k].config[j] - j))
                    {
                        errores++;
                    }
                    if ((population[k].config[i] + i) == (population[k].config[j] + j))
                    {
                        errores++;
                    }
                }
            }
        }

        population[k].fitness = errores;
    }
}

void mutation(Chromo *population, int prob, int N, int inicio, int fin)
{
    int aux, i, p1 = 0, p2 = 0;
    for (i = inicio; i < fin; i++)
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


void reservaMemoria(Chromo *population, Chromo *parents, int p, int np, int N)
{
    int i;
    for (i = 0; i < p; i++)
    {
        population[i].config = (int *)malloc(sizeof(int) * N);
    }

    for (i = 0; i < np; i++)
    {
        parents[i].config = (int *)malloc(sizeof(int) * N);
    }
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

    int numthreads = 5;
    omp_set_num_threads(numthreads);
    int posminglobal, posminlocal;
    int countGen = 0; //Contador de Generaciones
    Chromo *parents = (Chromo *)malloc(sizeof(Chromo) * np);
    Chromo *population = (Chromo *)malloc(sizeof(Chromo) * p);
    reservaMemoria(population, parents, p, np, N);

    int inicio, fin, idthread;

    int Bestfitness = 100000;
// agregar share
#pragma omp parallel private(idthread, inicio, fin, posminlocal) shared(population, countGen, Bestfitness, Best, parents)
    {
        idthread = omp_get_thread_num();
        inicio = (idthread * (p / numthreads));
        fin = inicio + (p / numthreads);
        //printf("Incio: %d Fin: %d\n",inicio,fin);

        //Generamos la poblacion incial
        InitConf(population, N, inicio, fin); //check

        //Calculamos el fit de la poblacion inicial
        calFit(population, N, inicio, fin); //check

        posminlocal = BuscaMin(population, inicio, fin);
        printf("Posicion local minima %d: \n", posminlocal);
//Insertion_sort(population, p);
//if (idthread == 0)
//{
#pragma omp critical
        {
            if (population[posminlocal].fitness < Bestfitness)
            {
                copyBest(Best, population[posminlocal], N);
                Bestfitness = population[posminlocal].fitness;
            }
        }
        //Caso donde se encuentra un optimo en la primer generacion

        // En caso contrario se guarda el candidato mas optimo

        //  }

        while ((Bestfitness > 0) && (countGen < numMaxGen))
        {

            printf("Mejor fittness global: %d y numero de generaciones %d\n", Bestfitness, countGen);
            if (idthread == 0)
            {
                //Seleccion de padres
                printf("Padres\n");
                selectChampionship(parents, population, N, p); //check

                //Cruza
                printf("Cruza\n");
                Crossover(parents, population, N, np); //check
            }

            printf("\tBarera %d \n", idthread);
#pragma omp barrier
            //Mutacion
            printf("Mutacion\n");
            mutation(population, prob, N, inicio, fin);

            //Calculo del Fit
            printf("Calfit");
            calFit(population, N, inicio, fin);

            //Ordenamos
            //Insertion_sort(population, p);
            printf("Buscamin\n");
            posminlocal = BuscaMin(population, inicio, fin);

            printf("Critical\n");
#pragma omp critical
            {
                if (population[posminlocal].fitness < Bestfitness)
                {
                    copyBest(Best, population[posminlocal], N);
                    Bestfitness = population[posminlocal].fitness;
                }
            }

            printf("Contador\n");

            if (idthread == 0)
            {

                countGen++;
            }

#pragma omp barrier
        }
    }
    return countGen;
}

int main()
{
    srand(time(NULL));

    int N = 10;            // reinas
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