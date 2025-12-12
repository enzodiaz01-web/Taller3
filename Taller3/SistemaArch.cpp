#include "SistemaArch.h"
#include <iostream>
#include <string>

using namespace std;


SistemaArchivos::SistemaArchivos() {
    arbol = new ArbolBPlus(4); 
    
    NodoDirectorio* root = new NodoDirectorio(0);
    arbol->insertar(0, root);
}


void SistemaArchivos::_rutas_recursivas(int id_actual, string ruta_parcial, string* lista, int& contador) {
    NodoGrafo* nodo = arbol->buscar(id_actual);
    if(!nodo) return;
    
    string nombre = nodo->es_directorio() ? to_string(nodo->getID()) : ((NodoArchivo*)nodo)->get_nombre();
    string nueva_ruta = "/" + nombre + ruta_parcial;

    if (nodo->get_cant_padres() == 0) {
        lista[contador++] = nueva_ruta;
        return;
    }

    int* padres = nodo->lista_padres();
    for(int i=0; i<nodo->get_cant_padres(); i++){
        _rutas_recursivas(padres[i], nueva_ruta, lista, contador);
    }
}

void SistemaArchivos::crear_nodo(int id_nuevo, int id_padre, string nombre, bool es_carpeta, int tamano) {

    NodoGrafo* nuevo;
    if (es_carpeta) nuevo = new NodoDirectorio(id_nuevo);
    else nuevo = new NodoArchivo(id_nuevo, tamano, 1, nombre);


    arbol->insertar(id_nuevo, nuevo);

    NodoGrafo* padre = arbol->buscar(id_padre);
    if (padre && padre->es_directorio()) {
        ((NodoDirectorio*)padre)->agregar_hijo(id_nuevo);
        nuevo->agregar_padre(id_padre); 
        cout << "Nodo " << id_nuevo << " creado con exito." << endl;
    } else {
        cout << "Error: Padre no existe o no es un directorio." << endl;
    }
}


void SistemaArchivos::listar_contenido(int id_directorio) {
    NodoGrafo* nodo = arbol->buscar(id_directorio);
    if (nodo && nodo->es_directorio()) {
        NodoDirectorio* dir = (NodoDirectorio*)nodo;
        int* hijos = dir->lista_hijos();
        int cant = dir->get_cant_hijos();

        cout << "Contenido de " << id_directorio << ":" << endl;
        for(int i = 0; i < cant; i++) {
            NodoGrafo* hijo = arbol->buscar(hijos[i]); 
            if (hijo) {
                if (hijo->es_directorio()) 
                    cout << " [DIR] " << hijo->getID() << endl;
                else 
                    cout << " [FILE] " << ((NodoArchivo*)hijo)->get_nombre() << endl;
            }
        }
    } else {
        cout << "ID invalido o no es un directorio." << endl;
    }
}


int SistemaArchivos::calcular_espacio(int id) {
    NodoGrafo* nodo = arbol->buscar(id); 
    if (!nodo) return 0;

    if (!nodo->es_directorio()) {
        return ((NodoArchivo*)nodo)->get_tamano();
    }

    int total = 0;
    NodoDirectorio* dir = (NodoDirectorio*)nodo;
    int* hijos = dir->lista_hijos();
    for(int i = 0; i < dir->get_cant_hijos(); i++) {
        total += calcular_espacio(hijos[i]); 
    }
    return total;
}


void SistemaArchivos::imprimir_rutas(int id_actual, string ruta_actual) {
    NodoGrafo* nodo = arbol->buscar(id_actual);
    if(!nodo) {
        cout << "Nodo no encontrado." << endl;
        return;
    }
    
    // Un pequeño hack para reutilizar la lógica:
    string* dummy = nullptr; 
    int cont = 0;
    _rutas_recursivas(id_actual, "", dummy, cont);
}

void SistemaArchivos::eliminar_nodo(int id) {
    if(id == 0) { cout << "No puedes borrar la raiz." << endl; return; }
    
    NodoGrafo* nodo = arbol->buscar(id);
    if (!nodo) {
        cout << "Error: ID no existe." << endl;
        return;
    }

    if (nodo->es_directorio()) {
        NodoDirectorio* dir = (NodoDirectorio*)nodo;
        if (dir->get_cant_hijos() > 0) {
            cout << "Error: La carpeta no esta vacia." << endl;
            return;
        }
    }


    int* padres = nodo->lista_padres();

    int cant_padres = nodo->get_cant_padres();
    
    for(int i = 0; i < cant_padres; i++) {
        NodoGrafo* nodoPadre = arbol->buscar(padres[i]);
        if(nodoPadre && nodoPadre->es_directorio()) {
             ((NodoDirectorio*)nodoPadre)->eliminar_hijo_de_lista(id);
        }
    }


    arbol->eliminar(id);
    
    cout << "Elemento eliminado con exito." << endl;
}
void SistemaArchivos::crear_enlace(int id_existente, int id_nuevo_padre) {
    NodoGrafo* nodo = arbol->buscar(id_existente);
    NodoGrafo* padre = arbol->buscar(id_nuevo_padre);

    if (nodo && padre && padre->es_directorio()) {
        int* hijos = ((NodoDirectorio*)padre)->lista_hijos();
        for(int i=0; i< ((NodoDirectorio*)padre)->get_cant_hijos(); i++){
            if(hijos[i] == id_existente) {
                cout << "El enlace ya existe." << endl;
                return;
            }
        }
        
        ((NodoDirectorio*)padre)->agregar_hijo(id_existente);
        nodo->agregar_padre(id_nuevo_padre);
        cout << "Enlace creado correctamente." << endl;
    } else {
        cout << "Error: IDs invalidos." << endl;
    }
}