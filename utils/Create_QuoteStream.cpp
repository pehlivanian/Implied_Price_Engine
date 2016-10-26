//
// Created by charles on 10/13/16.
//

#include "QuoteSimulator.hpp"

int main(int argc, char **argv)
{
    QuoteSimulator<6>* QS = new QuoteSimulator<6>(1000);
    QS->process();

    return 0;
}
