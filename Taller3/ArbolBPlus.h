#pragma once
#include "NodoBPlusBase.h" 
#include "NodoGrafo.h"
#include <iostream>
using namespace std;

class ArbolBPlus {
private:
    NodoBPlusBase* raiz;
    int orden; 
    void insertar_en_nodo_interno_con_espacio(NodoBInterno* padre, int clave, NodoBPlusBase* hijo_derecho) {
        int k = padre->getCantidad_claves();
        int* claves = padre->getClaves();
        NodoBPlusBase** punteros = padre->getPunteros();

        while (k > 0 && claves[k - 1] > clave) {
            claves[k] = claves[k - 1];         
            punteros[k + 1] = punteros[k];      
            k--;
        }

        claves[k] = clave;
        punteros[k + 1] = hijo_derecho;
        padre->setCantidad_claves(padre->getCantidad_claves() + 1);
    }
    void insertar_en_padre(NodoBPlusBase* n, int clave, NodoBPlusBase* nuevo_nodo) {

        if (n == raiz) {
            NodoBInterno* nueva_raiz = new NodoBInterno(orden);
            nueva_raiz->agregar(clave); 
            nueva_raiz->setPunteros(0, n);
            nueva_raiz->setPunteros(1, nuevo_nodo);

            n->setPadre(nueva_raiz);
            nuevo_nodo->setPadre(nueva_raiz);
            raiz = nueva_raiz;
            return;
        }

        
        NodoBInterno* padre = (NodoBInterno*)n->getPadre();

        if (padre->getCantidad_claves() < orden) {
            
            insertar_en_nodo_interno_con_espacio(padre, clave, nuevo_nodo);
        } else {
            
            split_interno(padre, clave, nuevo_nodo);
        }
    }
    void split_hoja(NodoBHoja* hoja, int id_extra, NodoGrafo* dato_extra) {
        
        int* claves_temp = new int[orden + 1];
        NodoGrafo** datos_temp = new NodoGrafo*[orden + 1];

        int total = hoja->getCantidad_claves();
        int* claves_orig = hoja->getClaves();
        NodoGrafo** datos_orig = hoja->getDatos();

       
        for (int i = 0; i < total; i++) {
            claves_temp[i] = claves_orig[i];
            datos_temp[i] = datos_orig[i];
        }

        
        int i = total - 1;
        while (i >= 0 && claves_temp[i] > id_extra) {
            claves_temp[i + 1] = claves_temp[i];
            datos_temp[i + 1] = datos_temp[i];
            i--;
        }
        claves_temp[i + 1] = id_extra;
        datos_temp[i + 1] = dato_extra;

        
        NodoBHoja* nueva_hoja = new NodoBHoja(orden);
        
        int split_idx = (orden + 1) / 2;

        
        hoja->setCantidad_claves(0);
        for (int j = 0; j < split_idx; j++) {
            hoja->insertar_dato(claves_temp[j], datos_temp[j]);
        }

        
        for (int j = split_idx; j < orden + 1; j++) {
            nueva_hoja->insertar_dato(claves_temp[j], datos_temp[j]);
        }

        
        nueva_hoja->setSiguiente(hoja->getSiguiente());
        hoja->setSiguiente(nueva_hoja);
        
        nueva_hoja->setPadre(hoja->getPadre());

        
        int clave_promocion = nueva_hoja->getClaves()[0];

        delete[] claves_temp;
        delete[] datos_temp;

        
        insertar_en_padre(hoja, clave_promocion, nueva_hoja);
    } 
    void split_interno(NodoBInterno* nodo, int clave_extra, NodoBPlusBase* hijo_extra) {
        int* claves_temp = new int[orden + 1];
        NodoBPlusBase** punteros_temp = new NodoBPlusBase*[orden + 2];

        int total_claves = nodo->getCantidad_claves();
        int* claves_actuales = nodo->getClaves();
        NodoBPlusBase** punteros_actuales = nodo->getPunteros();

        for(int k=0; k<total_claves; k++) claves_temp[k] = claves_actuales[k];
        for(int k=0; k<total_claves+1; k++) punteros_temp[k] = punteros_actuales[k];
        
        int k = total_claves;
        while(k > 0 && claves_temp[k-1] > clave_extra) {
            claves_temp[k] = claves_temp[k-1];
            punteros_temp[k+1] = punteros_temp[k];
            k--;
        }
        claves_temp[k] = clave_extra;
        punteros_temp[k+1] = hijo_extra;
        
        NodoBInterno* nuevo_interno = new NodoBInterno(orden);
        int split_idx = (orden + 1) / 2; 
        int clave_que_sube = claves_temp[split_idx];

        nodo->setCantidad_claves(0);
        for(int z = 0; z < split_idx; z++) {
            nodo->agregar(claves_temp[z]);
            nodo->setPunteros(z+1, punteros_temp[z+1]); 
        }

        nuevo_interno->setPunteros(0, punteros_temp[split_idx+1]); 
        for(int z = split_idx + 1; z < orden + 1; z++) {
            nuevo_interno->agregar(claves_temp[z]);
            nuevo_interno->setPunteros(nuevo_interno->getCantidad_claves(), punteros_temp[z+1]);
        }

        for(int z = 0; z <= nuevo_interno->getCantidad_claves(); z++) {
            if(nuevo_interno->getPunteros()[z])
                nuevo_interno->getPunteros()[z]->setPadre(nuevo_interno);
        }

        nuevo_interno->setPadre(nodo->getPadre());

        delete[] claves_temp;
        delete[] punteros_temp;

        insertar_en_padre(nodo, clave_que_sube, nuevo_interno);
    } 

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
        
        while (!actual->getHoja()) {
            NodoBInterno* interno = (NodoBInterno*)actual;
            int indice = interno->buscar_siguiente(id);
            actual = interno->getPunteros()[indice];
        }

        NodoBHoja* hoja = (NodoBHoja*)actual;
        return hoja->buscar_dato(id);
    }

    
    void insertar(int id, NodoGrafo* nodo) {
        if (raiz == nullptr) {
            raiz = new NodoBHoja(orden);
            ((NodoBHoja*)raiz)->insertar_dato(id, nodo);
            return;
        }

        NodoBPlusBase* actual = raiz;
        while (!actual->getHoja()) {
            NodoBInterno* interno = (NodoBInterno*)actual;
            int idx = interno->buscar_siguiente(id);
            actual = interno->getPunteros()[idx];
        }
        NodoBHoja* hoja = (NodoBHoja*)actual;

        if (hoja->getCantidad_claves() < orden) {
            hoja->insertar_dato(id, nodo);
        } else {
            split_hoja(hoja, id, nodo);
        }
    }
    void eliminar(int id) {
        if (raiz == nullptr) return;

        // Búsqueda simplificada para eliminar (Solo borrado en hoja, no hace merge/rebalanceo complejo por ahora)
        NodoBPlusBase* actual = raiz;
        while (!actual->getHoja()) {
            NodoBInterno* interno = (NodoBInterno*)actual;
            int idx = interno->buscar_siguiente(id);
            if (idx > interno->getCantidad_claves()) idx = interno->getCantidad_claves();
            actual = interno->getPunteros()[idx];
        }

        NodoBHoja* hoja = (NodoBHoja*)actual;
        
        int pos = -1;
        int* claves = hoja->getClaves();
        for (int i = 0; i < hoja->getCantidad_claves(); i++) {
            if (claves[i] == id) {
                pos = i;
                break;
            }
        }

        if (pos != -1) {
            for (int i = pos; i < hoja->getCantidad_claves() - 1; i++) {
                hoja->getClaves()[i] = hoja->getClaves()[i+1];
                hoja->getDatos()[i] = hoja->getDatos()[i+1];
            }
            hoja->setCantidad_claves(hoja->getCantidad_claves() - 1);
        }
    }
};