#include <iostream>
#include <fstream> // Necessário para manipular arquivos
#include <iomanip>
#include "../include/JobShopModeler.hpp"
#include "../include/Graph.hpp"
#include "../include/Algorithms.hpp"
#include "../include/Heuristics.hpp"

// Função auxiliar que agora aceita um ostream (pode ser cout ou um arquivo)
void generateReport(std::ostream& os, 
                    const std::string& filename,
                    int makespan, 
                    const std::vector<int>& minimais, 
                    const std::vector<int>& maximais, 
                    const std::vector<int>& caminho) {
    
    os << "\n==========================================" << std::endl;
    os << "        RELATORIO FINAL DO GRAFO          " << std::endl;
    os << "==========================================" << std::endl;
    os << "Arquivo de Instancia: " << filename << std::endl;
    os << "Algoritmo Aplicado:   Heuristica LPT" << std::endl;
    os << "Makespan Total:       " << makespan << std::endl;
    os << "------------------------------------------" << std::endl;

    os << "Elementos Minimais (" << minimais.size() << "): ";
    for (size_t i = 0; i < minimais.size() && i < 15; ++i) 
        os << minimais[i] << (i == minimais.size() - 1 ? "" : ", ");
    if (minimais.size() > 15) os << "...";
    os << std::endl;

    os << "Elementos Maximais (" << maximais.size() << "): ";
    for (size_t i = 0; i < maximais.size() && i < 15; ++i) 
        os << maximais[i] << (i == maximais.size() - 1 ? "" : ", ");
    if (maximais.size() > 15) os << "...";
    os << std::endl;

    os << "------------------------------------------" << std::endl;
    os << "Caminho Critico (" << caminho.size() << " vertices):" << std::endl;
    
    for (size_t i = 0; i < caminho.size(); ++i) {
        os << caminho[i] << (i < caminho.size() - 1 ? " -> " : "");
        if ((i + 1) % 10 == 0) os << "\n";
    }
    os << "\n==========================================" << std::endl;
}

int main() {
    JobShopModeler modeler;
    std::string path = "../data/JSP/yn2Jsp.psi";
    
    if (modeler.parseFile(path)) {
        Graph grafoInicial = modeler.buildInitialGraph();
        Graph grafoFinal = Heuristics::applyLPT(grafoInicial, modeler.getTotalJobs(), modeler.getTotalMachines());
        std::vector<int> ordem = Algorithms::topologicalSort(grafoFinal);
        int makespan = Algorithms::calculateMakespan(grafoFinal, ordem);
        
        auto minimais = Algorithms::getMinimalElements(grafoFinal);
        auto maximais = Algorithms::getMaximalElements(grafoFinal);
        auto caminhoCritico = Algorithms::getCriticalPath(grafoFinal, ordem);

        // 1. Mostrar no Terminal
        generateReport(std::cout, path, makespan, minimais, maximais, caminhoCritico);

        // 2. Salvar em Arquivo TXT
        std::string outName = "resultado_yn4Jsp.txt";
        std::ofstream outFile(outName);
        
        if (outFile.is_open()) {
            generateReport(outFile, path, makespan, minimais, maximais, caminhoCritico);
            outFile.close();
            std::cout << "\n[INFO] Resultado salvo com sucesso em: " << outName << std::endl;
        } else {
            std::cerr << "\n[ERRO] Nao foi possivel criar o arquivo de saida." << std::endl;
        }

    } else {
        std::cerr << "Erro ao abrir arquivo de instancia." << std::endl;
    }
    return 0;
}