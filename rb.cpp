#include "rb.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include </home/izabel/repositorios/ufape/cJSON/cJSON.h>

Arvore noNull;

void inicializar(Arvore *raiz)
{
    *raiz = NULL;
    noNull = (Arvore)malloc(sizeof(struct no));
    noNull->cor = DUPLO_PRETO;
    noNull->individuo = 0;
    noNull->esq = NULL;
    noNull->dir = NULL;
}

void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

Individuo *lerDados()
{
    Individuo *novo = (Individuo *)malloc(sizeof(Individuo));
    limparBufferEntrada(); // Limpa o buffer de entrada
    printf("Codigo: ");
    scanf("%d", &novo->codigo);
    limparBufferEntrada(); // Limpa o buffer de entrada novamente
    printf("Nome: ");
    fgets(novo->nome, 50, stdin);
    tirarEnter(novo->nome);
    return novo;
}

void tirarEnter(char *string)
{
    string[strlen(string) - 1] = '\0';
}

void adicionar(Individuo *individuo, Arvore *raiz)
{
    Arvore posicao, pai, novo;
    posicao = *raiz;
    pai = NULL;

    while (posicao != NULL)
    {
        pai = posicao;
        if (individuo->codigo > posicao->individuo->codigo)
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }
    novo = (Arvore)malloc(sizeof(struct no));
    novo->individuo = individuo;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;
    novo->cor = VERMELHO;

    if (isElementoRaiz(novo))
        *raiz = novo;
    else
    {
        if (individuo->codigo > pai->individuo->codigo)
            pai->dir = novo;
        else
            pai->esq = novo;
    }

    ajustar(raiz, novo);
}

void ajustar(Arvore *raiz, Arvore elemento)
{
    while (cor(elemento->pai) == VERMELHO && cor(elemento) == VERMELHO)
    {
        // caso 1
        if (cor(tio(elemento)) == VERMELHO)
        {
            elemento->pai->cor = PRETO;
            tio(elemento)->cor = PRETO;
            elemento->pai->pai->cor = VERMELHO;
            elemento = elemento->pai->pai;
            continue;
        }
        // caso 2a: rotação simples direita
        if (isFilhoEsquerdo(elemento) && isFilhoEsquerdo(elemento->pai))
        {
            rotacaoSimplesDir(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            irmao(elemento)->cor = VERMELHO;
            continue;
        }
        // caso 2b: rotação simples esquerda
        if (isFilhoDireito(elemento) && isFilhoDireito(elemento->pai))
        {
            rotacaoSimplesEsq(raiz, elemento->pai->pai);
            elemento->pai->cor = PRETO;
            irmao(elemento)->cor = VERMELHO;
            continue;
        }
        // caso 3: rotação dupla
        if (cor(tio(elemento)) == PRETO)
        {
            // caso 3a: rotação dupla direita
            if (isFilhoDireito(elemento) && isFilhoEsquerdo(elemento->pai))
            {
                rotacaoSimplesEsq(raiz, elemento->pai);
                rotacaoSimplesDir(raiz, elemento->pai);
                elemento->cor = PRETO;
                elemento->esq->cor = VERMELHO;
                elemento->dir->cor = VERMELHO;
            }
            // caso 3b: rotação dupla esquerda
            if (isFilhoEsquerdo(elemento) && isFilhoDireito(elemento->pai))
            {
                rotacaoSimplesDir(raiz, elemento->pai);
                rotacaoSimplesEsq(raiz, elemento->pai);
                elemento->cor = PRETO;
                elemento->esq->cor = VERMELHO;
                elemento->dir->cor = VERMELHO;
            }
        }
    }
    // caso 0
    (*raiz)->cor = PRETO;
}

void rotacaoSimplesDir(Arvore *raiz, Arvore pivo)
{
    Arvore u, t1, t2, t3;
    u = pivo->esq;
    t1 = u->esq;
    t2 = u->dir;
    t3 = pivo->dir;

    int pivoEsq = isFilhoEsquerdo(pivo);

    pivo->esq = t2;
    if (t2 != NULL)
        t2->pai = pivo;

    u->dir = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;

    if (isElementoRaiz(u))
        *raiz = u;
    else
    {
        if (pivoEsq)
            u->pai->esq = u;
        else
            u->pai->dir = u;
    }
}

void rotacaoSimplesEsq(Arvore *raiz, Arvore pivo)
{
    Arvore u, t1, t2, t3;
    u = pivo->dir;
    t1 = pivo->esq;
    t2 = u->esq;
    t3 = u->dir;

    int pivoDir = isFilhoDireito(pivo);

    pivo->dir = t2;
    if (t2 != NULL)
        t2->pai = pivo;

    u->esq = pivo;
    u->pai = pivo->pai;
    pivo->pai = u;

    if (isElementoRaiz(u))
        *raiz = u;
    else
    {
        if (pivoDir)
            u->pai->dir = u;
        else
            u->pai->esq = u;
    }
}

int isElementoRaiz(Arvore elemento)
{
    return (elemento->pai == NULL);
}

enum Cor cor(Arvore elemento)
{
    if (elemento == NULL)
        return PRETO;
    else
        return elemento->cor;
}

Arvore tio(Arvore elemento)
{
    return irmao(elemento->pai);
}

Arvore irmao(Arvore elemento)
{
    if (isFilhoEsquerdo(elemento))
        return elemento->pai->dir;
    else
        return elemento->pai->esq;
}

int isFilhoEsquerdo(Arvore elemento)
{
    return (elemento->pai != NULL && elemento == elemento->pai->esq);
}

int isFilhoDireito(Arvore elemento)
{
    return (elemento->pai != NULL && elemento == elemento->pai->dir);
}

void salvarArquivo(char *nomeArquivo, Arvore arvore)
{
    if (arvore == NULL)
    {
        printf("A árvore está vazia ou não inicializada.\n");
        return;
    }

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo)
    {
        cJSON *root = cJSON_CreateArray(); // Criar um array JSON

        salvarAux(arvore, root); // Passar o objeto JSON array criado

        char *jsonStr = cJSON_Print(root); // Imprimir o objeto JSON array
        fputs(jsonStr, arquivo);

        fclose(arquivo);
        cJSON_Delete(root);
        free(jsonStr);
    }
}

void salvarAux(Arvore raiz, cJSON *root)
{
    if (raiz != NULL)
    {
        cJSON *node = cJSON_CreateObject(); // Criar um novo objeto JSON para cada nó
        Individuo *individuo = raiz->individuo;
        cJSON_AddItemToObject(node, "codigo", cJSON_CreateNumber(individuo->codigo));
        cJSON_AddItemToObject(node, "nome", cJSON_CreateString(individuo->nome));

        cJSON_AddItemToArray(root, node); // Adicionar o objeto node ao array root

        salvarAux(raiz->esq, root);
        salvarAux(raiz->dir, root);
    }
}
