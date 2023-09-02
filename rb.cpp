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

    novo = (Arvore)malloc(sizeof(struct no));
    novo->individuo = individuo;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;
    novo->cor = VERMELHO;

    *raiz = novo;
}

void salvarArquivo(char *nomeArquivo, Arvore arvore)
{
    if (arvore == NULL)
    {
        printf("A árvore está vazia ou não inicializada.\n");
        return;
    }

    Individuo *individuo = arvore->individuo;

    if (individuo == NULL)
    {
        printf("O nó da árvore não possui um Individuo válido.\n");
        return;
    }

    cJSON *root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, "codigo", cJSON_CreateNumber(individuo->codigo));
    cJSON_AddItemToObject(root, "nome", cJSON_CreateString(individuo->nome));
    printf("%s", individuo->nome);

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo)
    {
        char *jsonStr = cJSON_Print(root);
        fputs(jsonStr, arquivo);
        fclose(arquivo);
        free(jsonStr);
    }

    cJSON_Delete(root);
}

void salvarAux(Arvore raiz, FILE *arq)
{
    if (raiz != NULL)
    {
        // fwrite(raiz->individuo, sizeof(Individuo), 1, arq);
        // salvar_auxiliar(raiz->esq, arq);
        // salvar_auxiliar(raiz->dir, arq);
    }
}