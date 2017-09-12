﻿#include <chrono>
#include <ctime>
#include <iostream>

#include <utils/psqlloader.h>
#include <samara.h>
#include "rcpp_samara.h"

using namespace std;

int main(int /*argc*/, char */*argv[]*/) {

    int nb_runs = 100;

    SamaraParameters * parameters = new SamaraParameters();
    PSQLLoader loader(parameters);
    loader.load_complete_simulation("06SB15-fev13-D1_SV21");
    std::chrono::time_point<std::chrono::system_clock> startC, endC;
    startC = std::chrono::system_clock::now();
    init_parameters(parameters);
    for (int i = 0; i < nb_runs; ++i) {
       run_samara_2_1(parameters);
    }

    endC = std::chrono::system_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::microseconds>
            (endC-startC).count() / nb_runs;
    std::time_t end_time = std::chrono::system_clock::to_time_t(endC);
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds << "microsec\n";
    std::cin.get();

//    runDB("06SB15-fev13-D1_SV21");
    std::cout << "finished" << endl;

	exit(0);
}