//
// Created by charles on 10/11/16.
//

#include "Clienttest.hpp"

int main(int argc, char **argv)
{
    Client* C = new Client(8008, (char*)"0.0.0.0");
    C->fetch();

    printf("%s\n", C->get_buf());

    return 0;

}