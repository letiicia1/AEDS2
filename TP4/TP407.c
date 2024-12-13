#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100 
#define TAMANHO 801
int comparacoes = 0;
int movimentacoes = 0;

// Estrutura de data
typedef struct {
    int dia;
    int mes;
    int ano;
} Date;

// Estrutura do Pokemon
typedef struct {
    int id;
    int generation;
    char name[MAX];
    char description[MAX];
    char types[2][MAX];
    char abilities[20][MAX];
    double weight;
    double height;
    int captureRate;
    bool isLegendary;  
    Date captureDate;
} Pokemon;

Pokemon* newPokemon() {
    return (Pokemon*) malloc(sizeof(Pokemon));
}

void delPokemon(Pokemon* p) {
    free(p);
}

// Função para procurar um Pokémon pelo ID (otimizada)
Pokemon* procurar(Pokemon** pokemons, int id) {
    for (int i = 0; i < TAMANHO; i++) {
        if (pokemons[i] && pokemons[i]->id == id) {
            return pokemons[i];
        }
    }
    return NULL;
}

// Função para trocar vírgulas fora de aspas por ponto e vírgula
void trocarVirgula(char* Line) {
    bool aspas = false;
    for (int i = 0; i < strlen(Line); i++) {
        if (!aspas && Line[i] == ',') {
            Line[i] = ';';
        } else if (Line[i] == '"') {
            aspas = !aspas;
        }
    }
}

void printar(Pokemon* pokemon) {
    printf("[#%d -> %s: %s - ", pokemon->id, pokemon->name, pokemon->description);

    if (strcmp(pokemon->types[1], " ") == 0) {
        printf("['%s'] - ", pokemon->types[0]);
    } else {
        printf("['%s', '%s'] - ", pokemon->types[0], pokemon->types[1]);
    }

    printf("[");

    for (int i = 0; strlen(pokemon->abilities[i]) > 0; i++) 
	{
        printf("'%s'", pokemon->abilities[i]);

        if (strlen(pokemon->abilities[i + 1]) > 0) {
            printf(", ");
        }
    }

    printf("] - %.1lfkg - %.1lfm - %d%% - %s - %d gen] - %02d/%02d/%d\n", 
           pokemon->weight, 
           pokemon->height, 
           pokemon->captureRate, 
           pokemon->isLegendary ? "true" : "false", 
           pokemon->generation,
           pokemon->captureDate.dia, 
           pokemon->captureDate.mes, 
           pokemon->captureDate.ano);
}

// Função para ler e configurar os campos do Pokémon a partir da linha do arquivo
Pokemon* lerPokemon(char Line[]) {
    Pokemon* temp = newPokemon();
    char trash[200], abilities[MAX] = " ";
    int legendary; 

    strcpy(temp->types[0], " ");
    strcpy(temp->types[1], " ");
    trocarVirgula(Line);

    sscanf(Line, "%d;%d;%[^;];%[^;];%[^;];%[^;]", 
           &temp->id, &temp->generation, temp->name, temp->description, temp->types[0], temp->types[1]);

    char resto[100];
    sscanf(Line, "%[^[][%[^]]]%s", trash, abilities, resto);

    int m = 0, j = 0;
    char dividir[10][100];
    for (int i = 1; i < strlen(resto); i++) {
        if (resto[i] != ';') {
            dividir[m][j++] = resto[i];
        } else {
            dividir[m++][j] = '\0';
            j = 0;
        }
    }

    sscanf(dividir[1], "%lf", &temp->weight);
    sscanf(dividir[2], "%lf", &temp->height);
    sscanf(dividir[3], "%d", &temp->captureRate);
    sscanf(dividir[4], "%d", &legendary);
    sscanf(dividir[5], "%d/%d/%d", &temp->captureDate.dia, &temp->captureDate.mes, &temp->captureDate.ano);
    temp->isLegendary = (legendary == 1);

    // Limpar aspas das habilidades
    j = 0;
    int tamanhoabilities = strlen(abilities);
    char abilities2[100];
    for (int i = 0; i < tamanhoabilities; i++) {
        if (abilities[i] != '\'') {
            abilities2[j++] = abilities[i];
        }
    }
    abilities2[j] = '\0';

    // Processar habilidades
    m = 0;
    j = 0;
    for (int i = 0; i < strlen(abilities2); i++) {
        if (abilities2[i] == ',') {
            temp->abilities[j++][m] = '\0';
            m = 0;
            i++;
        } else {
            temp->abilities[j][m++] = abilities2[i];
        }
    }
    temp->abilities[j][m] = '\0';
    return temp;
}

// Função para ler o arquivo CSV e carregar os Pokémons
void lerTodoArquivo(char fileName[], Pokemon* pokemons[]) {
    FILE* arquivo = fopen(fileName, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    int index = 0;
    char Line[200];
    fgets(Line, sizeof(Line), arquivo); // Pular o cabeçalho
    while (fgets(Line, sizeof(Line), arquivo) != NULL && index < TAMANHO) {
        pokemons[index++] = lerPokemon(Line);
    }
    fclose(arquivo);
}


#define NULO -1

// Estrutura para um nó da lista
typedef struct Celula {
    Pokemon* elemento;
    struct Celula *prox;
} Celula;

// Estrutura para a lista encadeada
typedef struct {
    Celula *primeiro;
    Celula *ultimo;
} Lista;

// Estrutura para a tabela hash
typedef struct {
    Lista *tabela;
    int tamanho;
} HashIndiretoLista;

// Funções da Lista
Lista* criarLista() {
    Lista *l = (Lista *)malloc(sizeof(Lista));
    l->primeiro = (Celula *)malloc(sizeof(Celula));
    l->primeiro->prox = NULL;
    l->ultimo = l->primeiro;
    return l;
}

int pesquisarLista(Lista *l, char elemento[]) {

    int res = -1; int j = 0;
    for (Celula *i = l->primeiro->prox; i != NULL; i = i->prox) {
        j++;
        comparacoes++;
        if (strcmp(i->elemento->name, elemento) == 0 ) {
            res = j;
        }
    }
    return res;
}

void inserirFimLista(Lista *l, Pokemon* elemento) {
     Celula *tmp = (Celula *)malloc(sizeof(Celula));
    tmp->elemento = elemento;
    tmp->prox = NULL; 

    l->ultimo->prox = tmp;
    l->ultimo = tmp;
    }



// Funções da Hash Indireta
HashIndiretoLista* criarHash(int tamanho) {
    HashIndiretoLista *h = (HashIndiretoLista *)malloc(sizeof(HashIndiretoLista));
    h->tamanho = tamanho;
    h->tabela = (Lista *)malloc(tamanho * sizeof(Lista));
    for (int i = 0; i < tamanho; i++) {
        h->tabela[i] = *criarLista();
    }
    return h;
}

int hash(int elemento, int tamanho) {
    return elemento % tamanho;
}


    int calculaSomaASCII(char nome[]) {
        int soma = 0;
        for (int i = 0; i < strlen(nome); i++) {
            soma += nome[i];
        }
        return soma;
    }
int pesquisarHash(HashIndiretoLista *h, char elemento[]) {
    int num = calculaSomaASCII(elemento);
    
    int pos = hash(num, h->tamanho);
    if (pesquisarLista(&h->tabela[pos], elemento) == -1)
    {
        pos = -1;
    } 

    return pos;
}

void inserirHash(HashIndiretoLista *h, Pokemon* elemento) {
    int num = calculaSomaASCII(elemento->name);
    int pos = hash(num, h->tamanho);
    inserirFimLista(&h->tabela[pos], elemento);
}



// Função para liberar memória dos Pokémons
void freeAllPokemons(Pokemon* pokemons[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (pokemons[i] != NULL) {
            free(pokemons[i]);
            pokemons[i] = NULL;  
        }
    }
}


int main(void) {
    Pokemon* pokemons[TAMANHO] = {0}; // Inicializa array de Pokémons
    char entrada[20];
    HashIndiretoLista* h = criarHash(21); 

    // Ler o arquivo CSV
    lerTodoArquivo("/tmp/pokemon.csv", pokemons);

    // Processa IDs de entrada e insere na AVL
    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        int idP;
        sscanf(entrada, "%d", &idP);
        Pokemon* encontrado = procurar(pokemons, idP);

        if (encontrado != NULL) {
           inserirHash(h, encontrado);
        }
    }

    // Pesquisa elementos na AVL
    clock_t inicio = clock();
    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        printf("=> %s: ", entrada);
        int res = pesquisarHash(h, entrada);
        if(res != -1)
        {
            printf("(Posicao: %d) SIM\n" , res);
        }
        else
        {
             printf("NAO\n");
        }
    }
    clock_t fim = clock();

    // Log do tempo de execução
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
    FILE* log = fopen("840757_hashIndireta.txt", "w");
    if (log) {
        fprintf(log, "840757\t%d\t%.2f\n", comparacoes, tempoExecucao);
        fclose(log);
    }

 
    freeAllPokemons(pokemons, TAMANHO);

    return 0;
}
