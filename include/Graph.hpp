#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>

// Nossa estrutura que representa um VÉRTICE no Grafo.
// No contexto do Job Shop, um vértice é uma "Operação" específica.
struct Operation {
    int id;           // Um identificador único para o vértice (ex: de 0 a 99)
    int job;          // A qual Job esta operação pertence
    int machine;      // Em qual Máquina será processada
    int weight;       // Tempo de processamento (peso do vértice, tirado da matriz 'Costs')
    
    int inDegree;     // Grau de entrada: Quantas arestas apontam para cá? 
                      // (Essencial para achar os elementos "minimais" depois)

    // Variáveis cruciais para o Passo 5 (Cálculo do Makespan)
    int earlyStart;   // Tempo mais cedo que pode começar
    int earlyFinish;  // Tempo mais cedo que pode terminar (earlyStart + weight)
};

// A classe que gerencia o Grafo inteiro
class Graph {
private:
    int numVertices;
    
    // Vetor que guarda todos os nossos vértices (as operações)
    std::vector<Operation> vertices; 
    
    // Lista de Adjacência: adjList[u] guarda uma lista de IDs para onde 'u' aponta
    std::vector<std::vector<int>> adjList; 

public:
    // Construtor: dizemos quantos vértices o grafo terá logo de cara
    Graph(int n);

    // Função para configurar um vértice com os dados do arquivo
    void addVertex(int id, int job, int machine, int weight);

    // Função para criar uma aresta direcionada (de 'fromId' para 'toId')
    void addEdge(int fromId, int toId);

    // Funções auxiliares (getters) para acessarmos os dados depois
    int getNumVertices() const;
    const Operation& getVertex(int id) const;
    const std::vector<int>& getNeighbors(int id) const;
    
    // Função para ajudar no debug
    void printGraph() const;

    void setWeight(int u, int weight);
};



#endif