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

//TIPO CELULA ===================================================================
typedef struct CelulaDupla {
	Pokemon* elemento;        // Elemento inserido na celula.
	struct CelulaDupla* prox; // Aponta a celula prox.
   struct CelulaDupla* ant;  // Aponta a celula anterior.
} CelulaDupla;

CelulaDupla* novaCelulaDupla(Pokemon* elemento) {
   CelulaDupla* nova = (CelulaDupla*) malloc(sizeof(CelulaDupla));
   nova->elemento = elemento;
   nova->ant = nova->prox = NULL;
   return nova;
}

//LISTA PROPRIAMENTE DITA =======================================================
CelulaDupla* primeiro;
CelulaDupla* ultimo;


/**
 * Cria uma lista dupla sem elementos (somente no cabeca).
 */
void start () {
   primeiro = novaCelulaDupla(NULL);
   ultimo = primeiro;
}


/**
 * Insere um elemento na primeira posicao da lista.
 * @param x int elemento a ser inserido.
 */
void inserirInicio(Pokemon* x) {
   CelulaDupla* tmp = novaCelulaDupla(x);

   tmp->ant = primeiro;
   tmp->prox = primeiro->prox;
   primeiro->prox = tmp;
   if (primeiro == ultimo) {                    
      ultimo = tmp;
   } else {
      tmp->prox->ant = tmp;
   }
   tmp = NULL;
}


/**
 * Insere um elemento na ultima posicao da lista.
 * @param x int elemento a ser inserido.
 */
void inserirFim(Pokemon* x) {
   ultimo->prox = novaCelulaDupla(x);
   ultimo->prox->ant = ultimo;
   ultimo = ultimo->prox;
}


/**
 * Remove um elemento da primeira posicao da lista.
 * @return resp int elemento a ser removido.
 */
Pokemon* removerInicio() {
   if (primeiro == ultimo) {
      printf("Erro ao remover (vazia)!\n");
   }

   CelulaDupla* tmp = primeiro;
   primeiro = primeiro->prox;
   Pokemon* resp = primeiro->elemento;
   tmp->prox = primeiro->ant = NULL;
   free(tmp);
   tmp = NULL;
   return resp;
}
/**
 * Remove um elemento da ultima posicao da lista.
 * @return resp int elemento a ser removido.
 */
Pokemon* removerFim() {
   if (primeiro == ultimo) {
      printf("Erro ao remover (vazia)!\n");
   } 
   Pokemon* resp = ultimo->elemento;
   ultimo = ultimo->ant;
   ultimo->prox->ant = NULL;
   free(ultimo->prox);
   ultimo->prox = NULL;
   return resp;
}


/**
 *  Calcula e retorna o tamanho, em numero de elementos, da lista.
 *  @return resp int tamanho
 */
int tamanho() {
   int tamanho = 0; 
   CelulaDupla* i;
   for(i = primeiro; i != ultimo; i = i->prox, tamanho++);
   return tamanho;
}


/**
 * Insere um elemento em uma posicao especifica considerando que o 
 * primeiro elemento valido esta na posicao 0.
 * @param x int elemento a ser inserido.
 * @param pos int posicao da insercao.
 * @throws Exception Se <code>posicao</code> invalida.
 */
void inserir(Pokemon* x, int pos) {

   int tam = tamanho();

   if(pos < 0 || pos > tam){
      printf("Erro ao remover (posicao %d/%d invalida!\n", pos, tam);
   } else if (pos == 0){
      inserirInicio(x);
   } else if (pos == tam){
      inserirFim(x);
   } else {
      // Caminhar ate a posicao anterior a insercao
      CelulaDupla* i = primeiro;
      int j;
      for(j = 0; j < pos; j++, i = i->prox);

      CelulaDupla* tmp = novaCelulaDupla(x);
      tmp->ant = i;
      tmp->prox = i->prox;
      tmp->ant->prox = tmp->prox->ant = tmp;
      tmp = i = NULL;
   }
}


/**
 * Remove um elemento de uma posicao especifica da lista
 * considerando que o primeiro elemento valido esta na posicao 0.
 * @param posicao Meio da remocao.
 * @return resp int elemento a ser removido.
 * @throws Exception Se <code>posicao</code> invalida.
 */
Pokemon* remover(int pos) {
     Pokemon* resp;
   int tam = tamanho();

   if (primeiro == ultimo){
      printf("Erro ao remover (vazia)!\n");
   } else if(pos < 0 || pos >= tam){
      printf("Erro ao remover (posicao %d/%d invalida!\n", pos, tam);
   } else if (pos == 0){
      resp = removerInicio();
   } else if (pos == tam - 1){
      resp = removerFim();
   } else {
      // Caminhar ate a posicao anterior a insercao
      CelulaDupla* i = primeiro->prox;
      int j;
      for(j = 0; j < pos; j++, i = i->prox);

      i->ant->prox = i->prox;
      i->prox->ant = i->ant;
      resp = i->elemento;
      i->prox = i->ant = NULL;
      free(i);
      i = NULL;
   }

   return resp;
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

void trocarCelulas(CelulaDupla* a, CelulaDupla* b) {

    // Guarda os ponteiros das células
    Pokemon* tempElemento = a->elemento;
    a->elemento = b->elemento;
    b->elemento = tempElemento;
}

CelulaDupla* getelementoposicao(int pos) {
    int tamanho = 0;
    CelulaDupla* i;
    for (i = primeiro->prox; tamanho < pos && i != NULL; i = i->prox) {
        tamanho++;
    }
    return i;
}

void ordenar(int esq, int dir) {

    int i = esq;
    int j = dir;
 
    CelulaDupla* pivoCelula = getelementoposicao((esq + dir) / 2); 
    
    int pivoGeneration = pivoCelula->elemento->generation;
    char pivoName[100];
    strcpy(pivoName, pivoCelula->elemento->name);
  
    while (i <= j) {
     
        // Buscar elemento maior ou igual ao pivô da esquerda para a direita
        CelulaDupla* x = getelementoposicao(i);
        while (x != NULL && 
              (x->elemento->generation < pivoGeneration || 
              (x->elemento->generation == pivoGeneration && strcmp(x->elemento->name, pivoName) < 0))) {
            i++;
            x = getelementoposicao(i);
            comparacoes++;
        }

        // Buscar elemento menor ou igual ao pivô da direita para a esquerda
        CelulaDupla* y = getelementoposicao(j);
        while (y != NULL &&
              (y->elemento->generation > pivoGeneration || 
              (y->elemento->generation == pivoGeneration && strcmp(y->elemento->name, pivoName) > 0))) {
            j--;
            y = getelementoposicao(j);
            comparacoes++;
            
        }

        // Trocar os elementos de i e j se eles ainda são válidos e não se cruzaram
        if (i <= j) {
            if (x != NULL && y != NULL) {  // Verificar se x e y são válidos
                trocarCelulas(x, y);
                movimentacoes += 3;
            }
            i++;
            j--;
        }
    }

    // Chamada recursiva para as sublistas, garantindo que são válidas
    if (esq < j) ordenar(esq, j);
    if (i < dir) ordenar(i, dir);
}

// Função principal
int main(void) {
    start();
    Pokemon* pokemons[TAMANHO] = {0}; // Inicializar array de Pokémons com NULL
    char entrada[20];
    int num = 0; 
    //lerTodoArquivo("pokemon.csv", pokemons);
    lerTodoArquivo("/tmp/pokemon.csv", pokemons);
  
    while (scanf("%s", entrada) && strcmp(entrada, "FIM") != 0) {
        int idP;
        sscanf(entrada, "%d", &idP);
        Pokemon* encontrado = procurar(pokemons, idP);
        
        if (encontrado != NULL) {
            inserirFim(encontrado);
        }
    }
 
     int cont = 0;
    CelulaDupla* atual = primeiro->prox;
    do {
         cont++;
        atual = atual->prox;
    } while (atual != NULL);


     clock_t inicio = clock(); 
    ordenar( 0, cont - 1);
     clock_t fim = clock();
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // Criar arquivo de log e escrever informações
    FILE* log = fopen("840757_quicksort2.txt", "w");
    if (log) {
        fprintf(log, "840757\t%d\t%d\t%.2f\n", comparacoes, movimentacoes, tempoExecucao);
        fclose(log);
    }

    cont = 0;
     atual = primeiro->prox;
    do {
        printf("[%d] ", cont++);
        printar(atual->elemento);
        atual = atual->prox;
    } while (atual != NULL);


    // Liberar memória
    freeAllPokemons(pokemons, TAMANHO);

    return 0;
}
