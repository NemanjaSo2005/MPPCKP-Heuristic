#include "Instance.h"

#include <cmath>

Instance::Instance(const std::string &fileName) {
    std::ifstream ifs(fileName.c_str());
    if (!ifs.is_open()) {
        std::cerr << "Error opening file " << fileName << std::endl;
        return;
    }

    ifs >> numBlocks >> numPeriods >> discountRate;
    periodCapacity.resize(numPeriods);
    for (int i = 0; i < numPeriods; ++i) {
        ifs >> periodCapacity[i];
    }

    blocks.resize(numBlocks);
    for (int i = 0; i < numBlocks; ++i) {
        ifs >> blocks[i].id >> blocks[i].weight >> blocks[i].baseProfit >> blocks[i].precedenceCount;
        if (blocks[i].id != std::to_string(i)) {
            std::cerr << "Error, block on position " << i << " has id " << blocks[i].id << " instead of its position in the list." << std::endl;
            return;
        }
        blocks[i].precedences.resize(blocks[i].precedenceCount);
        for (int j = 0; j < blocks[i].precedenceCount; ++j) {
            ifs >> blocks[i].precedences[j];
        }
    }

    ifs.close();
}

int Instance::getNumBlocks() const {
    return numBlocks;
}

int Instance::getNumPeriods() const {
    return numPeriods;
}

double Instance::getDiscountRate() const {
    return discountRate;
}

int Instance::getPeriodCapacity(int period) const {
    if (period >= periodCapacity.size()) {
        std::cerr << "Error, period " << period << " does not exist!" << std::endl;
        return -1;
    }
    return periodCapacity[period];
}

std::vector<int> Instance::getPeriods() {
    return periodCapacity;
}

int Instance::getBlockWeight(int block) const {
    if (block >= blocks.size()) {
        std::cerr << "Error, block " << block << " does not exist!" << std::endl;
        return -1;
    }
    return blocks[block].weight;
}

std::vector<int> Instance::getBlockPrecedences(int block) {
    if (block >= blocks.size()) {
        std::cerr << "Error, block " << block << " does not exist!" << std::endl;
        return std::vector<int>();
    }
    return blocks[block].precedences;
}

double Instance::getBlockProfitInPeriod(int block, int period) const {
    if (block >= blocks.size()) {
        std::cerr << "Error, block " << block << " does not exist!" << std::endl;
        return -1;
    }
    return blocks[block].baseProfit * std::pow(discountRate, period-1);
}
