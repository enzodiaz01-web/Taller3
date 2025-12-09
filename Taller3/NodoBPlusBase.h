#include <iostream>
using namespace std;
class NodoGrafo;

class NodoBPlusBase {
private:
int* claves; // vector de ids de NodoGrafo
int orden; // factor m de árbol
bool es_hoja; // indica si es nodo interno o hoja del árbol
int cantidad_clave;
public:
NodoBPlusBase(int orden, bool es_hoja){
    orden = orden;
    es_hoja = es_hoja;
    cantidad_clave =0;
    claves = new int[orden];
    }
    virtual ~NodoBPlusBase(){
        if(claves) delete[] claves;
    }
    bool getHoja(){ return es_hoja;}
    int getOrden(){return orden;}
    int getCantidad_clave(){return cantidad_clave;}
    int* getClaves(){return claves;}
    void setCantidad_claves(int cant){ cantidad_clave = cant;}
    void agregar(int clave){
        if(cantidad_clave<orden){
            claves[cantidad_clave++] = clave;
        }
    }
};
class NodoBInterno: public NodoBPlusBase {
private:
NodoBPlusBase** punteros; // Vector del tipo NodoBPlusBase*
// función que dado un id de NodoGrafo indica a cual Nodo del árbol B hay
// que moverse. Devuelve el índice de vector punteros.
public:
    NodoBInterno(int orden) : NodoBPlusBase(orden, false){
        punteros = new NodoBPlusBase*[orden +1];
    }
    ~NodoBInterno(){
        if(punteros) delete[] punteros;
    }
    int buscar_siguiente(int buscando){
        int* mis_claves = getClaves();
        int cant = getCantidad_clave();
        for(int i = 0; i<cant;i++){
            if(buscando<mis_claves[i]){
                return i;
            }
        }return cant;
    }
    NodoBPlusBase** getPunteros(){
        return punteros;
    }
    void setPunteros(int indice, NodoBPlusBase* nodo){
        punteros[indice]=nodo;
    }
};
class NodoBHoja: public NodoBPlusBase {
private:
NodoGrafo** datos; // vector del tipo NodoGrafo*
NodoBHoja* siguiente_hoja; // puntero del tipo NodoBHoja*
};