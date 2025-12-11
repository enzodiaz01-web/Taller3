#include <iostream>
#include "NodoGrafo.h"
using namespace std;
class NodoGrafo;

class NodoBPlusBase {
protected: 
    int* claves; 
    int orden; 
    bool es_hoja; 
    int cantidad_claves;
    NodoBPlusBase* padre;

public:
    NodoBPlusBase(int orden, bool es_hoja){
        this->orden = orden;
        this->es_hoja = es_hoja;
        cantidad_claves = 0;
        claves = new int[orden];
        padre = nullptr; 
    }
    virtual ~NodoBPlusBase(){
        if(claves) delete[] claves;
    }
    NodoBPlusBase* getPadre() { return padre; }
    void setPadre(NodoBPlusBase* p) { padre = p; }
    bool getHoja(){ return es_hoja;}
    int getOrden(){return orden;}
    int getCantidad_claves(){return cantidad_claves;}
    int* getClaves(){return claves;}
    void setCantidad_claves(int cant){ cantidad_claves = cant;}
    
    void agregar(int clave){
        if(cantidad_claves < orden){
            claves[cantidad_claves++] = clave;
        }
    }
};
class NodoBInterno: public NodoBPlusBase {
private:
    NodoBPlusBase** punteros; 
public:
    NodoBInterno(int orden) : NodoBPlusBase(orden, false){
        punteros = new NodoBPlusBase*[orden +1];
    }
    ~NodoBInterno(){
        if(punteros) delete[] punteros;
    }
    int buscar_siguiente(int buscando){
        for(int i = 0; i<cantidad_claves;i++){
            if(buscando<claves[i]){
                return i;
            }
        }return cantidad_claves;
    }
    NodoBPlusBase** getPunteros(){return punteros;}
    void setPunteros(int indice, NodoBPlusBase* nodo){punteros[indice]=nodo;}
};

class NodoBHoja: public NodoBPlusBase {
private:
    NodoGrafo** datos; 
    NodoBHoja* siguiente_hoja;
public:
    NodoBHoja(int orden) : NodoBPlusBase(orden, true) {
        // En las hojas guardamos punteros a los datos reales
        datos = new NodoGrafo*[orden]; 
        siguiente_hoja = nullptr;
    }

    ~NodoBHoja() {
        if(datos) delete[] datos;
    }
    NodoGrafo** getDatos() { return datos; }
    NodoBHoja* getSiguiente() { return siguiente_hoja; }
    void setSiguiente(NodoBHoja* sig) { siguiente_hoja = sig; }
    void insertar_dato(int clave, NodoGrafo* dato) {
        if (getCantidad_claves() < getOrden()) {
            int i = getCantidad_claves() - 1;
            while (i >= 0 && claves[i] > clave) {
                claves[i + 1] = claves[i];
                datos[i + 1] = datos[i];
                i--;
            }          
            claves[i + 1] = clave;
            datos[i + 1] = dato;
            cantidad_claves++;
        }
    }
    NodoGrafo* buscar_dato(int clave) {
        for (int i = 0; i < cantidad_claves; i++) {
            if (claves[i] == clave) {return datos[i];}
        }
        return nullptr;
    }
};