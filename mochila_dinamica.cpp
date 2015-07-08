#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Elemento {
    unsigned peso;
    unsigned beneficio;
    Elemento (int b, int p) {
        beneficio = b;
        peso = p;
    }
    friend ostream & operator << (ostream & os, Elemento & e) {
        os << "Peso: " << e.peso << "\tBeneficio: " << e.beneficio;
        return os;
    }
};

unsigned Max (unsigned n1, unsigned n2) {
    if (n1 >= n2) return n1;
    else return n2;
}

vector<vector<unsigned> > Mochila(vector<Elemento> & elems, unsigned m) {
    unsigned i = 0, j = 0;
    vector<vector<unsigned> > V(elems.size()+1);
    // inicializamos los casos base
    for (i=0; i<=elems.size(); i++) {
        V.at(i).resize(m+1);
        V.at(i).at(0) = 0;
    }

    for (j=0; j<=m; j++)
        V.at(0).at(j) = 0;

    // rellenamos la tabla
    for (i=1; i<=elems.size(); i++) {
        for (j=1; j<=m; j++) {
            // el objeto no cabe en la mochila
            int aux = j-elems[i-1].peso;
            if (aux < 0)
                V.at(i).at(j) = V.at(i-1).at(j);
            else {
                unsigned ben = elems.at(i-1).beneficio+(V.at(i-1).at(j-elems[i-1].peso));
                V.at(i).at(j) = Max(V.at(i-1).at(j), ben);
            }
        }
    }

    return V; 
}

vector<unsigned> Solucion (vector<vector<unsigned> > & mochila, vector<Elemento> & elems) {
    vector<unsigned> sol(elems.size());

    int j = mochila.at(0).size()-1;
    for (int i=mochila.size()-1; i>0; i--) {
        if (mochila.at(i).at(j) == mochila.at(i-1).at(j))
            sol.at(i-1) = 0;
        else { // if (mochila.at(i).at(j) == mochila.at(i-1).at(j.elems[i-1].peso))+elems[i-1].beneficio)
            sol.at(i-1) = 1;
            j -= elems[i-1].peso;
        }
    }

    return sol;
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

    vector<Elemento> candidatos;
    for (int i=1; i<argc-1; i+=2) {
        int b = atoi(argv[i]); int p = atoi(argv[i+1]);
        Elemento obj (b, p);
        candidatos.push_back(obj);
    }

    unsigned pesomochi = atoi(argv[argc-1]);

    clock_t antes, despues;
    antes = clock();
    vector<vector<unsigned> > mochi = Mochila(candidatos, pesomochi);


    vector<unsigned> sol = Solucion(mochi, candidatos);
    despues = clock();

    cout << "   ";
    for (unsigned i=0; i<=pesomochi; i++)
        cout << i << '\t';
    cout << endl << "------------------------------------------------------" <<
        "-------------------------------------------" << endl;

    for (unsigned i=0; i<mochi.size(); i++) {
        cout << i << '|' << ' ';
        for (unsigned j=0; j<mochi[i].size(); j++)
            cout << mochi.at(i).at(j) << '\t';
        cout << endl;
    }
    
    vector<Elemento>::iterator rit = candidatos.begin();
    cout << "Los objetos utilizados son: " << endl;
    for (unsigned i=0; i<sol.size(); i++) {
        if (sol[i]==1)
            cout << "Usamos el objeto " << *rit << endl;
        else
            cout << "NO usamos el objeto " << *rit << endl;
        ++rit;
    }

    cout << "Tiempo: " << (despues-antes)/(double)CLOCKS_PER_SEC << endl;

    return 0;
}
