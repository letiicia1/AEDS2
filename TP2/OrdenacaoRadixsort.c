#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100 
#define TAMANHO 801
#define NUM_ABILITIES 20 // Número máximo de habilidades por Pokémon
#define MAX_ABILITY_LEN 50 // Tamanho máximo de cada habilidade

int comparacoes = 0; // Variável global para contar comparações
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
    char abilities[NUM_ABILITIES][MAX];
    double weight;
    double height;
    int captureRate;
    bool isLegendary;  
    Date captureDate;
} Pokemon;

// Função para clonar um Pokémon
Pokemon clonePokemon(Pokemon original) {
    Pokemon clone;
    // Copiar campos simples
    clone.id = original.id;
    clone.generation = original.generation;
    strcpy(clone.name, original.name);
    strcpy(clone.description, original.description);

    // Copiar tipos
    strcpy(clone.types[0], original.types[0]);
    strcpy(clone.types[1], original.types[1]);

    // Copiar habilidades 
    int i = 0;
    while (original.abilities[i][0] != '\0') {
        strcpy(clone.abilities[i], original.abilities[i]);
        i++;
    }
    
    // Definir o terminador de array no clone
    clone.abilities[i][0] = '\0';

    // Copiar demais campos
    clone.weight = original.weight;
    clone.height = original.height;
    clone.captureRate = original.captureRate;
    clone.isLegendary = original.isLegendary;

    // Copiar data
    clone.captureDate.dia = original.captureDate.dia;
    clone.captureDate.mes = original.captureDate.mes;
    clone.captureDate.ano = original.captureDate.ano;

    return clone;
}

// Função para procurar um Pokémon pelo ID
Pokemon procurar(Pokemon* pokemons, int id) 
{
     Pokemon pokemon;
    pokemon.id = -1;

    for (int i = 0; i < TAMANHO; i++) {
        if (pokemons[i].id == id) {
            pokemon = pokemons[i];
            i = TAMANHO;
        }
    }
    
   
    return pokemon; 
}



// Função auxiliar para obter o caractere de uma posição específica da habilidade
int obterCaractere(Pokemon pokemon, int pos) {
    if (pos < strlen(pokemon.abilities[0])) {
        comparacoes++;
        return pokemon.abilities[0][pos]; // Primeira habilidade
    }
    return 0; // Se o caractere não existir, retorna 0
}

// Função de contagem para Radixsort
void countingSort(Pokemon* pokemons, int n, int pos) {
    Pokemon output[n];
    int count[256] = {0}; // Contagem de caracteres (ASCII)

    // Contar a frequência de cada caractere na posição 'pos'
    for (int i = 0; i < n; i++) {
        int charAtPos = obterCaractere(pokemons[i], pos);
        count[charAtPos]++;
    }

    // Calcular a posição acumulada
    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    // Construir array de saída baseado nas contagens
    for (int i = n - 1; i >= 0; i--) {
        int charAtPos = obterCaractere(pokemons[i], pos);
        output[count[charAtPos] - 1] = pokemons[i];
        count[charAtPos]--;
    }

    // Copiar a ordenação temporária para o array original
    for (int i = 0; i < n; i++) {
        pokemons[i] = output[i];
        movimentacoes++;
    }
}

// Função principal do Radixsort
void radixsort(Pokemon* pokemons, int n) {
    int maxLen = 0;

    // Determinar a maior comprimento da primeira habilidade
    for (int i = 0; i < n; i++) {
        int len = strlen(pokemons[i].abilities[0]);
        if (len > maxLen) {
            maxLen = len;
        }
    }

    // Aplicar countingSort da menor posição para a maior (dígito menos significativo para o mais significativo)
    for (int pos = maxLen - 1; pos >= 0; pos--) {
        countingSort(pokemons, n, pos);
    }

    // Critério de desempate pelo nome do Pokémon
    for (int i = 0; i < n - 1; i++) {
        if (strcmp(pokemons[i].abilities[0], pokemons[i + 1].abilities[0]) == 0) {
            // Aplicar uma ordenação estável pelo nome
            comparacoes++;
            if (strcmp(pokemons[i].name, pokemons[i + 1].name) > 0) {
                Pokemon temp = pokemons[i];
                pokemons[i] = pokemons[i + 1];
                pokemons[i + 1] = temp;
                movimentacoes+=3;
            }
        }
    }
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

// Função para ler o arquivo CSV e carregar os Pokémons
Pokemon* lerTodoArquivo(char fileName[]) {
    Pokemon* pokemons = (Pokemon*)malloc(TAMANHO * sizeof(Pokemon));
    
    FILE* arquivo = fopen(fileName, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }
    
    char trash[200];
    fgets(trash, sizeof(trash), arquivo); // Pular o cabeçalho
    
    char Line[500];
    int legendary; 
    int index = 0;
    
    while (fgets(Line, sizeof(Line), arquivo) != NULL) {
        char abilities[MAX] = " ";
        Pokemon temp = { .types = { " ", " " } };

        trocarVirgula(Line);

        sscanf(Line, "%d;%d;%[^;];%[^;];%[^;];%[^;]", 
               &temp.id, &temp.generation, temp.name, temp.description, temp.types[0], temp.types[1]);

        char resto[100];
        sscanf(Line, "%[^[][%[^]]]%s", trash, abilities, resto);

        char dividir[10][100];
        int m = 0, j = 0;

        for (int i = 1; i < strlen(resto); i++) {
            if (resto[i] != ';') {
                dividir[m][j++] = resto[i];
            } else {
                dividir[m++][j] = '\0';
                j = 0;
            }
        }

        sscanf(dividir[1], "%lf", &temp.weight);
        sscanf(dividir[2], "%lf", &temp.height);
        sscanf(dividir[3], "%d", &temp.captureRate);
        sscanf(dividir[4], "%d", &legendary);
        sscanf(dividir[5], "%d/%d/%d", &temp.captureDate.dia, &temp.captureDate.mes, &temp.captureDate.ano);

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
        int tamanho2 = strlen(abilities2);
        // Processar habilidades
        m = 0;
        j = 0;
        for (int i = 0; i < tamanho2; i++) {
            if (abilities2[i] == ',') {
                temp.abilities[j][m] = '\0';
                j++;
                m = 0;
                i++;
            } else {
                temp.abilities[j][m++] = abilities2[i];
            }
        }

        temp.isLegendary = (legendary == 1);
        pokemons[index++] = temp;
    }
    
    fclose(arquivo);
    return pokemons;
}

// Função para imprimir informações de um Pokémon
void printar(Pokemon pokemon) {
    printf("[#%d -> %s: %s - ", pokemon.id, pokemon.name, pokemon.description);

    if (strcmp(pokemon.types[1], " ") == 0) {
        printf("['%s'] - ", pokemon.types[0]);
    } else {
        printf("['%s', '%s'] - ", pokemon.types[0], pokemon.types[1]);
    }

    printf("[");

    for (int i = 0; strlen(pokemon.abilities[i]) > 0; i++) 
    {
        printf("'%s'", pokemon.abilities[i]);

        if (strlen(pokemon.abilities[i + 1]) > 0) {
            printf(", ");
        }
    }

    printf("] - %.1lfkg - %.1lfm - %d%% - %s - %d gen] - %02d/%02d/%d\n", 
           pokemon.weight, 
           pokemon.height, 
           pokemon.captureRate, 
           pokemon.isLegendary ? "true" : "false", 
           pokemon.generation,
           pokemon.captureDate.dia, 
           pokemon.captureDate.mes, 
           pokemon.captureDate.ano);
}

// Função Principal
int main(void) {
    int id;
    int i = 0;
    char entrada[20];
    
    Pokemon* pokemons = lerTodoArquivo("/tmp/pokemon.csv");
    if (!pokemons) {
        return 1;
    }

    Pokemon* selecionados = (Pokemon*)malloc(TAMANHO * sizeof(Pokemon));
    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        sscanf(entrada, "%d", &id);
        Pokemon encontrado = procurar(pokemons, id);
        if (encontrado.id != -1) {
            selecionados[i++] = encontrado;
        }
    }

    clock_t inicio = clock(); // Captura o tempo inicial

    radixsort(selecionados, i); // Usar Radixsort para ordenar

    clock_t fim = clock(); // Captura o tempo final
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Imprimir Pokémon ordenados
    for (int j = 0; j < i; j++) {
        printar(selecionados[j]);
    }

    // Criar arquivo de log
    FILE* log = fopen("matricula_radixsort.txt", "w");
    if (log) {
        fprintf(log, "matricula\t%d\t%d\t%.2f\n", comparacoes, movimentacoes, tempoExecucao);
        fclose(log);
    }

    free(pokemons);
    return 0;
}