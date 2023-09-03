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

typedef struct no
{
    Individuo *individuo;
    enum Cor cor;
    struct no *esq, *dir, *pai;
} no;

typedef no *Arvore;

void inicializar(Arvore *raiz);

Individuo *lerDados();
void tirarEnter(char *string);
void adicionar(Individuo *individuo, Arvore *raiz);

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

#endif
