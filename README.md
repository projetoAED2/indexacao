# Instalar o g++
`sudo apt-get install g++`

# Configurar a bibliotesca cJSON
`git clone https://github.com/DaveGamble/cJSON.git`

(dentro da raiz do projeto clonado)
`mkdir build`
`cd build`
`cmake ..`
`make`

# Dentro da raiz desse projeto
`export LD_LIBRARY_PATH=/home/izabel/repositorios/ufape/cJSON/build:$LD_LIBRARY_PATH`

# Comando para compilar: 
`g++ *.cpp -o programa -L/home/izabel/repositorios/ufape/cJSON/build -lcjson`
# Comando para executar:
 `./programa`