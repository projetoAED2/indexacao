# Instalar o g++
```
sudo apt-get install g++
```

# Utilizamos a bibliotesca cJSON
https://github.com/DaveGamble/cJSON.git

# Dentro da raiz desse projeto
```
export LD_LIBRARY_PATH=../cJSON/build:$LD_LIBRARY_PATH
```

# Comando para compilar: 
```
g++ *.cpp -o programa -L ../cJSON/build -lcjson
```

# Comando para executar:
```
./programa
```

# Executar testes

Indexação de código:
```
./programa < teste-index-codigo.in
diff index-codigo.json teste-index-codigo-out.json
```

Indexação de nome:
```
./programa < teste-index-nome.in 
diff index-nome.json teste-index-nome-out.json
```