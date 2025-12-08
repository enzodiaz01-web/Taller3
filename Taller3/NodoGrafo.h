#include <iostream>
#include <string>

using namespace std;

class NodoGrafo {
private:
int id;
int* padres;      
int num_padres;
int max_padres;  
public:
    NodoGrafo(int id) {
        id = id;
        num_padres = 0;
        max_padres = 2;
        padres = new int[max_padres];
    }
    virtual ~NodoGrafo(){
        if(padres) delete[] padres;
    }
    int getID(){return id;}
    virtual bool es_directorio()=0;
    int* lista_padres(){return padres;}
    int get_cant_padres(){return;}

    void agregar_padre(int id_p) {
        if (num_padres == max_padres) {
            int* aux = new int[max_padres * 2];
            for(int i=0; i<num_padres; i++) aux[i] = padres[i];
            delete[] padres;
            padres = aux;
            max_padres *= 2;
        }
        padres[num_padres++] = id_p;
    }
    void eliminar_padres(int id){
        int pos =-1;
        for(int i=0;i<num_padres;i++){
            if(padres[i]==id){
                pos = i;
                break;
            }
        }if(pos != -1){
            for(int i=pos;i<num_padres -1; i++){
                padres[i]= padres[i+1];

            }
            num_padres--;
        }
    }

    virtual bool es_directorio() = 0; 
};
class NodoDirectorio : public NodoGrafo {
private:
    int* hijos;       
    int num_hijos;
    int max_hijos;
public:
    NodoDirectorio(int _id) : NodoGrafo(_id) {
        num_hijos = 0;
        max_hijos = 5;
        hijos = new int[max_hijos];
    }
    ~NodoDirectorio() {
        if (hijos) delete[] hijos;
    }

    bool es_directorio() { return true; }
    int* lista_hijos() {
        return hijos;
    }
    int get_cant_hijos() { return num_hijos; }

    void agregar_hijo(int id_h) {
        if (num_hijos == max_hijos) {
            int* aux = new int[max_hijos * 2];
            for(int i=0; i<num_hijos; i++) aux[i] = hijos[i];
            delete[] hijos;
            hijos = aux;
            max_hijos *= 2;
        }
        hijos[num_hijos++] = id_h;
    }
    void eliminar_hijo_de_lista(int id_hijo) {
        int pos = -1;
        for(int i=0; i<num_hijos; i++) {
            if(hijos[i] == id_hijo) {
                pos = i;
                break;
            }
        }
        if(pos != -1) {
            for(int i = pos; i < num_hijos - 1; i++) {
                hijos[i] = hijos[i+1];
            }
            num_hijos--;
        }
    }
};

class NodoArchivo: public NodoGrafo {
private:
    int tamaño;
    string nombre;
    int tipo; // 0: Imagen; 1: Documento; 2: ejecutable; 3: Video; 4: Comprimido
public:
    NodoArchivo(int id, int tamano, int tipo, string nombre) : NodoGrafo(id) {
        tamaño = tamano;
        tipo = tipo;
        nombre = nombre;
    }
    bool es_directorio() { return false; }
    int get_tamano() { return tamaño; }
    int get_tipo() { return tipo; }
    string get_nombre() { return nombre; }
};