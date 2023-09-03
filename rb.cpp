#include "rb.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "../cJSON/cJSON.h"

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

void removerRb(int valor, Arvore *raiz)
{
    Arvore posicao;
    posicao = *raiz;

    while (posicao != NULL)
    {
        if (valor == posicao->individuo->codigo)
        {
            // 0 filho
            if (posicao->esq == NULL && posicao->dir == NULL)
            {
                // raiz 0 filho
                if (isElementoRaiz(posicao))
                {
                    *raiz = NULL;
                    free(posicao);
                    break;
                }
                // vermelho 0 filho
                if (posicao->cor == VERMELHO)
                {
                    if (isFilhoEsquerdo(posicao))
                        posicao->pai->esq = NULL;
                    else
                        posicao->pai->dir = NULL;
                    free(posicao);
                    break;
                }
                // preto 0 filho
                else
                {
                    noNull->cor = DUPLO_PRETO;
                    noNull->pai = posicao->pai;
                    if (isFilhoEsquerdo(posicao))
                        posicao->pai->esq = noNull;
                    else
                        posicao->pai->dir = noNull;
                    free(posicao);

                    reajustar(raiz, noNull);
                    break;
                }
                break;
            }
            // 1 filho esq
            if (posicao->esq != NULL && posicao->dir == NULL)
            {
                posicao->esq->cor = PRETO;
                posicao->esq->pai = posicao->pai;

                if (isElementoRaiz(posicao))
                    *raiz = posicao->esq;
                else
                {
                    if (isFilhoDireito(posicao))
                        posicao->pai->dir = posicao->esq;
                    else
                        posicao->pai->esq = posicao->esq;
                }
                free(posicao);
                break;
            }
            // 1 filho dir
            if (posicao->dir != NULL && posicao->esq == NULL)
            {
                posicao->dir->cor = PRETO;
                posicao->dir->pai = posicao->pai;

                if (isElementoRaiz(posicao))
                    *raiz = posicao->dir;
                else
                {
                    if (isFilhoEsquerdo(posicao))
                        posicao->pai->esq = posicao->dir;
                    else
                        posicao->pai->dir = posicao->dir;
                }
                free(posicao);
                break;
            }
            // caso 2 filhos
            int maiorValorEsq = maiorElemento(posicao->esq)->individuo->codigo;
            posicao->individuo->codigo = maiorValorEsq;
            removerRb(posicao->individuo->codigo, &(posicao->esq));
            break;
        }
        if (valor > posicao->individuo->codigo)
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }
}

void reajustar(Arvore *raiz, Arvore elemento)
{
    // caso 1
    if (isElementoRaiz(elemento))
    {
        elemento->cor = PRETO;
        if (elemento == noNull)
            *raiz = NULL;
        return;
    }
    // caso 2
    if (cor(elemento->pai) == PRETO && cor(irmao(elemento)) == VERMELHO && cor(irmao(elemento)->dir) == PRETO && cor(irmao(elemento)->esq) == PRETO)
    {
        if (isFilhoEsquerdo(elemento))
            rotacaoSimplesEsq(raiz, elemento->pai);
        else
            rotacaoSimplesDir(raiz, elemento->pai);

        elemento->pai->pai->cor = PRETO;
        elemento->pai->cor = VERMELHO;

        reajustar(raiz, elemento);
        return;
    }
    // caso 3
    if (cor(elemento->pai) == PRETO && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == PRETO && cor(irmao(elemento)->esq) == PRETO)
    {
        elemento->pai->cor = DUPLO_PRETO;
        irmao(elemento)->cor = VERMELHO;
        retirarDuploPreto(raiz, elemento);
        reajustar(raiz, elemento->pai);
        return;
    }

    // caso 4
    if (cor(elemento->pai) == VERMELHO && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == PRETO && cor(irmao(elemento)->esq) == PRETO)
    {
        irmao(elemento)->cor = VERMELHO;
        elemento->pai->cor = PRETO;
        retirarDuploPreto(raiz, elemento);
        return;
    }
    // caso 5a
    if (isFilhoEsquerdo(elemento) && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == VERMELHO && cor(irmao(elemento)->dir) == PRETO)
    {
        rotacaoSimplesDir(raiz, irmao(elemento));
        irmao(elemento)->cor = PRETO;
        irmao(elemento)->dir->cor = VERMELHO;

        reajustar(raiz, elemento);
        return;
    }
    // caso 5b
    if (isFilhoDireito(elemento) && cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == VERMELHO && cor(irmao(elemento)->esq) == PRETO)
    {
        rotacaoSimplesEsq(raiz, irmao(elemento));
        irmao(elemento)->cor = PRETO;
        irmao(elemento)->esq->cor = VERMELHO;

        reajustar(raiz, elemento);
        return;
    }
    // caso 6a
    if (cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->dir) == VERMELHO)
    {
        rotacaoSimplesEsq(raiz, elemento->pai);
        elemento->pai->pai->cor = elemento->pai->cor;
        elemento->pai->cor = PRETO;
        tio(elemento)->cor = PRETO;
        retirarDuploPreto(raiz, elemento);
        return;
    }
    // caso 6b
    if (cor(irmao(elemento)) == PRETO && cor(irmao(elemento)->esq) == VERMELHO)
    {
        rotacaoSimplesDir(raiz, elemento->pai);
        elemento->pai->pai->cor = elemento->pai->cor;
        elemento->pai->cor = PRETO;
        tio(elemento)->cor = PRETO;
        retirarDuploPreto(raiz, elemento);
        return;
    }
}

void retirarDuploPreto(Arvore *raiz, Arvore elemento)
{
    if (elemento == noNull)
        if (isFilhoEsquerdo(elemento))
            elemento->pai->esq = NULL;
        else
            elemento->pai->dir = NULL;
    else
        elemento->cor = PRETO;
}

Arvore maiorElemento(Arvore raiz)
{
    if (raiz == NULL)
        return NULL;
    if (raiz->dir == NULL)
        return raiz;
    else
        return maiorElemento(raiz->dir);
}
