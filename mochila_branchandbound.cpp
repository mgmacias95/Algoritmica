#include <vector>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <limits>
#include <queue>
#include <ctime>

using namespace std;

struct Elemento {
    unsigned peso;
    unsigned beneficio;
    Elemento (unsigned b, unsigned p) {
        beneficio = b;
        peso = p;
    }
    bool operator < (const Elemento & elem1) {
        return (((*this).beneficio/(*this).peso) > (elem1.beneficio/elem1.peso));
    }
    bool operator == (const Elemento & elem1) const {
        return (*this).peso == elem1.peso && (*this).beneficio == elem1.beneficio;
    }

    friend ostream & operator<< (ostream & os, Elemento & e) {
        os << "Beneficio = " << e.beneficio << "\tPeso = " << e.peso;
        return os;
    }
};

ostream & operator<< (ostream & os, list<Elemento> & l) {
    list<Elemento>::iterator i;
    for (i = l.begin(); i != l.end(); ++i)
        os << (*i);
    return os;
}

ostream & operator<< (ostream & os, vector<bool> & v){
    for (unsigned i=0; i<v.size(); i++) {
        if (v.at(i)) os << 1 << " ";
        else os << 0 << " ";
    }
    os << endl;
    return os;
}

struct Nodo {
    double CI;
    double CS;
    int nivel;
    double valor_actual;
    double peso_actual;
    vector<bool> tupla;
    Nodo (double cota_i, double cota_s, int n,
        double vact, double pact, vector<bool> & t) {
        CI = cota_i;
        CS = cota_s;
        nivel = n;
        valor_actual = vact;
        peso_actual = pact;
        tupla = t;
    }
    bool operator > (const Nodo & n1)const {
        return (*this).CI > n1.CI;
    }
    bool operator < (const Nodo & n1)const {
        return (*this).CI < n1.CI;
    }
};

double GreedyFraccional (const double lim_peso, list<Elemento> & objetos) {
    double pesoact = 0, b = 0;
    list<Elemento>::iterator i;

    for (i=objetos.begin(); i!=objetos.end() && pesoact < lim_peso; i++) {
        if ((pesoact + (*i).peso) <= lim_peso) {
            b += (*i).beneficio;
            pesoact += (*i).peso;
        }

        else {
            double aux = (lim_peso - pesoact);
            aux /= (*i).peso;
            b += aux * (*i).beneficio;
            pesoact = lim_peso;
        }
    }

    return b;
}

double Greedy01 (const double lim_peso, list<Elemento> & objetos) {
    double pesoact = 0, beneficio = 0;
    list<Elemento>::iterator i;

    for (i=objetos.begin(); i!=objetos.end() && pesoact < lim_peso; i++) {
        if ((pesoact + (*i).peso) <= lim_peso) {
            beneficio += (*i).beneficio;
            pesoact += (*i).peso;
        }
    }

    return beneficio;
}

Nodo NodoInicial (list<Elemento> & objs, double m) {
    double cota_s = GreedyFraccional(m, objs);
    double cota_i = Greedy01(m, objs);
    vector<bool> t(objs.size(), false);
    Nodo n (cota_i, cota_s, -1, 0, 0, t);

    return n;
}

Nodo Generar (Nodo & nodo_actual, bool eleccion, list<Elemento> & objs, double m) {
    Nodo res = Nodo(0, 0, nodo_actual.nivel+1, 0, 0, nodo_actual.tupla);

    // cogemos el objeto que estamos considerando
    list<Elemento>::iterator obj_it = objs.begin();
    for (int k=0; k<res.nivel; k++) ++obj_it;
    
    // generamos una lista con los objetos restantes que procesara el greedy
    list<Elemento> aux; list<Elemento>::iterator ax = obj_it;
    ++ax;
    for (list<Elemento>::iterator a = ax; a != objs.end(); ++a) {
        aux.push_back(*a);
    }
    
    res.tupla.at(res.nivel) = eleccion;

    if (!eleccion) {
        res.valor_actual = nodo_actual.valor_actual;
        res.peso_actual = nodo_actual.peso_actual;
    }

    else /*if (eleccion)*/ {
        res.valor_actual = nodo_actual.valor_actual + (*obj_it).beneficio;
        res.peso_actual = nodo_actual.peso_actual + (*obj_it).peso;
    }

    res.CI = res.valor_actual + Greedy01((m - res.peso_actual), aux);
    res.CS = res.valor_actual + GreedyFraccional((m - res.peso_actual), aux);


    if (res.peso_actual > m) {
        res.CI = numeric_limits<int>::min();
        res.CS = numeric_limits<int>::min();
        res.valor_actual = numeric_limits<int>::min();
    }

    return res;
}

vector<bool> Mochila(list<Elemento> & elementos, unsigned m) {
    Nodo inic = NodoInicial(elementos, m);
    int C = inic.CI;
    priority_queue<Nodo> LNV;
    LNV.push(inic);
    int s = numeric_limits<int>::min();
    vector<bool> resultado;

    while (!LNV.empty()) {
        Nodo x = LNV.top();
        LNV.pop();
        if (x.CS >= C) {
            for (unsigned k = 0; k < 2; k++) {
                bool elec = (k==0) ? false : true;
                Nodo y = Generar(x, elec, elementos, m);
                if (y.nivel == elementos.size()-1 && y.valor_actual > s) {
                    s = y.valor_actual;
                    C = (C >= s) ? C : s;
                    resultado = y.tupla;
                }

                else if (y.nivel < elementos.size()-1 && y.CS >= C) {
                    C = (C >= y.CI) ? C : y.CI;
                    LNV.push(y);
                }
            }
        }
    }

    return resultado;
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

    list<Elemento> candidatos;
    for (int i=1; i<argc-1; i+=2) {
        int b = atoi(argv[i]); int p = atoi(argv[i+1]);
        Elemento obj (b, p);
        candidatos.push_back(obj);
    }

    unsigned pesomochi = atoi(argv[argc-1]);
    cout << "Peso de la mochila: " << pesomochi << endl;

    clock_t antes, despues;
    antes = clock();
    candidatos.sort();
    // cout << "Objetos disponibles: " << endl << candidatos;

    // cout << "----------------------------------------" << endl;

    vector<bool> mochi = Mochila(candidatos, pesomochi);
    despues = clock();

    list<Elemento>::iterator it = candidatos.begin();

    cout << "Los objetos utilizados son: " << endl;
    for (unsigned i=0; i<mochi.size(); i++) {
        if (mochi.at(i))
            cout << "Usamos el objeto" << (*it) << endl;
        // else
        //     cout << "NO usamos el objeto " << i+1 << endl;
        ++it;
    }
    cout << "Tiempo: " << (despues-antes)/(double)CLOCKS_PER_SEC << endl;

    return 0;
}
