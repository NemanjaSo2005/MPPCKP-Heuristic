#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main() {
    IloEnv env;

    try {
        IloModel model(env);

        const int N = 2000;

        // 2000 binary variables
        IloBoolVarArray x(env, N);

        // Objective: maximize the number of selected variables
        IloExpr objective(env);

        for (int i = 0; i < N; i++) {
            objective += x[i];
        }

        model.add(IloMaximize(env, objective));
        objective.end();

        // 2000 constraints
        for (int i = 0; i < N; i++) {
            model.add(x[i] + x[(i + 1) % N] <= 1);
        }

        // Solve
        IloCplex cplex(model);

        cplex.setOut(env.out());

        std::cout << "Variables: " << N << std::endl;
        std::cout << "Constraints: " << N << std::endl;

        if (cplex.solve()) {
            std::cout << "\nSolution found!" << std::endl;
            std::cout << "Status: " << cplex.getStatus() << std::endl;
            std::cout << "Objective value: "
                      << cplex.getObjValue() << std::endl;
            std::cout << "Number of variables: "
                      << N << std::endl;
            std::cout << "Number of constraints: "
                      << N << std::endl;
        } else {
            std::cout << "No solution found." << std::endl;
        }

    } catch (IloException& e) {
        std::cerr << "CPLEX exception: " << e << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception." << std::endl;
    }

    env.end();

    return 0;
}