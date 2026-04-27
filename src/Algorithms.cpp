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