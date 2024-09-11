# Workload_Scheduling_Rate_Monotonic
Determinação do Escalonamento de Carga de Trabalho usando Rate Monotonic

# 1.0 Install librarie

```shell
sudo apt-get install nlohmann-json3-dev
```

# 2.0 Compile file

```shell
g++ -std=c++11 main_code.cpp -o main_code -lstdc++fs
```

- O parâmetro **g++** é o compilador c++ integrado ao linux

- O parâmetro **-std=c++11** especifica a versão do C++ a ser usada

- O parâmetro **-lstdc++fs** linka a biblioteca de sistema de arquivos necessária se estiver manipulando arquivos JSON diretamente.


# 3.0 run code

```shell
./main_code
```

# 4.0 Calculo do escalonamento 

## 4.1 Para verificar a viabilidade do escalonamento, é usada a fórmula de utilização total:

$$
U = \sum_{i=1}^{n}  \frac{C_{i}}{T_{i}}
$$


##  4.2 O escalonamento é viável se a utilização total não ultrapassar o limite calculado por:

$$
U  \le n \cdot (2^{1/n} - 1)
$$

# 5.0 Libraries

```c++
#include <iostream>
```

- Fornece funcionalidades para entrada e saída padrão em C++, como **std::cout** e **std::cerr**.

```c++
#include <fstream>
```

-  Fornece funcionalidades para operações de entrada e saída de arquivos, como **std::ifstream** e **std::ofstream**.

```c++
#include <vector>
```

-  Fornece a classe **std::vector**, uma estrutura de dados dinâmica para armazenar coleções de elementos.

```c++
#include <cmath>
```

-   Fornece funções matemáticas como **std::pow** para operações matemáticas de precisão flutuante.


```c++
#include <algorithm>
```

-   Fornece funções de algoritmos genéricos como **std::sort** para ordenação e outras operações em coleções.

```c++
#include <nlohmann/json.hpp>
```

- Fornece funcionalidades para manipulação de JSON, incluindo leitura, escrita e formatação de JSON.

```c++
#include <chrono>
```

- Fornece funcionalidades para manipulação de tempo e durações.

```c++
#include <thread>
```

- Fornece funcionalidades para trabalhar com threads, como **std::this_thread::sleep_for**.

# 6.0 Funções e Métodos

## 6.1 calculate_utilization:

```c++
double calculate_utilization(const std::vector<Task>& tasks) {
    double utilization = 0.0;
    for (const auto& task : tasks) {
        utilization += (double)task.execution_time / task.period;
    }
    return utilization;
}
```

 - Calcula a utilização total do sistema como a soma das frações de tempo de execução sobre o período para todas as tarefas.
 - Permite avaliar a carga total do sistema, crucial para determinar a viabilidade do escalonamento.







