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
    std::cout << "Schedulability: " << (schedulable ? "viable" : "not viable") << std::endl;
    std::cout << "Suggested Schedule:\n";
    for (size_t i = 0; i < schedule.size(); ++i) {
        std::cout << "  Task ID: " << schedule[i].id << ", Priority: " << i + 1 << std::endl;
    }

    // Salvar a saída no arquivo JSON
    save_output(schedule, schedulable);

    return 0;
}
