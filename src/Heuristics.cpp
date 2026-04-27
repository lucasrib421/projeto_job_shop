#include "../include/Heuristics.hpp"
#include <vector>
#include <list>

Graph Heuristics::applySPT(const Graph& initialGraph, int totalJobs, int totalMachines) {
    int n = initialGraph.getNumVertices();
    
    // Criamos uma cópia do grafo para ser o nosso Grafo Final (onde vamos adicionar as setas)
    Graph finalGraph = initialGraph;

    // Vetor para guardar o ID da ÚLTIMA operação que usou cada máquina.
    // Inicializamos com -1 (indicando que a máquina está livre)
    std::vector<int> lastOpOnMachine(totalMachines, -1);

    // Precisamos de uma cópia dos graus de entrada para simular a execução
    std::vector<int> inDegree(n);
    std::list<int> readyOperations; // Lista de tarefas que já podem ser processadas

    for (int i = 0; i < n; ++i) {
        inDegree[i] = initialGraph.getVertex(i).inDegree;
        if (inDegree[i] == 0) {
            readyOperations.push_back(i); // A primeira operação de cada Job entra aqui
        }
    }

    // Enquanto houver operações prontas para serem agendadas
    while (!readyOperations.empty()) {
        
        // --- AQUI ENTRA A HEURÍSTICA (REGRA DE DECISÃO) ---
        // SPT: Escolher a operação pronta com o MENOR tempo de processamento
        auto bestIt = readyOperations.begin();
        int minWeight = finalGraph.getVertex(*bestIt).weight;

        for (auto it = readyOperations.begin(); it != readyOperations.end(); ++it) {
            int currentWeight = finalGraph.getVertex(*it).weight;
            if (currentWeight < minWeight) {
                minWeight = currentWeight;
                bestIt = it;
            }
        }

        // Removemos o vencedor da lista de prontos
        int u = *bestIt;
        readyOperations.erase(bestIt);

        // --- RESOLVENDO O CONFLITO DA MÁQUINA ---
        int machine = finalGraph.getVertex(u).machine;
        
        // Se alguém já usou essa máquina antes, a tarefa 'u' tem que esperar!
        // Então criamos uma aresta: TarefaAntiga -> TarefaNova(u)
        if (lastOpOnMachine[machine] != -1) {
            finalGraph.addEdge(lastOpOnMachine[machine], u);
        }
        
        // Agora 'u' é a última operação a usar esta máquina
        lastOpOnMachine[machine] = u;

        // --- LIBERANDO AS PRÓXIMAS TAREFAS DO JOB ---
        // A tarefa 'u' terminou. Vamos avisar a próxima tarefa do mesmo Job que ela pode começar
        for (int vizinho : initialGraph.getNeighbors(u)) {
            inDegree[vizinho]--;
            if (inDegree[vizinho] == 0) {
                readyOperations.push_back(vizinho);
            }
        }
    }

    return finalGraph;
}

Graph Heuristics::applyLPT(const Graph& initialGraph, int totalJobs, int totalMachines) {
    int n = initialGraph.getNumVertices();
    Graph finalGraph = initialGraph;
    std::vector<int> lastOpOnMachine(totalMachines, -1);
    std::vector<int> inDegree(n);
    std::list<int> readyOperations;

    for (int i = 0; i < n; ++i) {
        inDegree[i] = initialGraph.getVertex(i).inDegree;
        if (inDegree[i] == 0) {
            readyOperations.push_back(i);
        }
    }

    while (!readyOperations.empty()) {
        
        // --- AQUI MUDA A REGRA DE DECISÃO ---
        // LPT: Escolher a operação com o MAIOR tempo de processamento
        auto bestIt = readyOperations.begin();
        int maxWeight = finalGraph.getVertex(*bestIt).weight;

        for (auto it = readyOperations.begin(); it != readyOperations.end(); ++it) {
            int currentWeight = finalGraph.getVertex(*it).weight;
            // A ÚNICA diferença para o SPT está nesta linha (sinal de >)
            if (currentWeight > maxWeight) {
                maxWeight = currentWeight;
                bestIt = it;
            }
        }

        int u = *bestIt;
        readyOperations.erase(bestIt);

        int machine = finalGraph.getVertex(u).machine;
        
        if (lastOpOnMachine[machine] != -1) {
            finalGraph.addEdge(lastOpOnMachine[machine], u);
        }
        
        lastOpOnMachine[machine] = u;

        for (int vizinho : initialGraph.getNeighbors(u)) {
            inDegree[vizinho]--;
            if (inDegree[vizinho] == 0) {
                readyOperations.push_back(vizinho);
            }
        }
    }

    return finalGraph;
}