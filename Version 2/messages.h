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





void confFinal(Chromo Best, int N, clock_t start,int numMaxGen)
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

    printf("Numero de generaciones transcurridas: %d\n",numMaxGen);
    printf("Tiempo transcurrido: %2.10f seg.\n", ((double)clock() - start) / CLOCKS_PER_SEC);
}
