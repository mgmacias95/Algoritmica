#include <iostream>
#include <list>
#include <cstdlib>

using namespace std;

struct Objeto {
    int beneficio;
    int peso;
    bool operator < (Objeto & obj1) {
        return ((beneficio/peso) < (obj1.beneficio/obj1.peso));
    }
    bool operator > (Objeto & obj1) {
        return ((beneficio/peso) > (obj1.beneficio/obj1.peso));
    }
    Objeto (int b, int p) {
        beneficio = b;
        peso = p;
    }
};

list<float> Mochila (int lim_peso, list<Objeto> & objetos) {
    list<float> sol;
    int peso_actual = 0;
    list<Objeto>::reverse_iterator rit;

    for (rit=objetos.rbegin(); rit!=objetos.rend() && lim_peso > peso_actual; ++rit) {
        if (peso_actual <= (*rit).peso) {
            sol.push_back(1);
            peso_actual += (*rit).peso;
        }

        else {
            sol.push_back((lim_peso-peso_actual)/((*rit).peso));
            peso_actual = lim_peso;
        }
    }

    while (rit != objetos.rend()){
        sol.push_back(0);
        ++rit;
    }

    return sol;
}

int CalculaBeneficioTotal (list<float> & m, list<Objeto> & objetos) {
    int ben = 0;
    list<float>::iterator it_f;
    list<Objeto>::reverse_iterator rit_o;
    for (it_f = m.begin(), rit_o = objetos.rbegin(); it_f != m.end() && 
        rit_o != objetos.rend(); ++it_f, ++rit_o)
        ben += (*it_f) * (*rit_o).beneficio; 

    return ben;
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cerr << "ERROR: falta la lista de beneficios-pesos" <<
            " y el peso de la mochila" << endl;
        exit(1);
    }

    if (argc%2 != 0) {
        cerr << "ERROR: argumentos incorrectos: beneficio-peso pesomochi" << endl;
        exit(2);
    }

    list<Objeto> candidatos;
    for (int i=1; i<argc-1; i+=2) {
        int b = atoi(argv[i]); int p = atoi(argv[i+1]);
        Objeto obj (b, p);
        candidatos.push_back(obj);
    }

    candidatos.sort();

    cout << "Los objetos de la mochila ordenados de mayor a menor son:" << endl;
    list<Objeto>::reverse_iterator rit;
    for (rit=candidatos.rbegin(); rit!=candidatos.rend(); ++rit)
        cout << (*rit).beneficio << " " << (*rit).peso << "\t";
    cout << endl;

    int pesomochi = atoi(argv[argc-1]);

    list<float> m = Mochila(pesomochi, candidatos);

    cout << "La proporcion de cada uno que cogemos es:" << endl;
    list<float>::iterator it;
    for (it=m.begin(); it!=m.end(); ++it)
        cout << *it << "\t";
    cout << endl;

    cout << "El beneficio total es, por tanto " <<
        CalculaBeneficioTotal(m, candidatos) << endl;

    return 0;
}