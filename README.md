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

# Para verificar a viabilidade do escalonamento, é usada a fórmula de utilização total:

$$
U = \sum_{i=1}^{n}  \frac{C_{i}}{T_{i}}
$$


#  O escalonamento é viável se a utilização total não ultrapassar o limite calculado por:

$$
U  \le n \cdot (2^{1/n} - 1)
$$