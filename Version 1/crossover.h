//Partially Mapped Crossover
void Crossover(Chromo *parents, Chromo *population, int N, int np)
{
    //hijos
    int *c1 = (int *)malloc(sizeof(int) * N);
    int *c2 = (int *)malloc(sizeof(int) * N);
    //padres
    int *p1, *p2;
    int flag1;
    int flag2;
    int pos1, pos2;
    int k = N / 3;
    int posnp = np;

    for (int n = 0; (n + 1) < np; n = n + 2)
    {

        //inicializo los hijos
        for (int m = 0; m < N; m++)
        {
            c1[m] = -1;
            c2[m] = -1;
        }
        flag1 = 0;
        flag2 = 0;

        //inicializo los padres

        p1 = parents[n].config;
        p2 = parents[n + 1].config;

        for (int a = k; a < (N - k); a++)
        {
            c1[a] = p2[a];
            c2[a] = p1[a];
        }

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
}