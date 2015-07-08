#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Objeto {
    int beneficio;
    int peso;
    float benporpeso;
    bool operator < (Objeto & obj1) {
        return (benporpeso > obj1.benporpeso);
    }
    friend ostream & operator<< (ostream & os, const Objeto & o) {
        os << "Peso: " << o.peso << '\t' << "Beneficio: " << o.beneficio << 
            '(' << o.benporpeso << ')';
        return os;
    }
    Objeto (int b, int p) {
        beneficio = b;
        peso = p;
        benporpeso = (float)beneficio/peso;
    }
};

list<float> Mochila (int lim_peso, list<Objeto> & objetos) {
    list<float> sol;
    int peso_actual = 0;
    list<Objeto>::iterator rit;

    for (rit=objetos.begin(); rit!=objetos.end() && lim_peso > peso_actual; ++rit) {
        if ((peso_actual + (*rit).peso) <= lim_peso) {
            sol.push_back(1);
            peso_actual += (*rit).peso;
        }

        else {
            float ben = (float)(lim_peso-peso_actual)/((*rit).peso);
            sol.push_back(ben);
            peso_actual = lim_peso;
        }
    }

    while (rit != objetos.end()){
        sol.push_back(0);
        ++rit;
    }

    return sol;
}

float CalculaBeneficioTotal (list<float> & m, list<Objeto> & objetos) {
    float ben = 0;
    list<float>::iterator it_f;
    list<Objeto>::iterator rit_o;
    for (it_f = m.begin(), rit_o = objetos.begin(); it_f != m.end() && 
        rit_o != objetos.end(); ++it_f, ++rit_o)
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

    clock_t antes, despues;

    antes = clock();
    candidatos.sort();

    // cout << "Los objetos de la mochila ordenados de mayor a menor son:" << endl;
    // list<Objeto>::iterator rit;
    // for (rit=candidatos.begin(); rit!=candidatos.end(); ++rit)
    //     cout << (*rit).beneficio << " " << (*rit).peso << "\t";
    // cout << endl;

    int pesomochi = atoi(argv[argc-1]);

    list<float> m = Mochila(pesomochi, candidatos);
    despues = clock();

    cout << "La proporcion de cada uno que cogemos es:" << endl;
    list<float>::iterator it; list<Objeto>::iterator rit = candidatos.begin();
    for (it=m.begin(); it!=m.end(); ++it) {
        cout << *rit << " -> " <<  *it << endl;
        ++rit;
    }


    cout << "El beneficio total es, por tanto " <<
        CalculaBeneficioTotal(m, candidatos) << endl;
    
    cout << "Tiempo: " << (despues-antes)/(double)CLOCKS_PER_SEC << endl;

    return 0;
}
