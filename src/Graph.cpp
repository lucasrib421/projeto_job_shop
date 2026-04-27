#include "../include/Graph.hpp"

// Construtor: Inicializa os vetores com o tamanho correto
Graph::Graph(int n) {
    numVertices = n;
    vertices.resize(n);
    adjList.resize(n);
    
    // Inicializa os vértices com valores zerados para evitar lixo de memória
    for (int i = 0; i < n; ++i) {
        vertices[i].id = i;
        vertices[i].inDegree = 0;
        vertices[i].earlyStart = 0;
        vertices[i].earlyFinish = 0;
    }
}

// Configura os dados específicos de uma Operação
void Graph::addVertex(int id, int job, int machine, int weight) {
    vertices[id].id = id;
    vertices[id].job = job;
    vertices[id].machine = machine;
    vertices[id].weight = weight;
}

// Cria uma aresta direcionada de 'fromId' para 'toId'
void Graph::addEdge(int fromId, int toId) {
    adjList[fromId].push_back(toId);
    vertices[toId].inDegree++; // Aumenta o grau de entrada do vértice que recebe a seta
}

// Métodos Getters para acessar as propriedades de forma segura
int Graph::getNumVertices() const {
    return numVertices;
}

const Operation& Graph::getVertex(int id) const {
    return vertices[id];
}

const std::vector<int>& Graph::getNeighbors(int id) const {
    return adjList[id];
}

// Função de Debug: Imprime a lista de adjacência formatada
void Graph::printGraph() const {
    std::cout << "\n--- Estrutura Atual do Grafo ---" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        std::cout << "Vértice " << i 
                  << " [Job " << vertices[i].job 
                  << " | Máq " << vertices[i].machine 
                  << " | Peso " << vertices[i].weight 
                  << " | Grau Entrada " << vertices[i].inDegree << "] -> Aponta para: ";
        
        if (adjList[i].empty()) {
            std::cout << "Nenhum";
        } else {
            for (int neighbor : adjList[i]) {
                std::cout << neighbor << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------------------\n" << std::endl;
}