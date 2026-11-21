#ifndef MPPCKP_HEURISTIC_INSTANCE_H
#define MPPCKP_HEURISTIC_INSTANCE_H

#include <string>
#include <vector>

class Instance {
    public:
    explicit Instance(const std::string& fileName);

    std::string getName() const;
    int getNumBlocks() const;
    int getNumPeriods() const;
    double getDiscountRate() const;
    int getPeriodCapacity(int period) const;
    std::vector<int> getPeriods() const;
    int getBlockWeight(int block) const;
    std::vector<int> getBlockPrecedences(int block) const;
    double getBlockProfitInPeriod(int block, int period) const;

    private:
    struct block {
        std::string id;
        int weight{};
        int baseProfit{};
        int precedenceCount{};
        std::vector<int> precedences;
    };

    std::string name;
    int numBlocks{};
    int numPeriods{};
    double discountRate{};
    std::vector<int> periodCapacity;
    std::vector<block> blocks;
};


#endif //MPPCKP_HEURISTIC_INSTANCE_H
