int algoritmoGenetico(int N, int p, int np, Chromo *Best, int prob, int numMaxGen, clock_t start)
{

    int numthreads = 5;
    omp_set_num_threads(numthreads);
    int posminlocal;
    int countGen = 0; // Contador de Generaciones
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
        // printf("Incio: %d Fin: %d\n",inicio,fin);

        // Generamos la poblacion incial
        InitConf(population, N, inicio, fin); // check

        // Calculamos el fit de la poblacion inicial
        calFit(population, N, inicio, fin); // check

        posminlocal = BuscaMin(population, inicio, fin);

// Insertion_sort(population, p);
// if (idthread == 0)
//{
#pragma omp critical
        {
            if (population[posminlocal].fitness < Bestfitness)
            {
                copyBest(Best, population[posminlocal], N);
                Bestfitness = population[posminlocal].fitness;
            }
        }
        // Caso donde se encuentra un optimo en la primer generacion

        // En caso contrario se guarda el candidato mas optimo

        //  }

        while ((Bestfitness > 0) && (countGen < numMaxGen))
        {

            if (idthread == 0)
            {
                // Seleccion de padres
                selectChampionship(parents, population, N, p); // check
                // Cruza
                Crossover(parents, population, N,0 ,np); // check
            }

#pragma omp barrier
            // Mutacion

            mutation(population, prob, N, inicio, fin);

            // Calculo del Fit
            calFit(population, N, inicio, fin);

            // Ordenamos
            // Insertion_sort(population, p);
            posminlocal = BuscaMin(population, inicio, fin);

#pragma omp critical
            {
                if (population[posminlocal].fitness < Bestfitness)
                {
                    copyBest(Best, population[posminlocal], N);
                    Bestfitness = population[posminlocal].fitness;
                }
            }

            if (idthread == 0)
            {

                countGen++;
            }

#pragma omp barrier
        }
    }
    return countGen;
}
