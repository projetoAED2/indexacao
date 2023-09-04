#ifndef RB_H
#define RB_H
#include <iostream>
#include <fstream>
#include "../cJSON/cJSON.h"

enum Cor
{
    VERMELHO,
    PRETO,
    DUPLO_PRETO
};

typedef struct individuo
{
    int codigo;
    char nome[51];
} Individuo;

typedef struct indice
{
    int codigo;
    int indice; // posição no arquivo do primeiro byte do registro
} Indice;

typedef struct no
{
    Indice *indice;
    enum Cor cor;
    struct no *esq, *dir, *pai;
} no;

typedef no *Arvore;

typedef struct tabela
{
    FILE *arquivoDados;
    Arvore indices;
} Tabela;

int inicializarTabela(Tabela *tab);
void inicializar(Arvore *raiz);

Individuo *lerDados();
void tirarEnter(char *string);

Arvore carregarArquivo(char *nome, Arvore a);

void adicionar(Indice *individuo, Arvore *raiz, int cod);
void adicionarIndividuo(Tabela *tab, Individuo *individuo);

void ajustar(Arvore *raiz, Arvore elemento);

void rotacaoSimplesDir(Arvore *raiz, Arvore pivo);
void rotacaoSimplesEsq(Arvore *raiz, Arvore pivo);

int isElementoRaiz(Arvore elemento);
enum Cor cor(Arvore elemento);
Arvore tio(Arvore elemento);
Arvore irmao(Arvore elemento);
int isFilhoEsquerdo(Arvore elemento);
int isFilhoDireito(Arvore elemento);

void salvarArquivo(char *arquivo, Arvore individuo);
void salvarAux(Arvore raiz, cJSON *root);

void removerRb(int valor, Arvore *raiz);
void reajustar(Arvore *raiz, Arvore elemento);
void retirarDuploPreto(Arvore *raiz, Arvore elemento);
Arvore maiorElemento(Arvore raiz);

int maiorVariavel(Indice *individuo, Arvore arvore, int cod);
void preOrder(Arvore raiz, Tabela *tab);
void imprimirElemento(Arvore raiz, Tabela *tab);

#endif
