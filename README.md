# Atividade Indexação

Atividade desenvolvida como pré-requisito para aprovação na disciplina de AED II.

> Desenvolvido por: Izabel Nascimento, Rochel Angelo e Vandielson Tenório.

# Indexação de individuos

Estrutura de um indivíduo:
```
{
    "codigo": 123,
    "nome": "Nome do individuo variavel"
}
```

Utilizamos a bibliotesca cJSON:
https://github.com/DaveGamble/cJSON.git

# Para rodar o projeto

Instalar o g++
```
sudo apt-get install g++
```

Na raiz do projeto:
```
export LD_LIBRARY_PATH=../cJSON/build:$LD_LIBRARY_PATH
```

Comando para compilar: 
```
g++ *.cpp -o programa -L ../cJSON/build -lcjson
```

Comando para executar:
```
./programa
```

# Executar testes 
(obs: o arquivos dados.json, index-codigo.json e index-nome.json dever ter array vazio `[]`)

Indexação de código:
```
./programa < teste-index-codigo.in
diff index-codigo.json teste-index-codigo-out.json
```

Indexação de nome (não implementado ainda):
```
./programa < teste-index-nome.in 
diff index-nome.json teste-index-nome-out.json
```
