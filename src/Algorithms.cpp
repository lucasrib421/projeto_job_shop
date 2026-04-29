#include "../include/Algorithms.hpp"
#include <queue>
#include <iostream>
#include <algorithm>

std::vector<int> Algorithms::topologicalSort(const Graph& graph) {
    int n = graph.getNumVertices();
    std::vector<int> inDegreeCopy(n, 0);
    std::queue<int> zeroInDegreeQueue;

    // 1. Copiamos os graus de entrada do grafo para não alterarmos a estrutura original
    // e já enfileiramos os Elementos Minimais (grau de entrada == 0)
    for (int i = 0; i < n; ++i) {
        inDegreeCopy[i] = graph.getVertex(i).inDegree;
        if (inDegreeCopy[i] == 0) {
            zeroInDegreeQueue.push(i);
        }
    }

    std::vector<int> topoOrder;

    // 2. Processamos a fila
    while (!zeroInDegreeQueue.empty()) {
        int u = zeroInDegreeQueue.front();
        zeroInDegreeQueue.pop();
        
        topoOrder.push_back(u); // Coloca na ordem topológica

        // Para cada vizinho de 'u', "removemos" a aresta diminuindo o grau de entrada
        for (int v : graph.getNeighbors(u)) {
            inDegreeCopy[v]--;
            // Se o vizinho passou a ter grau 0, ele está pronto para ser processado
            if (inDegreeCopy[v] == 0) {
                zeroInDegreeQueue.push(v);
            }
        }
    }

    // 3. Verificação de Ciclo (Crucial para o Job Shop)
    if (topoOrder.size() != (size_t)n) {
        std::cerr << "ALERTA: Ciclo detectado! A heuristica criou um deadlock." << std::endl;
        return {}; // Retorna vetor vazio indicando falha
    }

    return topoOrder;
}

int Algorithms::calculateMakespan(const Graph& graph, const std::vector<int>& topoOrder) {
    int n = graph.getNumVertices();
    int makespan = 0;

    // Vetores para rastrear os tempos de cada operação
    std::vector<int> earlyStart(n, 0);
    std::vector<int> earlyFinish(n, 0);

    // Percorremos os vértices exatamente na ordem topológica
    for (int u : topoOrder) {
        int weight = graph.getVertex(u).weight;
        
        // O término mais cedo é o início mais cedo + tempo de execução
        earlyFinish[u] = earlyStart[u] + weight;

        // Atualiza o Makespan global se essa tarefa terminar mais tarde que o recorde atual
        if (earlyFinish[u] > makespan) {
            makespan = earlyFinish[u];
        }

        // Propaga o tempo para os vizinhos (tarefas dependentes)
        // O vizinho não pode começar antes que 'u' termine!
        for (int v : graph.getNeighbors(u)) {
            if (earlyFinish[u] > earlyStart[v]) {
                earlyStart[v] = earlyFinish[u];
            }
        }
    }

    return makespan;
}

std::vector<int> Algorithms::getMinimalElements(const Graph& graph) {
    std::vector<int> minimal;
    for (int i = 0; i < graph.getNumVertices(); ++i) {
        if (graph.getVertex(i).inDegree == 0) {
            minimal.push_back(i);
        }
    }
    return minimal;
}

std::vector<int> Algorithms::getMaximalElements(const Graph& graph) {
    std::vector<int> maximal;
    for (int i = 0; i < graph.getNumVertices(); ++i) {
        // Grau de saída é o número de vizinhos na lista de adjacência
        if (graph.getNeighbors(i).empty()) {
            maximal.push_back(i);
        }
    }
    return maximal;
}

std::vector<int> Algorithms::getCriticalPath(const Graph& graph, const std::vector<int>& topoOrder) {
    int n = graph.getNumVertices();
    std::vector<int> earlyStart(n, 0);
    std::vector<int> earlyFinish(n, 0);
    
    // Vetor mágico que guarda "de onde vim"
    std::vector<int> predecessor(n, -1); 

    int makespan = 0;
    int lastNode = -1; // Guarda o ID da tarefa que terminou por último

    for (int u : topoOrder) {
        int weight = graph.getVertex(u).weight;
        earlyFinish[u] = earlyStart[u] + weight;

        // Se essa tarefa terminar mais tarde que o recorde, ela é o novo fim do projeto
        if (earlyFinish[u] > makespan) {
            makespan = earlyFinish[u];
            lastNode = u;
        }

        // Propaga o tempo para os vizinhos
        for (int v : graph.getNeighbors(u)) {
            if (earlyFinish[u] > earlyStart[v]) {
                earlyStart[v] = earlyFinish[u];
                predecessor[v] = u; // Marca 'u' como o culpado pelo atraso de 'v'
            }
        }
    }

    // Agora fazemos o caminho reverso (do último nó até o primeiro)
    std::vector<int> criticalPath;
    int current = lastNode;
    while (current != -1) {
        criticalPath.push_back(current);
        current = predecessor[current]; // Dá um passo para trás
    }

    // Como pegamos do fim pro começo, precisamos inverter o vetor para a ordem correta
    std::reverse(criticalPath.begin(), criticalPath.end());
    
    return criticalPath;
}