#include <ilcplex/ilocplex.h>
#include <iostream>
#include <filesystem>

#include "src/Instance.h"

ILOSTLBEGIN

static void invalidUsage(const int argc, char** argv) {
    cerr << "Usage: " << argv[0] << " CPLEX <path> | " << endl;
    //cerr << "       " << argv[0] << " HEURISTIC ([filename] | 'all') | " << endl;
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
            cerr << "No such file or directory: " << filePath << endl;
            exit(1);
        }

        vector<string> files;

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
            cout << "Running CPLEX on file " << file << endl;
            Instance inst(file);
            cout<<inst.getNumBlocks()<<" "<<inst.getNumPeriods()<<endl;
        }


        return 0;
    }

    cerr << "NOT YET IMPLEMENTED" << endl;
    return 1;
}