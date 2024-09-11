# Workload_Scheduling_Rate_Monotonic
Determinação do Escalonamento de Carga de Trabalho usando Rate Monotonic

# install librare

```
sudo apt-get install nlohmann-json3-dev
```

# compile file

```
g++ -std=c++11 rate_monotonic.cpp -o rate_monotonic -lstdc++fs
```

- O parâmetro **g++** é o compilador c++ integrado ao linux

- O parâmetro **-std=c++11** especifica a versão do C++ a ser usada

- O parâmetro **-lstdc++fs** linka a biblioteca de sistema de arquivos necessária se estiver manipulando arquivos JSON diretamente.


# run code

```
./rate_monotonic
```