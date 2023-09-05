#include "rb.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "../cJSON/cJSON.h"

Arvore noNull;

int inicializarTabela(Tabela *tab)
{
    inicializar(&tab->indices);
    tab->arquivoDados = fopen("dados.json", "a+b");
    char nomeArq[] = "index-codigo.json";
    tab->indices = carregarArquivo(nomeArq, tab->indices);
    if (tab->arquivoDados != NULL)
        return 1;
    else
        return 0;
}

Arvore carregarArquivo(char *nome, Arvore a)
{
    FILE *arq;
    arq = fopen(nome, "rb");
    Indice *temp;
    if (arq != NULL)
    {
        fseek(arq, 0L, SEEK_END);
        long tamanho = ftell(arq);
        fseek(arq, 0L, SEEK_SET);

        char *jsonString = (char *)malloc(tamanho + 1);
        if (!jsonString)
        {
            printf("Erro ao alocar memória para a string JSON.\n");
            fclose(arq);
            exit(-1);
        }

        size_t bytesRead = fread(jsonString, 1, tamanho, arq);
        jsonString[bytesRead] = '\0';

        cJSON *listaJson = cJSON_Parse(jsonString);
        free(jsonString);

        if (!listaJson || !cJSON_IsArray(listaJson))
        {
            printf("Erro ao analisar o JSON.\n");
            cJSON_Delete(listaJson);
            exit(-1);
        }

        int numIndices = cJSON_GetArraySize(listaJson);
        Indice *indices = (Indice *)malloc(numIndices * sizeof(Indice));

        for (int i = 0; i < numIndices; i++)
        {
            cJSON *indiceJson = cJSON_GetArrayItem(listaJson, i);
            cJSON *codigoJson = cJSON_GetObjectItem(indiceJson, "codigo");
            cJSON *indiceValueJson = cJSON_GetObjectItem(indiceJson, "indice");

            if (cJSON_IsNumber(codigoJson) && cJSON_IsNumber(indiceValueJson))
            {
                indices[i].codigo = codigoJson->valueint;
                indices[i].indice = indiceValueJson->valueint;
            }
            else
            {
                printf("Erro ao ler os dados do índice %d.\n", i);
            }
        }

        cJSON_Delete(listaJson);

        for (int i = 0; i < numIndices; i++)
        {
            temp = (Indice *)malloc(sizeof(Indice));
            temp->codigo = indices[i].codigo;
            temp->indice = indices[i].indice;
            adicionar(temp, &a, 1);
        }

        fclose(arq);
    }
    return a;
}

void inicializar(Arvore *raiz)
{
    *raiz = NULL;
    noNull = (Arvore)malloc(sizeof(struct no));
    noNull->cor = DUPLO_PRETO;
    noNull->indice = NULL;
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

void adicionarIndividuo(Tabela *tab, Individuo *individuo)
{
    if (tab->arquivoDados != NULL)
    {
        Indice *novo = (Indice *)malloc(sizeof(Indice));
        novo->codigo = individuo->codigo;
        cJSON *listaJson = NULL;
        char *jsonString = NULL;

        fseek(tab->arquivoDados, 0L, SEEK_END);
        novo->indice = ftell(tab->arquivoDados);

        fseek(tab->arquivoDados, 0L, SEEK_END);
        // Verifique se o arquivo está vazio
        if (ftell(tab->arquivoDados) == 0)
        {
            // O arquivo está vazio, crie uma nova lista JSON
            listaJson = cJSON_CreateArray();
        }
        else
        {
            fseek(tab->arquivoDados, 0L, SEEK_SET);
            char buffer[4096];
            size_t bytesRead = fread(buffer, 1, sizeof(buffer), tab->arquivoDados);
            buffer[bytesRead] = '\0';

            listaJson = cJSON_Parse(buffer);
        }

        fclose(tab->arquivoDados);
        tab->arquivoDados = fopen("dados.json", "w");
        cJSON *json = cJSON_CreateObject();
        cJSON_AddNumberToObject(json, "codigo", individuo->codigo);
        cJSON_AddStringToObject(json, "nome", individuo->nome);
        cJSON_AddItemToArray(listaJson, json);
        jsonString = cJSON_Print(listaJson);

        fprintf(tab->arquivoDados, "%s", jsonString);
        fflush(tab->arquivoDados);

        free(jsonString);
        cJSON_Delete(listaJson);

        fclose(tab->arquivoDados);
        tab->arquivoDados = fopen("dados.json", "a+b");

        adicionar(novo, &(tab->indices), 1);
    }
}

void adicionar(Indice *individuo, Arvore *raiz, int cod)
{
    Arvore posicao, pai, novo;
    posicao = *raiz;
    pai = NULL;

    while (posicao != NULL)
    {
        pai = posicao;
        // se individuo->var > posicao->individuo->var
        if (maiorVariavel(individuo, posicao, cod))
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }
    novo = (Arvore)malloc(sizeof(struct no));
    novo->indice = individuo;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = pai;
    novo->cor = VERMELHO;

    if (isElementoRaiz(novo))
        *raiz = novo;
    else
    {
        // se individuo->var > pai->individuo->var
        if (maiorVariavel(individuo, pai, cod))
            pai->dir = novo;
        else
            pai->esq = novo;
    }

    ajustar(raiz, novo);
}

int maiorVariavel(Indice *individuo, Arvore arvore, int cod)
{
    if (cod)
        return (individuo->codigo > arvore->indice->codigo);

    // int c = strcmp(individuo->nome, arvore->individuo->nome);
    // if (c > 0)
    // {
    //     // printf("c: %d -> %s > %s = %d \n", c, individuo->nome, arvore->individuo->nome, 1);
    //     return 1;
    // }
    // // printf("c: %d -> %s > %s = %d \n", c, individuo->nome, arvore->individuo->nome, 0);
    return 0;
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
        Indice *indice = raiz->indice;
        cJSON_AddItemToObject(node, "codigo", cJSON_CreateNumber(indice->codigo));
        cJSON_AddItemToObject(node, "indice", cJSON_CreateNumber(indice->indice));

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
        if (valor == posicao->indice->codigo)
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
            int maiorValorEsq = maiorElemento(posicao->esq)->indice->codigo;
            posicao->indice->codigo = maiorValorEsq;
            removerRb(posicao->indice->codigo, &(posicao->esq));
            break;
        }
        if (valor > posicao->indice->codigo)
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

void preOrder(Arvore raiz, Tabela *tab)
{
    if (raiz != NULL)
    {
        imprimirElemento(raiz, tab);
        preOrder(raiz->esq, tab);
        preOrder(raiz->dir, tab);
    }
}

void imprimirElemento(Arvore raiz, Tabela *tab)
{
    Individuo *temp = (Individuo *)malloc(sizeof(Individuo));
    temp->codigo = 1000;
    printf("indice: %d\n", raiz->indice->indice);

    fseek(tab->arquivoDados, raiz->indice->indice, SEEK_SET);

    fseek(tab->arquivoDados, 0L, SEEK_SET);
    char buffer[4096];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), tab->arquivoDados);
    buffer[bytesRead] = '\0';

    cJSON *listaJson = NULL;
    listaJson = cJSON_Parse(buffer);

    // Verifique se o parsing foi bem-sucedido
    if (listaJson)
    {
        // Verifique se a lista é um array JSON
        if (cJSON_IsArray(listaJson))
        {
            int arraySize = cJSON_GetArraySize(listaJson);
            for (int i = 0; i < arraySize; i++)
            {
                cJSON *element = cJSON_GetArrayItem(listaJson, i);
                if (element)
                {
                    cJSON *nomeJson = cJSON_GetObjectItem(element, "nome");
                    if (cJSON_IsString(nomeJson))
                    {
                        char *nome = nomeJson->valuestring;
                        printf("[%d, %s]\n", raiz->indice->codigo, nome);
                        continue;
                    }
                }
            }
        }
        cJSON_Delete(listaJson);
    }

    free(temp);
}

int busca(Tabela *tab, int codigo)
{
    Indice *indice = buscarIndice(tab->indices, codigo);

    if (indice != NULL)
    {
        cJSON *listaJson = NULL;
        char *jsonString = NULL;
        fseek(tab->arquivoDados, 0L, SEEK_END);

        if (ftell(tab->arquivoDados) == 0)
        {
            printf("Nenhum dado cadastrado para esse código.\n");
        }
        else
        {
            fseek(tab->arquivoDados, 0L, SEEK_SET);
            char buffer[4096];
            size_t bytesRead = fread(buffer, 1, sizeof(buffer), tab->arquivoDados);
            buffer[bytesRead] = '\0';

            listaJson = cJSON_Parse(buffer);

            for (int i = 0; i < cJSON_GetArraySize(listaJson); i++)
            {
                cJSON *item = cJSON_GetArrayItem(listaJson, i);
                if (cJSON_GetObjectItem(item, "codigo")->valueint == codigo)
                {
                    int codigoEncontrado = cJSON_GetObjectItem(item, "codigo")->valueint;
                    const char *nomeEncontrado = cJSON_GetObjectItem(item, "nome")->valuestring;

                    printf("Código: %d\n", codigoEncontrado);
                    printf("Nome: %s\n", nomeEncontrado);
                }
            }
        }
        return 1;
    }
    else
    {
        printf("Código não encontrado.\n");
        return 0;
    }
}

Indice *buscarIndice(Arvore raiz, int codigo)
{
    Arvore posicao = raiz;
    while (posicao != NULL)
    {
        if (codigo == posicao->indice->codigo)
            return posicao->indice;

        if (codigo > posicao->indice->codigo)
            posicao = posicao->dir;
        else
            posicao = posicao->esq;
    }
    return NULL;
}

void removerDados(Tabela *tab, int codigo)
{
    if (tab->arquivoDados != NULL)
    {
        cJSON *listaJson = NULL;
        char *jsonString = NULL;

        fseek(tab->arquivoDados, 0L, SEEK_END);
        // Verifique se o arquivo está vazio
        if (ftell(tab->arquivoDados) == 0)
        {
            // O arquivo está vazio, crie uma nova lista JSON
            listaJson = cJSON_CreateArray();
        }
        else
        {
            fseek(tab->arquivoDados, 0L, SEEK_SET);
            char buffer[4096];
            size_t bytesRead = fread(buffer, 1, sizeof(buffer), tab->arquivoDados);
            buffer[bytesRead] = '\0';

            listaJson = cJSON_Parse(buffer);
        }

        fclose(tab->arquivoDados);

        int numElementos = cJSON_GetArraySize(listaJson);

        for (int i = 0; i < numElementos; i++)
        {
            cJSON *elemento = cJSON_GetArrayItem(listaJson, i);
            cJSON *codigoJson = cJSON_GetObjectItem(elemento, "codigo");

            if (cJSON_IsNumber(codigoJson) && codigoJson->valueint == codigo)
            {
                cJSON_DeleteItemFromArray(listaJson, i);
            }
        }

        tab->arquivoDados = fopen("dados.json", "w");

        jsonString = cJSON_Print(listaJson);

        fprintf(tab->arquivoDados, "%s", jsonString);
        fflush(tab->arquivoDados);

        free(jsonString);
        cJSON_Delete(listaJson);

        fclose(tab->arquivoDados);
        tab->arquivoDados = fopen("dados.json", "a+b");
    }
}