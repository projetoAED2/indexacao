#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

int main()
{
    Arvore indexCodigo, indexNome;
    Individuo *individuo;
    int opcao, codigo;
    char arqCodigo[] = "index-codigo.json";
    char arqNome[] = "index-nome.json";
    char nome[51];
    inicializar(&indexCodigo);
    inicializar(&indexNome);

    while (1)
    {
        scanf("%d", &opcao);
        switch (opcao)
        {
            int valor;
        case 1:
            individuo = lerDados();
            adicionar(individuo, &indexCodigo, 1);
            adicionar(individuo, &indexNome, 0);
            break;
        case 2:
            salvarArquivo(arqCodigo, indexCodigo);
            salvarArquivo(arqNome, indexNome);
            break;
        case 3:
            scanf("%d\n", &codigo);
            removerRb(codigo, &indexCodigo);
            break;
        case 99:
            exit(0);
        }
    }
}