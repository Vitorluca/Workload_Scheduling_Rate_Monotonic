#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <nlohmann/json.hpp> // Certifique-se de ter a biblioteca nlohmann/json instalada

using json = nlohmann::json;

struct Task {
    std::string id;
    int period;
    int execution_time;
};

double calculate_utilization(const std::vector<Task>& tasks) {
    double utilization = 0.0;
    for (const auto& task : tasks) {
        utilization += (double)task.execution_time / task.period;
    }
    return utilization;
}

bool is_schedulable(const std::vector<Task>& tasks) {
    int n = tasks.size();
    double utilization = calculate_utilization(tasks);
    
    // Limite de utilização para Rate Monotonic
    double rm_limit = n * (std::pow(2.0, 1.0 / n) - 1.0);
    
    return utilization <= rm_limit;
}

std::vector<Task> rate_monotonic_schedule(std::vector<Task>& tasks) {
    // Ordena as tarefas por período, menor período tem maior prioridade
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.period < b.period;
    });
    return tasks;
}

int main() {
    // Abrir o arquivo JSON de entrada
    std::ifstream file("tarefas.json");
    if (!file.is_open()) {
        std::cerr << "Não foi possível abrir o arquivo.\n";
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
    if (is_schedulable(tasks)) {
        std::cout << "Schedulability: viable\n";
    } else {
        std::cout << "Schedulability: not viable\n";
    }

    // Sugerir um escalonamento usando Rate Monotonic
    std::vector<Task> schedule = rate_monotonic_schedule(tasks);
    
    // Exibir o escalonamento sugerido
    std::cout << "Suggested Schedule:\n";
    for (size_t i = 0; i < schedule.size(); ++i) {
        std::cout << "  Task ID: " << schedule[i].id << ", Priority: " << i + 1 << "\n";
    }

    return 0;
}
