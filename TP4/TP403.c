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

typedef struct No {
    Pokemon* elemento; // Conteúdo do nó
    struct No* esq;   // Filho esquerdo
    struct No* dir;   // Filho direito
    int nivel;        // Número de níveis abaixo do nó
} No;

// Função para criar um novo nó
No* newNo(Pokemon* elemento) {
    No* novo = (No*)malloc(sizeof(No));
    novo->elemento = elemento;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->nivel = 1; // Um nó isolado tem nível 1
    return novo;
}

// Função para calcular o número de níveis a partir de um nó
int getNivel(No* no) {
    return (no == NULL) ? 0 : no->nivel;
}

// Função para atualizar o nível de um nó
void setNivel(No* no) {
    if (no != NULL) {
        no->nivel = 1 + (getNivel(no->esq) > getNivel(no->dir) ? getNivel(no->esq) : getNivel(no->dir));
    }
}

// Função para realizar rotação à direita
No* rotacionarDir(No* no) {
    No* noEsq = no->esq;
    No* noEsqDir = noEsq->dir;

    noEsq->dir = no;
    no->esq = noEsqDir;

    setNivel(no);
    setNivel(noEsq);

    return noEsq;
}

// Função para realizar rotação à esquerda
No* rotacionarEsq(No* no) {
    No* noDir = no->dir;
    No* noDirEsq = noDir->esq;

    noDir->esq = no;
    no->dir = noDirEsq;

    setNivel(no);
    setNivel(noDir);

    return noDir;
}

// Função para balancear um nó
No* balancear(No* no) {
    if (no != NULL) {
        int fator = getNivel(no->dir) - getNivel(no->esq);

        // Balanceado
        if (abs(fator) <= 1) {
            setNivel(no);
        }
        // Desbalanceado para a direita
        else if (fator == 2) {
            if (getNivel(no->dir->dir) < getNivel(no->dir->esq)) {
                no->dir = rotacionarDir(no->dir);
            }
            no = rotacionarEsq(no);
        }
        // Desbalanceado para a esquerda
        else if (fator == -2) {
            if (getNivel(no->esq->dir) > getNivel(no->esq->esq)) {
                no->esq = rotacionarEsq(no->esq);
            }
            no = rotacionarDir(no);
        }
    }
    return no;
}

// Função para inserir um elemento na árvore AVL
No* inserir(Pokemon* elemento, No* no) {
    if (no == NULL) {
        no = newNo(elemento);
    } else if (strcmp(elemento->name, no->elemento->name) < 0) {
        no->esq = inserir(elemento, no->esq);
    } else if (strcmp(elemento->name, no->elemento->name) > 0) {
        no->dir = inserir(elemento, no->dir);
    } 
    return balancear(no);
}

// Função para pesquisar um elemento na árvore AVL
bool pesquisar(char* nome, No* no) {
    bool res;
    if (no == NULL) {
        res = false; // Falso: não encontrou
    } else if (strcmp(nome, no->elemento->name) == 0) {
        res = true; // Verdadeiro: encontrou
    } else if (strcmp(nome, no->elemento->name) < 0) {
        comparacoes++;
        printf("esq ");
        res =  pesquisar(nome, no->esq);
    } else {
        comparacoes++;
        printf("dir ");
        res =  pesquisar(nome, no->dir);

    }
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

// Função principal
void liberarArvore(No* no) {
    if (no != NULL) {
        liberarArvore(no->esq);
        liberarArvore(no->dir);
        free(no);
    }
}

int main(void) {
    Pokemon* pokemons[TAMANHO] = {0}; // Inicializa array de Pokémons
    char entrada[20];
    No* raiz = NULL; // Inicializa raiz da AVL como NULL

    // Ler o arquivo CSV
    lerTodoArquivo("/tmp/pokemon.csv", pokemons);

    // Processa IDs de entrada e insere na AVL
    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        int idP;
        sscanf(entrada, "%d", &idP);
        Pokemon* encontrado = procurar(pokemons, idP);

        if (encontrado != NULL) {
            raiz = inserir(encontrado, raiz);
        }
    }

    // Pesquisa elementos na AVL
    clock_t inicio = clock();
    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        printf("%s\n", entrada);
        printf("raiz ");
        bool res = pesquisar(entrada, raiz);
        printf(res ? "NAO\n" : "SIM\n");
    }
    clock_t fim = clock();

    // Log do tempo de execução
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;
    FILE* log = fopen("840757_avl.txt", "w");
    if (log) {
        fprintf(log, "840757\t%d\t%.2f\n", comparacoes, tempoExecucao);
        fclose(log);
    }

    // Libera memória
    liberarArvore(raiz);
    freeAllPokemons(pokemons, TAMANHO);

    return 0;
}
