#pragma once
#include "NodoBPlusBase.h" 
#include <iostream>

class ArbolBPlus {
private:
    NodoBPlusBase* raiz;
    int orden; 


    void split_hoja(NodoBPlusBase* nodo); 
    void split_interno(NodoBPlusBase* nodo); 

public:
    ArbolBPlus(int _orden) {
        this->orden = _orden;
        this->raiz = nullptr;
    }

    ~ArbolBPlus() {
   
    }

    
    NodoGrafo* buscar(int id) {
        if (raiz == nullptr) return nullptr;

        NodoBPlusBase* actual = raiz;
        int accesos_disco = 0; 

       
        while (!actual->getHoja()) {
            accesos_disco++; 
            NodoBInterno* interno = (NodoBInterno*)actual;
     
            int indice = interno->buscar_siguiente(id);
            actual = interno->getPunteros()[indice];
        }

        // Llegamos a la hoja
        accesos_disco++; 
        NodoBHoja* hoja = (NodoBHoja*)actual;
        
        cout << "Accesos a nodos B+ (simulacion disco): " << accesos_disco << endl;
        return hoja->buscar_dato(id);
    }

    
    void insertar(int id, NodoGrafo* nodo) {
        if (raiz == nullptr) {
            raiz = new NodoBHoja(orden);
            ((NodoBHoja*)raiz)->insertar_dato(id, nodo);
            return;
        }

        NodoBPlusBase* actual = raiz;
        
    }
};