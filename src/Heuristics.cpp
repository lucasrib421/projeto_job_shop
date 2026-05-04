#include "../include/Heuristics.hpp"
#include "../include/Algorithms.hpp"
#include <vector>
#include <list>
#include <algorithm>

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

Graph Heuristics::applyMWKR(Graph grafoInicial, int jobs, int machines) {
    int totalVertices = grafoInicial.getNumVertices();
    
    // Vetor para guardar o MWKR calculado de cada operação (id)
    std::vector<int> mwkr(totalVertices, 0);

    // 1. CÁLCULO DO MWKR (Trabalho Restante)
    // Pegamos a ordem topológica para conseguir percorrer o grafo de trás para frente
    std::vector<int> ordem = Algorithms::topologicalSort(grafoInicial);
    
    for (int i = ordem.size() - 1; i >= 0; --i) {
        int u = ordem[i];
        int maxTempoSucessores = 0;
        
        // Percorre os vizinhos (próximas operações do mesmo Job)
        const std::vector<int>& vizinhos = grafoInicial.getNeighbors(u);
        for (int v : vizinhos) {
            if (mwkr[v] > maxTempoSucessores) {
                maxTempoSucessores = mwkr[v];
            }
        }
        
        // MWKR de 'u' = Seu próprio peso + o maior caminho restante à frente
        mwkr[u] = grafoInicial.getVertex(u).weight + maxTempoSucessores;
    }

    // 2. RESOLUÇÃO DOS CONFLITOS NAS MÁQUINAS
    Graph grafoFinal = grafoInicial;
    
    // Agrupa os IDs das operações pela máquina onde elas vão rodar
    std::vector<std::vector<int>> operacoesPorMaquina(machines);
    for (int i = 0; i < totalVertices; ++i) {
        int maq = grafoInicial.getVertex(i).machine;
        operacoesPorMaquina[maq].push_back(i);
    }

    // Para cada máquina, ordenamos a fila de tarefas e criamos as arestas de restrição
    for (int maq = 0; maq < machines; ++maq) {
        std::vector<int>& filaDaMaquina = operacoesPorMaquina[maq];

        // Ordenação MWKR: Quem tem o maior trabalho restante ganha prioridade
        std::sort(filaDaMaquina.begin(), filaDaMaquina.end(), [&mwkr](int idA, int idB) {
            return mwkr[idA] > mwkr[idB]; // Sinal de maior (>) garante a regra MWKR
        });

        // Cria arestas direcionadas garantindo a ordem na máquina (A -> B -> C...)
        for (size_t i = 0; i < filaDaMaquina.size() - 1; ++i) {
            grafoFinal.addEdge(filaDaMaquina[i], filaDaMaquina[i+1]);
        }
    }

    return grafoFinal;
}