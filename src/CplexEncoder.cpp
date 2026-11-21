#include "CplexEncoder.h"
#include <iostream>
#include <ilcplex/ilocplex.h>
#include <fstream>

ILOSTLBEGIN

void CplexEncoder::runCplex(const Instance &inst, string outputFile, double maxTime) {
    const int blocks = inst.getNumBlocks();
    const int periods = inst.getNumPeriods();

    IloEnv env;

    try {
        IloModel model(env);

        // VARIABLES

        // x[block][period]
        IloArray<IloBoolVarArray> x(env, blocks);
        for (int i = 0; i < blocks; i++) {
            x[i] = IloBoolVarArray(env, periods);
        }

        // profit[block][period]
        IloArray<IloFloatArray> profit(env, blocks);
        for (int i = 0; i < blocks; i++) {
            profit[i] = IloFloatArray(env, periods);
            for (int j = 0; j < periods; j++) {
                profit[i][j] = inst.getBlockProfitInPeriod(i, j);
            }
        }

        // weight[block]
        IloIntArray weight(env, blocks);
        for (int i = 0; i < blocks; i++) {
            weight[i] = inst.getBlockWeight(i);
        }

        // capacity[period]
        IloIntArray capacity(env, periods);
        for (int j = 0; j < periods; j++) {
            capacity[j] = inst.getPeriodCapacity(j);
        }

        // OBJECTIVE

        IloExpr objective(env);

        for (int i = 0; i < blocks; i++) {
            objective += x[i][0] * profit[i][0];
            for (int j = 1; j < periods; j++) {
                objective += (x[i][j] - x[i][j-1]) * profit[i][j];
            }
        }

        model.add(IloMaximize(env, objective));
        objective.end();

        // CONSTRAINTS

        for (int j = 0; j < periods; j++) {
            IloExpr totalWeightInPeriod(env);

            if (j == 0) {
                for (int i = 0; i < blocks; i++) {
                    totalWeightInPeriod += x[i][0] * weight[i];
                }
            }
            else {
                for (int i = 0; i < blocks; i++) {
                    totalWeightInPeriod += (x[i][j] - x[i][j-1]) * weight[i];
                }
            }
            model.add(totalWeightInPeriod <= capacity[j]);

            totalWeightInPeriod.end();
        }

        for (int i = 0; i < blocks; i++) {
            auto precedences = inst.getBlockPrecedences(i);
            for (int p : precedences) {
                for (int j = 0; j < periods; j++) {
                    model.add(x[i][j] <= x[p][j]);
                }
            }
        }

        for (int i = 0; i < blocks; i++) {
            for (int j = 1; j < periods; j++) {
                model.add(x[i][j-1] <= x[i][j]);
            }
        }

        // SOLVING

        IloCplex cplex(model);
        //cplex.setOut(env.getNullStream());
        //cplex.exportModel((outputFile + "_model.lp").c_str());
        cplex.setParam(IloCplex::Param::TimeLimit, maxTime);

        cout << "Starting solving the instance" << endl;

        auto solved = cplex.solve();

        cout << "Finished solving the instance " << endl;

        ofstream ouf(outputFile + ".log");

        if (!ouf.is_open()) {
            cerr << "Unable to open file for writing: " << outputFile << endl;
            model.end();
            env.end();
            return;
        }

        ouf << "Status: " << cplex.getStatus() << endl;
        ouf << "Execution time: " << cplex.getTime() << endl;

        if (!solved) {
            model.end();
            env.end();
            return;
        }

        ouf << "Objective function: " << cplex.getObjValue() << endl;

        ouf << "Solution: (i-th number is the period of the i-th block, or -1 if the block is not taken at all)" << endl;
        for (int i = 0; i < blocks; i++) {
            int period = -1;
            for (int j = 0; j < periods && period == -1; j++) {
                if (cplex.getValue(x[i][j]) == 1) {
                    period = j;
                }
            }
            ouf << period << " ";
        }
        ouf << endl;

        ouf.close();

        model.end();
    }
    catch (IloException& e) {
        cerr << "CPLEX error: " << e << endl;
    }

    env.end();
}
