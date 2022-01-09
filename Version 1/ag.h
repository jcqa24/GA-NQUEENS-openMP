int algoritmoGenetico(int N, int p, int np, Chromo *Best, int prob, int numMaxGen, clock_t start)
{

    int numthreads = 5;
    omp_set_num_threads(numthreads);
    int posminlocal;
    int countGen = 0; // Contador de Generaciones
    Chromo *parents = (Chromo *)malloc(sizeof(Chromo) * np);
    Chromo *population = (Chromo *)malloc(sizeof(Chromo) * p);
    reservaMemoria(population, parents, p, np, N);

    int inicio = 0, fin = np;

    int Bestfitness = 100000;

    // printf("Incio: %d Fin: %d\n",inicio,fin);

    // Generamos la poblacion incial
    InitConf(population, N, inicio, fin); // check

    // Calculamos el fit de la poblacion inicial
    calFit(population, N, inicio, fin); // check

    posminlocal = BuscaMin(population, inicio, fin);

    // Insertion_sort(population, p);
    // if (idthread == 0)

    if (population[posminlocal].fitness < Bestfitness)
    {
        copyBest(Best, population[posminlocal], N);
        Bestfitness = population[posminlocal].fitness;
    }

    // Caso donde se encuentra un optimo en la primer generacion

    // En caso contrario se guarda el candidato mas optimo

    while ((Bestfitness > 0) && (countGen < numMaxGen))
    {

        // Seleccion de padres
        selectChampionship(parents, population, N, p); // check
        // Cruza
        Crossover(parents, population, N, 0, np); // check

        // Mutacion

        mutation(population, prob, N, inicio, fin);

        // Calculo del Fit
        calFit(population, N, inicio, fin);

        // Ordenamos
        // Insertion_sort(population, p);
        posminlocal = BuscaMin(population, inicio, fin);

        if (population[posminlocal].fitness < Bestfitness)
        {
            copyBest(Best, population[posminlocal], N);
            Bestfitness = population[posminlocal].fitness;
        }

        countGen++;
    }

    return countGen;
}
