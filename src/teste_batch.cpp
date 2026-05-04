#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <dirent.h>
#include <algorithm>
#include "../include/JobShopModeler.hpp"
#include "../include/Graph.hpp"
#include "../include/Algorithms.hpp"
#include "../include/Heuristics.hpp"

int main() {
    std::string basePath = "../data/JSP/";
    std::string csvPath = "../resultados_batch.csv";

    // Prepara o arquivo CSV e escreve o cabeçalho
    std::ofstream csvFile(csvPath);
    if (csvFile.is_open()) {
        csvFile << "sep=;\n"; 
        csvFile << "Instancia;Jobs;Maquinas;Makespan LPT;Makespan SPT;Makespan MWKR;Melhor\n";
    } else {
        std::cerr << "[ERRO] Nao foi possivel criar o arquivo CSV.\n";
    }

    std::cout << "\n========================================================================\n";
    std::cout << "          ANALISE COMPARATIVA EM LOTE: HEURISTICAS LPT vs SPT           \n";
    std::cout << "========================================================================\n";
    
    std::cout << std::left << std::setw(15) << "Instancia" 
              << std::right << std::setw(12) << "Jobs" 
              << std::right << std::setw(12) << "Maquinas" 
              << std::right << std::setw(15) << "Makespan LPT" 
              << std::right << std::setw(15) << "Makespan SPT" 
              << std::right << std::setw(12) << "MWKR"
              << std::right << std::setw(12) << "Melhor" 
              << "\n";
    std::cout << "------------------------------------------------------------------------\n";

    DIR *dir;
    struct dirent *ent;
    
    // Tenta abrir a pasta
    if ((dir = opendir(basePath.c_str())) != NULL) {
        // Lê todos os arquivos dentro do diretório, um por vez
        while ((ent = readdir(dir)) != NULL) {
            std::string nomeArquivo = ent->d_name;
            
            // Ignora pastas do sistema ("." e "..") e lê apenas arquivos com ".psi" no nome
            if (nomeArquivo != "." && nomeArquivo != ".." && nomeArquivo.find(".psi") != std::string::npos) {
                std::string fullPath = basePath + nomeArquivo;

                try {
                    JobShopModeler modeler;
                    if (modeler.parseFile(fullPath)) {
                        int jobs = modeler.getTotalJobs();
                        int machines = modeler.getTotalMachines();
                        Graph grafoInicial = modeler.buildInitialGraph();

                        // LPT
                        Graph grafoLPT = Heuristics::applyLPT(grafoInicial, jobs, machines);
                        std::vector<int> ordemLPT = Algorithms::topologicalSort(grafoLPT);
                        int makespanLPT = Algorithms::calculateMakespan(grafoLPT, ordemLPT);

                        // SPT
                        Graph grafoInicialLimpo = modeler.buildInitialGraph();
                        Graph grafoSPT = Heuristics::applySPT(grafoInicialLimpo, jobs, machines);
                        std::vector<int> ordemSPT = Algorithms::topologicalSort(grafoSPT);
                        int makespanSPT = Algorithms::calculateMakespan(grafoSPT, ordemSPT);

                        // MWKR
                        Graph grafoInicialLimpo2 = modeler.buildInitialGraph();
                        Graph grafoMWKR = Heuristics::applyMWKR(grafoInicialLimpo2, jobs, machines);
                        std::vector<int> ordemMWKR = Algorithms::topologicalSort(grafoMWKR);
                        int makespanMWKR = Algorithms::calculateMakespan(grafoMWKR, ordemMWKR);

                        // Descobrindo quem é o menor dos três
                        int menorMakespan = std::min({makespanLPT, makespanSPT, makespanMWKR});
                        std::string vencedor = "";

                        if (makespanLPT == makespanSPT && makespanSPT == makespanMWKR) {
                            vencedor = "Empate Triplo";
                        } else {
                            if (makespanLPT == menorMakespan) vencedor += "LPT ";
                            if (makespanSPT == menorMakespan) vencedor += "SPT ";
                            if (makespanMWKR == menorMakespan) vencedor += "MWKR ";
                        }

                        // 1. Imprime no Terminal
                        std::cout << std::left << std::setw(15) << nomeArquivo 
                                  << std::right << std::setw(12) << jobs 
                                  << std::right << std::setw(12) << machines 
                                  << std::right << std::setw(15) << makespanLPT 
                                  << std::right << std::setw(15) << makespanSPT 
                                  << std::right << std::setw(12) << makespanMWKR
                                  << std::right << std::setw(12) << vencedor 
                                  << "\n";

                        // 2. Salva no arquivo CSV
                        if (csvFile.is_open()) {
                            csvFile << nomeArquivo << ";" 
                                    << jobs << ";" 
                                    << machines << ";" 
                                    << makespanLPT << ";" 
                                    << makespanSPT << ";" 
                                    << makespanMWKR << ";"
                                    << vencedor << "\n";
                        }
                    }
                } catch (const std::exception& e) {
                    std::cout << std::left << std::setw(15) << nomeArquivo 
                              << " -> ERRO INTERNO: " << e.what() << "\n";
                } catch (...) {
                    std::cout << std::left << std::setw(15) << nomeArquivo 
                              << " -> ERRO CRITICO DESCONHECIDO.\n";
                }
            }
        }
        closedir(dir); 
    } else {
        std::cerr << "[ERRO] Nao foi possivel abrir a pasta: " << basePath << "\n";
    }

    std::cout << "========================================================================\n";
    std::cout << "[INFO] Resultados exportados com sucesso para: " << csvPath << "\n\n";

    if (csvFile.is_open()) csvFile.close();

    return 0;
}