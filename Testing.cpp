//
// Created by lucie on 7/6/2022.
//

#include "Testing.h"
#include <string>
#include <vector>
#include "Algebra/MissingBlock.hpp"
#include "Performance/Benchmark.h"
#include "Algebra/Auxiliary.h"


namespace Testing {

arma::vec extractBlock(uint64_t column, uint64_t startingIndex, uint64_t blockSize,
                       arma::mat &matrix)
{
    arma::vec extraction(blockSize);

    for (uint64_t i = 0; i < blockSize; ++i)
    {
        extraction[i] = matrix(startingIndex + i, column);
    }

    return extraction;
}

void imputeBlock(uint64_t column, uint64_t startingIndex, uint64_t blockSize,
                 arma::mat &matrix, const arma::vec &data)
{
    for (uint64_t i = 0; i < blockSize; ++i)
    {
        matrix(startingIndex + i, column) = data.at(i);
    }
}

void TestMissPerc()
{
    arma::mat TestMat;
    TestMat.zeros(10,4);
    Algorithms::MissingBlock missing(0, 2, 5, TestMat);
    arma::vec TestVec(5);
    TestVec.fill(arma::datum::nan);
    missing.imputeBlock(TestVec);

    TestMat.print();
}

std::vector<std::vector<double>> synth_missing = {
        {-12.00, 8.00,   -4.00,  -8.00},
        {0.00,   0.00,   0.00,   0.00},
        {-48.00, 32.00,  -16.00, -32.00},
        {NAN,    64.00,  -32.00, -64.00},
        {NAN,    24.00,  -12.00, -24.00},
        {NAN,    64.00,  -32.00, -64.00},
        {NAN,    16.00,  -8.00,  -16.00},
        {NAN,    8.00,   -4.00,  -8.00},
        {NAN,    -32.00, 16.00,  32.00},
        {NAN,    8.00,   -4.00,  -8.00},
        {12.00,  -8.00,  4.00,   8.00},
        {NAN,    -24.00, 12.00,  24.00},
        {NAN,    16.00,  -8.00,  -16.00},
        {-12.00, 8.00,   -4.00,  -8.00},
        {24.00,  -16.00, 8.00,   16.00},
        {NAN,    -8.00,  4.00,   8.00},
        {NAN,    -12.00, 6.00,   12.00},
        {NAN,    -24.00, 12.00,  24.00},
        {48.00,  -32.00, 16.00,  32.00},
        {12.00,  -8.00,  4.00,   8.00}
};

void TestRecovery(){
    const arma::mat mx = Algebra::Operations::std_to_arma(synth_missing);
    arma::mat copy;

    std::string algos[]{"cd", "tkcm", "spirit", "grouse", "nnmf", "dynammo",
                           "svt", "rosl", "itersvd", "softimpute", "st-mvl"};
    for(auto algo : algos) {
        copy = mx;
        std::vector<double> params = {0,0,0};
        std::cout << algo << std::endl;
        u_int64_t result = Performance::Recovery(copy, algo, params);
        copy.print("Recovered with " + algo);
        copy.reset();
    }
}
} // Testing