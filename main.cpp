#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

int main()
{
    Arvore a;
    int opcao, codigo;
    char nomeArquivo[] = "dados.json";
    inicializar(&a);

    while (1)
    {
        scanf("%d", &opcao);
        switch (opcao)
        {
            int valor;
        case 1:
            adicionar(lerDados(), &a);
            break;
        case 2:
            salvarArquivo(nomeArquivo, a);
            break;
        case 3:
            scanf("%d\n", &codigo);
            removerRb(codigo, &a);
            break;
        case 99:
            exit(0);
        }
    }
}