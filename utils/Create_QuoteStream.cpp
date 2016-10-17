//
// Created by charles on 10/13/16.
//

#include "QuoteSimulator.hpp"

int main(int argc, char **argv)
{
    QuoteSimulator<12>* QS = new QuoteSimulator<12>(10000);
    QS->process();

    return 0;
}