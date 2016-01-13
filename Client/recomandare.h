#ifndef RECOMANDARE_H
#define RECOMANDARE_H
#include "global.h"

class Recomandare:public Comunicare
{
public:
    Recomandare();
    bool trimite_username_la_server(char * username);
};

#endif // RECOMANDARE_H
