# Workload_Scheduling_Rate_Monotonic
Determinação do Escalonamento de Carga de Trabalho usando Rate Monotonic

# 1.0 Install libraries

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


 ## 6.2 is_schedulable:

```c++
bool is_schedulable(const std::vector<Task>& tasks) {
    int n = tasks.size();
    double utilization = calculate_utilization(tasks);
    
    // Limite de utilização para Rate Monotonic
    double rm_limit = n * (std::pow(2.0, 1.0 / n) - 1.0);
    
    return utilization <= rm_limit;
}
```

 - Verifica se a carga do sistema é menor ou igual ao limite de utilização para o Rate Monotonic Scheduling.
 - Utiliza a fórmula para o limite de utilização de RMS para determinar se o conjunto de tarefas pode ser escalonado.


 ## 6.3 rate_monotonic_schedule:

```c++
std::vector<Task> rate_monotonic_schedule(std::vector<Task>& tasks) {
    // Ordena as tarefas por período, menor período tem maior prioridade
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.period < b.period;
    });
    return tasks;
}
```

 - Ordena as tarefas por período, garantindo que a tarefa com menor período (e, portanto, maior prioridade) seja executada primeiro.
 -  A ordenação é essencial para o escalonamento de acordo com a política Rate Monotonic.

 ## 6.4 save_output:

```c++
void save_output(const std::vector<Task>& tasks, bool schedulable) {
    json output;
    output["schedulability"] = schedulable ? "viable" : "not viable";

    json suggested_schedule = json::array();
    int priority = 1;
    for (const auto& task : tasks) {
        suggested_schedule.push_back({
            {"id", task.id},
            {"priority", priority++}
        });
    }
    output["suggested_schedule"] = suggested_schedule;

    std::ofstream file("output_tarefas.json");
    if (file.is_open()) {
        file << output.dump(4); // Formata o JSON com 4 espaços de indentação
        file.close();
    } else {
        std::cerr << "Não foi possível abrir o arquivo de saída.\n";
    }
}
```

 - Salva o resultado da análise e o escalonamento sugerido em um arquivo JSON.
 - Utiliza a biblioteca nlohmann::json para manipulação de JSON, proporcionando uma maneira estruturada de salvar e visualizar os resultados.



## 6.5 cyclic_executive:

```c++
void cyclic_executive(const std::vector<Task>& tasks, int cycle_time_ms) {
    auto cycle_duration = std::chrono::milliseconds(cycle_time_ms);
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        
        // Executar tarefas
        for (const auto& task : tasks) {
            // Simula a execução da tarefa
            std::cout << "Executing Task ID: " << task.id << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(task.execution_time));
        }

        // Calcular o tempo restante no ciclo
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        auto sleep_time = cycle_duration - elapsed;

        // Espera pelo restante do ciclo
        if (sleep_time.count() > 0) {
            std::this_thread::sleep_for(sleep_time);
        } else {
            std::cerr << "Warning: Task execution time exceeded cycle time!" << std::endl;
        }
    }
}
```

 - Executa as tarefas em um ciclo contínuo, onde o tempo de ciclo é definido pelo usuário. Ajusta o tempo de espera para manter o ciclo.
 -  Implementa a execução real das tarefas e garante que o tempo de ciclo seja respeitado, ajustando o tempo de espera conforme necessário.


## 6.6 main:

```c++
int main() {
    // Abrir o arquivo JSON de entrada
    std::ifstream file("tarefas.json");
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo de entrada.\n";
        return 1;
    }

    json task_json;
    file >> task_json;
    file.close();

    std::vector<Task> tasks;
    
    // Processar as tarefas do arquivo JSON
    for (const auto& task : task_json["tasks"]) {
        tasks.push_back({task["id"], task["period"], task["execution_time"]});
    }

    // Calcular se o escalonamento é viável
    bool schedulable = is_schedulable(tasks);

    // Sugerir um escalonamento usando Rate Monotonic
    std::vector<Task> schedule = rate_monotonic_schedule(tasks);

    // Exibir o resultado no terminal
    
    if (schedulable) {
        std::cout << "\nSchedulability: " << (schedulable ? "viable" : "not viable") << std::endl;
        std::cout << "Suggested Schedule:\n";
        for (size_t i = 0; i < schedule.size(); ++i) {
            std::cout << "  Task ID: " << schedule[i].id << ", Priority: " << i + 1 << std::endl;
        }
    }
    else {
        std::cout << "\nSchedulability: " << (schedulable ? "viable" : "not viable") << std::endl;
    }
    
    // Salvar a saída no arquivo JSON
    save_output(schedule, schedulable);

    // Iniciar o executivo cíclico com o tempo de ciclo definido
    int cycle_time_ms = 5000000; // Tempo de ciclo em milissegundos (ajustar conforme necessário)
    cyclic_executive(schedule, cycle_time_ms);

    return 0;
}
```

 - Função principal que orquestra a leitura das tarefas, a análise de escalonabilidade, a sugestão de escalonamento, e a execução das tarefas.
 - Coordena todas as operações do sistema, garantindo que as tarefas sejam lidas, processadas e executadas conforme o planejado.

 # 7.0 Escolhas de Design

 ## 7.1 Escalonamento Rate Monotonic:

 Optou-se por usar o escalonamento Rate Monotonic, que é um algoritmo de escalonamento de prioridade fixa baseado em períodos de tarefas. A decisão foi baseada na sua simplicidade e eficácia para sistemas de tempo real com tarefas periódicas e o pre-requisito do projeto.

 ## 7.2 Manipulação de JSON com **nlohmann::json**:

 A biblioteca **nlohmann::json** foi escolhida por sua interface intuitiva e suporte para manipulação de JSON em C++. Facilita a leitura e escrita de dados no formato JSON de forma eficiente.

 ## 7.3 Ciclo Contínuo com **std::this_thread::sleep_for**:

 Implementa o executivo cíclico que garante que as tarefas sejam executadas em um ciclo contínuo. O ajuste de tempo é feito para assegurar que o tempo de ciclo seja respeitado, prevenindo atrasos na execução.



