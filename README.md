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