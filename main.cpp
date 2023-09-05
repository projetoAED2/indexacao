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
        case 99:
            salvarArquivo(arqCodigo, tabela.indices);
            exit(0);
        }
    }
}




