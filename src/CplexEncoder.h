#ifndef MPPCKP_HEURISTIC_CPLEXENCODER_H
#define MPPCKP_HEURISTIC_CPLEXENCODER_H

#include "Instance.h"

class CplexEncoder {
    public:

    static void runCplex(const Instance &inst, std::string outputFile, double maxTime);
};


#endif //MPPCKP_HEURISTIC_CPLEXENCODER_H
