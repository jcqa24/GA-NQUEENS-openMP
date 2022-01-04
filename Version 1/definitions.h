#if !defined(DEFINITIONS_H_)
#define DEFINITIONS_H_

struct Chromosome
{
    int fitness;
    int *config;
};
typedef struct Chromosome Chromo;

void ip(Chromo *, int, int);
void Insertion_sort(Chromo *, int);
void InitConf(Chromo *, int, int);
void shuffle(Chromo *, int, int);
int BuscaMin(Chromo *, int);
void Crossover(Chromo *, Chromo *, int, int);
void selectChampionship(Chromo *, Chromo *, int, int);
void printConf(int *, int);
void calFit(Chromo *, int, int);
void mutation(Chromo *, int, int, int);
void copyBest(Chromo *, Chromo, int);
void reservaMemoria(Chromo *, Chromo *, int, int, int);
void confFinal(Chromo, int, clock_t);
int algoritmoGenetico(int, int, int, Chromo *, int, int, clock_t);

#endif // DEFINITIONS_H_
