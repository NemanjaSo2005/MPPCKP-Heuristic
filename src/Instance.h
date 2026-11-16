#ifndef MPPCKP_HEURISTIC_INSTANCE_H
#define MPPCKP_HEURISTIC_INSTANCE_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Instance {
    public:
    explicit Instance(const std::string& fileName);

    int getNumBlocks() const;
    int getNumPeriods() const;
    double getDiscountRate() const;
    int getPeriodCapacity(int period) const;
    std::vector<int> getPeriods();
    int getBlockWeight(int block) const;
    std::vector<int> getBlockPrecedences(int block);
    double getBlockProfitInPeriod(int block, int period) const;

    private:
    struct block {
        std::string id;
        int weight{};
        int baseProfit{};
        int precedenceCount{};
        std::vector<int> precedences;
    };

    int numBlocks{};
    int numPeriods{};
    double discountRate{};
    std::vector<int> periodCapacity;
    std::vector<block> blocks;
};


#endif //MPPCKP_HEURISTIC_INSTANCE_H
