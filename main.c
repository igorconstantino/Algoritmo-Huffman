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
    printf("Caracter: %c Frequência: %d\n", aux->caracter, aux->frequencia);
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

// Função Principal --------------------------------------------------------------------------  
int main(){
  
  // Para utilizar acentuação
  setlocale(LC_ALL, "Portuguese");

  Lista lista;
  No *arvore;
  int frequencia[TAM_ASCII];
  unsigned char string[] = "Vamos aprender a programa";

  zerarTabela(frequencia);
  inicializarLista(&lista);
  
  calcularFrequencia(string, frequencia);
  montarLista(frequencia, &lista);
  mostrarLista(&lista);

  arvore = construirArvore(&lista);  
  printf("\n\tÁrvore de Huffman:");
  mostrarArvore(arvore, 0);

  
  
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