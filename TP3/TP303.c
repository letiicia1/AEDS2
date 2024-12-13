#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100 
#define TAMANHO 801

#define MAXTAM    881


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
  Pokemon* p = (Pokemon*) malloc(sizeof(Pokemon));
  return p;
}

void delPokemon(Pokemon* p) {
  free(p);
}


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
Pokemon* procurar(Pokemon** pokemons, int id) {
    Pokemon* pokemon = NULL;

    for (int i = 0; i < TAMANHO; i++) {
        if (pokemons[i]->id == id) {
            pokemon = pokemons[i];
           i = TAMANHO;
        }
    }

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

Pokemon* pokemons[881]; 

Pokemon* lerPokemon(Pokemon* temp, char Line[]) {
  //Le e configura os campos do pokemon a partir da linh
     char trash[200];
  char abilities[MAX] = " ";
    int legendary; 
    
         strcpy(temp->types[0], " ");
        strcpy(temp->types[1], " ");

        trocarVirgula(Line);

        sscanf(Line, "%d;%d;%[^;];%[^;];%[^;];%[^;]", 
               &temp->id, &temp->generation, temp->name, temp->description, temp->types[0], temp->types[1]);

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

        sscanf(dividir[1], "%lf", &temp->weight);
        sscanf(dividir[2], "%lf", &temp->height);
        sscanf(dividir[3], "%d", &temp->captureRate);
        sscanf(dividir[4], "%d", &legendary);
        sscanf(dividir[5], "%d/%d/%d", &temp->captureDate.dia, &temp->captureDate.mes, &temp->captureDate.ano);

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
                temp->abilities[j][m] = '\0';
                j++;
                m = 0;
                i++;
            } else {
                temp->abilities[j][m++] = abilities2[i];
            }
        }
       // temp->abilities[j][m] = '\0';

        temp->isLegendary = (legendary == 1);

        return(temp);
        
}

// Função para ler o arquivo CSV e carregar os Pokémons
  void lerTodoArquivo(char fileName[]) {
    
    FILE* arquivo = fopen(fileName, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo\n");
        
    }
    int index = 0;
    char Line[200];
    fgets(Line, sizeof(Line), arquivo); // Pular o cabeçalho
    
  
    while (fgets(Line, sizeof(Line), arquivo) != NULL) 
    {
        
        Pokemon* p = newPokemon();
        pokemons[index++] = lerPokemon(p, Line);
    }
    
    fclose(arquivo);
}

Pokemon* array[MAXTAM];    
int n;               // Quantidade de array.


/**
 * Inicializacoes
 */
void start(){
   n = 0;
}


/**
 * Insere um elemento na ultima posicao da 
 * @param x int elemento a ser inserido.
 */
void inserirFim(Pokemon* x) {

   //validar insercao
   if(n >= MAXTAM){
      printf("Erro ao inserir!");
      exit(1);
   }

   array[n] = x;
   n++;
}


/**
 * Remove um elemento da ultima posicao da 
 * @return resp int elemento a ser removido.
 */
Pokemon* removerFim() {

   //validar remocao
   if (n == 0) {
      printf("Erro ao remover!");
   }
      printf("(R) %s\n", array[n - 1]->name);
    
   return array[--n];
}



// Função para imprimir informações de um Pokémon
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

void freeAllPokemons(Pokemon* pokemons[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        if (pokemons[i] != NULL) {
            free(pokemons[i]);
            pokemons[i] = NULL;  
        }
    }
}


//  Função Principal
int main(void) {
    start( );
    int idP;
    char entrada[20];
    int num = 0; 
    Pokemon* encontrado = newPokemon();
    lerTodoArquivo("/tmp/pokemon.csv");
     //lerTodoArquivo("pokemon.csv");


    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        sscanf(entrada, "%d", &idP);
        encontrado = procurar(pokemons, idP);
        
        if (encontrado != NULL) {
             inserirFim(encontrado);
        }
    }

    scanf("%d" , &num);
    getchar ( );
    
 
for(int i = 0; i < num; i++) 
{
    char partes[3][10]; 
    int id = 0; 
    int pos = 0; 
    char entrada[30]; 

    fgets(entrada, sizeof(entrada), stdin); 

    sscanf(entrada, "%s %s %s", partes[0], partes[1], partes[2]);

   // printf("COMANDO: %s, ID: %s, Pos: %s\n", partes[0], partes[1], partes[2]); // Debug

     if (strcmp(partes[0], "I") == 0) 
     {
        sscanf(partes[1], "%d", &id);
        encontrado = procurar(pokemons, id);
        inserirFim(encontrado);
    } 
    
    else if (strcmp(partes[0], "R") == 0) 
    {
        removerFim();
    } 
}

    for(int j = 0; j < n; j++)
    {
    
        printf("[%d] " , j);
        printar(array[j]);
    }
    
    
    
   
   freeAllPokemons(pokemons, TAMANHO);
    return 0;
}
