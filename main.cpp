#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

int main()
{
    Arvore indexCodigo, indexNome;
    Individuo *individuo;
    Tabela tabela;
    int opcao, codigo;
    char arqCodigo[] = "index-codigo.json";
    char arqNome[] = "index-nome.json";
    char nome[51];
    inicializarTabela(&tabela);
    // inicializar(&indexCodigo);
    // inicializar(&indexNome);

    while (1)
    {
        scanf("%d", &opcao);
        switch (opcao)
        {
            int valor;
        case 1:
            adicionarIndividuo(&tabela, lerDados());
            break;
        case 2:
            salvarArquivo(arqCodigo, tabela.indices);
            break;
        case 3:
            scanf("%d\n", &codigo);
            removerRb(codigo, &indexCodigo);
            salvarArquivo(arqCodigo, tabela.indices);
            break;
        case 4:
            preOrder(tabela.indices, &tabela);
            break;
        case 99:
            // salva a indexação antes de encerrar o programa
            salvarArquivo(arqCodigo, tabela.indices);
            exit(0);
        }
    }
}