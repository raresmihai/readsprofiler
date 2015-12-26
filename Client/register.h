#ifndef REGISTER_H
#define REGISTER_H


class Register
{
private:
    char *username, *parola, *confParola;
    char *nume, *prenume, *email;
public:
    Register(char *un,char* pw,char* cp,char *nm,char *pren,char *em);
    bool datele_sunt_valide();
    bool trimite_datele_la_server();
    bool primeste_raspuns_de_la_server();
    bool este_inregistrat();
};

#endif // REGISTER_H
