#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100 
#define TAMANHO 801

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



//clone
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

void ordenar(Pokemon* pokemons) 
 {
    for (int i = 0; i < TAMANHO; i++) {
        int temp = i;
        for (int j = i + 1; j < TAMANHO; j++) {
            // Comparar os nomes dos Pokémons usando strcmp
            if (strcmp(pokemons[temp].name, pokemons[j].name) > 0) {
                temp = j;
            }
        }
        // Trocar os Pokémons de lugar
        Pokemon temp1 = pokemons[i];
        pokemons[i] = pokemons[temp];
        pokemons[temp] = temp1;
    }
}


Pokemon procuraBinaria(Pokemon* pokemons, char* nome) {
    int inicio = 0;
    int fim = TAMANHO - 1;
    Pokemon pokemon;
    pokemon.id = -1;
    // Garantir que a lista está ordenada por nome antes de realizar a busca
    ordenar(pokemons);

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        // Comparar o nome no meio com o nome buscado
        int comparacao = strcmp(pokemons[meio].name, nome);

        if (comparacao == 0) {
            // Encontrou o Pokémon com o nome correspondente
            pokemon = pokemons[meio];
            inicio = fim + 1;
        }
        else if (comparacao < 0) {
            // O nome buscado está depois (alfabeticamente)
            inicio = meio + 1;
        } else {
            // O nome buscado está antes (alfabeticamente)
            fim = meio - 1;
        }
    }

    // Se não encontrou, retorna um Pokémon inválido
    
    return pokemon; 
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
       // temp.abilities[j][m] = '\0';

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

//  Função Principal
int main(void) {
    int id;
    int i = 0;
    char entrada[20];
    
      Pokemon* pokemons = lerTodoArquivo("/tmp/pokemon.csv");
    //Pokemon* pokemons = lerTodoArquivo("pokemon.csv");

    Pokemon* selecionados = (Pokemon*)malloc(TAMANHO * sizeof(Pokemon));

    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        sscanf(entrada, "%d", &id);
        Pokemon encontrado = procurar(pokemons, id);
        
        if (encontrado.id != -1) {
            selecionados[i++] = encontrado;
        }
    }

     while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        
        Pokemon encontrado = procuraBinaria(selecionados, entrada);
        
        if (encontrado.id != -1) {
            printf("SIM\n");
        }
        else
        {
            printf("NAO\n");    
        }
    }
    
    free(pokemons);
    return 0;
}