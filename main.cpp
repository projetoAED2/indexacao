#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

int main()
{
    Tabela tabela;
    int opcao, codigo;
    char arqDados[] = "dados.json";
    char arqCodigo[] = "index-codigo.json";
    char nome[51];
    inicializarTabela(&tabela);

    while (1)
    {
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            adicionarIndividuo(&tabela, lerDados());
            break;
        case 2:
            salvarArquivo(arqCodigo, tabela.indices);
            break;
        case 3:
            scanf("%d", &codigo);
            busca(&tabela, codigo);
            break;
        case 4:
            scanf("%d", &codigo);
            removerRb(codigo, &tabela.indices);
            removerDados(&tabela, codigo);
            salvarArquivo(arqCodigo, tabela.indices);
            inicializarTabela(&tabela);
            break;
        case 99:
            salvarArquivo(arqCodigo, tabela.indices);
            exit(0);
        }
    }
}
