// Inclusão de bibliotecas, constantes e structs ---------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define TAM_ASCII 256

typedef struct no{
  int frequencia;
  unsigned char caracter; // Unsigned para usarmos caracteres especiais 
  struct no *esq, *dir; // Para usar na árvore
  struct no *prox; // Para usar na lista
}No;

typedef struct lista{
  int tam;
  No *inicio;
}Lista;


// Funções -----------------------------------------------------------------------------------

// Função que zera todos os elementos da tabela de frequência
void zerarTabela(int frequencia[]){
  for(int i = 0; i < TAM_ASCII; i++)
    frequencia[i] = 0;
}

// Função que calcula a frequência de cada caractere do texto
void calcularFrequencia(unsigned char texto[], int frequencia[]){
  int i = 0;

  while(texto[i] != '\0'){
    frequencia[texto[i]]++; // Índice é o número (ASCII) do próprio caractere lido 
    i++;
  }
}

// Função que cria a lista 
void inicializarLista(Lista *lista){
  lista->inicio = NULL;
  lista->tam = 0;
}

// Função que insere os nós na lista 
void inserirLista(Lista *lista, No *no){
  No *aux;

  if(lista->inicio == NULL) // Lista vazia
    lista->inicio = no;
  else if(no->frequencia < lista->inicio->frequencia){ // Se tiver frequência menor que o início da lista
    no->prox = lista->inicio;
    lista->inicio = no;
  }else{ // Se tiver frequência maior ou igual
    aux = lista->inicio;

    while(aux->prox != NULL && aux->prox->frequencia <= no->frequencia)
      aux = aux->prox; // Encontrando onde inserir

    no->prox = aux->prox; // Conectando o novo nó na lista
    aux->prox = no; // Aux agora aponta para o novo nó
  }
  lista->tam++;
}

// Função que preenche a lista de acordo com a frequência
void montarLista(int frequencia[], Lista *lista){
  No *novo;

  for(int i = 0; i < TAM_ASCII; i++){ // Percorrendo a tabela de frequência
    if(frequencia[i] > 0){ // Verificando se há ocorrência
      novo = malloc(sizeof(No));

      if(novo != NULL){
        novo->prox = NULL;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->caracter = i; // Pegando qual o caracter
        novo->frequencia = frequencia[i]; // Pegando a frequência do caracter

        inserirLista(lista, novo); // Inserindo o elemento na lista
      }else{
        printf("Erro ao alocar memória para montar a lista");
        break;
      }
    }
  }
}

// Função que mostra a lista ordenada
void mostrarLista(Lista *lista){
  No *aux = lista->inicio;

  printf("\n\tLista ordenada: \n");
  while(aux != NULL){
    printf("Caracter: %c Frequencia: %d\n", aux->caracter, aux->frequencia);
    aux = aux->prox;
  }
}

// Função para remover o nó do início
No* removerNo(Lista *lista){
  No *aux = NULL;

  if(lista->inicio != NULL){
    aux = lista->inicio;
    lista->inicio = aux->prox; // Novo início da lista
    aux->prox = NULL; 
    lista->tam--;
  } 
  return aux; // Retornando o nó removido
}

// Função que constrói a árvore codificadora ótima
No* construirArvore(Lista *lista){
  No *primeiro, *segundo, *noCriado;
  
  while(lista->tam > 1){
    primeiro = removerNo(lista);
    segundo = removerNo(lista);
    noCriado = malloc(sizeof(No));

    // Atribuindo os valores ao nó criado
    if(noCriado != NULL){
      noCriado->caracter = '*';
      noCriado->frequencia = primeiro->frequencia + segundo->frequencia;
      noCriado->esq = primeiro;
      noCriado->dir = segundo;
      noCriado->prox = NULL;

      inserirLista(lista, noCriado); // Inserindo o nó criado na lista
    }else{
      printf("Erro ao alocar memória para criar nó da árvore");
      break;
    }
  }
  return lista->inicio; // Retornando a raiz da árvore codificadora ótima
}

// Função que mostra a árvore
void mostrarArvore(No *raiz, int altura){
  if(raiz->esq == NULL && raiz->dir == NULL)
    printf("\nCaracter: %c Altura: %d", raiz->caracter, altura);
  else{
    mostrarArvore(raiz->esq, altura + 1);
    mostrarArvore(raiz->dir, altura + 1);
  }
}

//Função que calcula a altura da árvore
int alturaArvore(No * raiz){
  int dir, esq;
  
  if(raiz == NULL)
    return -1;
  else{
    esq = alturaArvore(raiz->esq) + 1; 
    dir = alturaArvore(raiz->dir) + 1;

    // Retornando o maior lado da árvore
    if(esq > dir)
      return esq;
    else
      return dir;
  }  
}

// Função para alocar espaço para dicionário
char** alocarDicionario(int colunas){
  char **dicionario; // Ponteiros duplos pois são alocações para matrizes

  dicionario = malloc(sizeof(char*) * TAM_ASCII); // Cada ponteiro de ponteiro contém um ponteiro para char / 256 linhas

  for (int i = 0; i < TAM_ASCII; i++)
    dicionario[i] = calloc(colunas, sizeof(char)); // Alocando memória para cada caracter "colunas" vezes
  
  return dicionario; // Retornando o espaço alocado para a matriz
}

// Função para gerar o dicionário
void preencherDicionario(char **dicionario, No *raiz, char *percorrido, int colunas){
  char esq[colunas], dir[colunas]; 
  
  if(raiz->esq == NULL && raiz->dir == NULL) // Verificando nó folha
    strcpy(dicionario[raiz->caracter], percorrido); // Copiando o caminho na matriz dicionário, no caractere desse nó
  else{
    // Copiando o que já existe no caminho para cada "lado" da árvore
    strcpy(esq, percorrido);
    strcpy(dir, percorrido);
    // Adicionando 0 se for pela esquerda e 1 se for pela direita
    strcat(esq, "0");
    strcat(dir, "1");
    // Percorrendo a árvore para esquerda e direita
    preencherDicionario(dicionario, raiz->esq, esq, colunas);
    preencherDicionario(dicionario, raiz->dir, dir, colunas);
  }  
}

// Funcão para msotrar o dicionário
void mostrarDicionario(char **dicionario){
  printf("\n\n\tDicionario: \n");

  for (int i = 0; i < TAM_ASCII; i++){
    if(strlen(dicionario[i]) > 0)
      printf("\t%3d: %s\n", i, dicionario[i]);
  }  
} 

// Função que calcula o tamanho da string para podermos codificar
int tamanhoString(char **dicionario, unsigned char *string){
  int i = 0, tam = 0;

  // Incrementando o tamanho até chegar no final da string
  while(string[i] != '\0'){
    tam += strlen(dicionario[string[i]]); // String[i] é o caractere do dicionário
    i++;
  }
  
  return tam + 1; // Tamanho da string mais \0
}
 
// Função que codifica o texto
char* codificar(char **dicionario, unsigned char *string){
  int i = 0, tamStr = tamanhoString(dicionario, string);
  
  char* codigo = calloc(tamStr, sizeof(char)); // Alocando espaço para "tamStr" vezes, o espaço de um caracter

  // Percorrendo a string e concatenando o conteúdo do dicionário no texto codificado
  while(string[i] != '\0'){
    strcat(codigo, dicionario[string[i]]);
    i++;
  }

  return codigo; // Retorna o texto codificado
}

// Função para decodificar o texto
char* decodificar(unsigned char string[], No *raiz){
  int i = 0;
  No *aux = raiz;
  char temp[2];
  char *decodificado = calloc(strlen(string), sizeof(char));

  // Percorrendo a string e caminhando na árvore conforme os 0's e 1's
  while(string[i] != '\0'){
    if(string[i] == '0')
      aux = aux->esq;
    else
      aux = aux->dir;

    // Se nó folha, concatenar os 0's e 1's lidos ao texto decodificado
    if(aux->esq == NULL && aux->dir == NULL){
      temp[0] = aux->caracter;
      temp[1] = '\0';
      strcat(decodificado, temp);
      aux = raiz; // Aux começa na raiz de novo
    }
    i++;
  }
  return decodificado; // Retorna o texto decodificado
}

// Função para compactar o arquivo
void compactar(unsigned char string[]){
  FILE *arq = fopen("compactado.huf", "wb");
  int i = 0, j = 7;
  unsigned char mascara, byte = 0;

  // Transformando os caracteres em bits
  if(arq){
    while(string[i] != '\0'){
      mascara = 1;

      if(string[i] == '1'){
        mascara = mascara << j; // Copiando em mascara o bit 1 na esquerda (posição 7)
        byte = byte | mascara; // Colocando o bit 1 na posição mais à esquerda do byte através do ou bit a bit
      }
      j--; // j decrementa para percorrer toda a sequência de caracteres

      if(j < 0){ // Sequência de 8 bits já foi percorrida
        fwrite(&byte, sizeof(unsigned char), 1, arq); // Escrevendo um byte no arquivo
        byte = 0; // Reinicializando o byte para continuar a sequência
        j = 7; // Reinicializando j para percorrer os bits novamente
      }
      i++; // Caminhando na string
    }
    if(j != 7) // Ficou um byte pela metade (ex: 11110000)
      fwrite(&byte, sizeof(unsigned char), 1, arq);

    fclose(arq);  
  }else
    printf("Erro ao criar arquivo na compactação");
}




// Função Principal --------------------------------------------------------------------------  
int main(){
  
  // Para utilizar acentuação
  setlocale(LC_ALL, "Portuguese");

  Lista lista;
  No *arvore;
  int frequencia[TAM_ASCII];
  int colunas;
  char **dicionario;
  char *codificado, *decodificado;
  unsigned char string[] = "Vamos aprender a programar";

  // Tabela de frequência
  zerarTabela(frequencia);
  calcularFrequencia(string, frequencia);
  
  // Lista ordenada
  inicializarLista(&lista);
  montarLista(frequencia, &lista);
  mostrarLista(&lista);

  // Árvore de Huffman
  arvore = construirArvore(&lista);  
  printf("\n\tArvore de Huffman:");
  mostrarArvore(arvore, 0);

  // Dicionário
  colunas = alturaArvore(arvore) + 1;
  dicionario = alocarDicionario(colunas);
  preencherDicionario(dicionario, arvore, "", colunas);
  mostrarDicionario(dicionario);

  // Codificação
  codificado = codificar(dicionario, string);
  printf("\n\tString codificada: %s\n", codificado);

  // Decodificação
  decodificado = decodificar(codificado, arvore);
  printf("\n\tString decodificada: %s\n", decodificado);
  
  return 0;
}


/*

/--------------- ALGORITMO DE HUFFMAN ---------------/

1 - Ler string e gerar uma tabela de frequência;

  1.1 - Um vetor de inteiros do tamanho da tabela ASCII com o índice sendo o
        valor do caractere e o valor armazenado sendo a quantidade de vezes que aparece 
  1.2 - Inicializar o vetor com zero 
  1.3 - Percorrer a string incrementando o valor do vetor na posição do caractere

2 - Gerar a lista com nós da arvore;

  2.1 - Criar o nó da árvore binária
  2.2 - Percorrer a tabela de frequência, para cada caractere existente no
        texto, criar seu nó e inserir na lista

3 - Gerar a árvore codificadora ótima;

  3.1 - Remover os nós da lista seguindo o algoritmo e obter a árvore
        decodificadora ótima

4 - Gerar dicionário;

  4.1 - Matriz de string (tabela de codificação)
  4.2 - Linhas = tamanho da tabela ASCII
  4.3 - Colunas = altura da árvore codificadora ótima (máximo de dígitos que o
        código pode ter) + 1 (para o \0)

5 - Codificar o texto lido;

  5.1 - Codificar cada caracter e ir montando a sequência de 0's e 1's

6 - Decodificar o texto lido;

  6.1 - Percorrer a árvore codificadora ótima de acordo com cada bit lido na
        string
  6.2 - Concatenar o caracter de cada nó folha atingido na string original


/--------------- ALGORITMO DE HUFFMAN ---------------/

*/