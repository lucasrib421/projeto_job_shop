#include <iostream>
#include "../include/Graph.hpp"
#include "../include/Algorithms.hpp"

int main() {
    Graph grafoProf(16);

    // Definindo as arestas solicitadas
    grafoProf.addEdge(1, 2);  grafoProf.addEdge(1, 11);
    grafoProf.addEdge(2, 12);
    grafoProf.addEdge(3, 5);
    grafoProf.addEdge(4, 3);  grafoProf.addEdge(4, 1);
    grafoProf.addEdge(5, 14); grafoProf.addEdge(5, 15);
    grafoProf.addEdge(7, 8);  grafoProf.addEdge(7, 10);
    grafoProf.addEdge(8, 2);  grafoProf.addEdge(8, 14);
    grafoProf.addEdge(10, 1); grafoProf.addEdge(10, 3);
    grafoProf.addEdge(11, 14); grafoProf.addEdge(11, 15);
    grafoProf.addEdge(12, 9); grafoProf.addEdge(12, 13);
    grafoProf.addEdge(13, 6);
    grafoProf.addEdge(14, 13);
    grafoProf.addEdge(15, 6); grafoProf.addEdge(15, 9);

    // --- A SOLUÇÃO ESTÁ AQUI ---
    // Definimos peso 1 para todos os vértices de 1 a 15
    for(int i = 1; i <= 15; i++) {
        grafoProf.setWeight(i, 1);
    }

    std::vector<int> ordem = Algorithms::topologicalSort(grafoProf);
    int makespan = Algorithms::calculateMakespan(grafoProf, ordem);
    auto caminhoCritico = Algorithms::getCriticalPath(grafoProf, ordem);

    std::cout << "\n--- TESTE DO PROFESSOR ---\n";
    std::cout << "Tamanho Maximo: " << makespan << " passos.\n";
    std::cout << "Caminho Maximo Absoluto: ";
    for (size_t i = 0; i < caminhoCritico.size(); ++i) {
        std::cout << caminhoCritico[i] << (i < caminhoCritico.size() - 1 ? " -> " : "");
    }
    std::cout << "\n--------------------------\n";

    return 0;
}