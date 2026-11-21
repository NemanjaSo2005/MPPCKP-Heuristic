#include <iostream>
#include <filesystem>

#include "src/CplexEncoder.h"
#include "src/Instance.h"

#define CPLEX_VARIABLE_LIMIT 1'000'000
#define MAX_CPLEX_TIME 900

static void invalidUsage(const int argc, char** argv) {
    std::cerr << "Usage: " << argv[0] << " CPLEX <path> | " << std::endl;
    //std::cerr << "       " << argv[0] << " HEURISTIC ([filename] | 'all') | " << std::endl;
    exit(1);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        invalidUsage(argc, argv);
    }
    if (std::string(argv[1]) == "CPLEX") {
        if (argc != 3) {
            invalidUsage(argc, argv);
        }
        auto filePath = std::string(argv[2]);

        if (!std::filesystem::exists(filePath)) {
            std::cerr << "No such file or directory: " << filePath << std::endl;
            exit(1);
        }

        std::vector<std::string> files;

        if (std::filesystem::is_directory(filePath)) {
            try {
                for (const auto& entry : std::filesystem::directory_iterator(filePath)) {
                    if (entry.is_regular_file()) {
                        files.push_back(entry.path().string());
                    }
                }
            } catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Filesystem error: " << e.what() << '\n';
                exit(1);
            }
        }
        else {
            files.push_back(filePath);
        }

        for (const auto &file : files) {
            std::cout << "Running CPLEX on file " << file << std::endl;
            Instance inst(file);
            if (inst.getNumBlocks() * static_cast<long long>(inst.getNumPeriods()) > CPLEX_VARIABLE_LIMIT) {
                std::cout << "Instance too big to be ran on CPLEX" << std::endl;
                continue;
            }
            CplexEncoder::runCplex(inst, "./results/cplex/" + inst.getName(), MAX_CPLEX_TIME);
        }


        return 0;
    }

    std::cerr << "NOT YET IMPLEMENTED" << std::endl;
    return 1;
}