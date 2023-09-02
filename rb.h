#ifndef RB_H
#define RB_H
#include <iostream>
#include <fstream>

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

void salvarArquivo(char *arquivo, Arvore individuo);
void salvarAux(Arvore raiz, FILE *arq);

#endif
