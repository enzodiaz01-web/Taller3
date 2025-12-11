#pragma once
#include "ArbolBPlus.h" // Asumiendo que separaste el árbol también
#include <string>
#include <iostream>
using namespace std;

class SistemaArchivos {
private:
    ArbolBPlus* arbol;
    void _rutas_recursivas(int id_actual, string ruta_parcial, string* lista, int& contador);

public:
    SistemaArchivos();
    void crear_nodo(int id_nuevo, int id_padre, std::string nombre, bool es_carpeta, int tamano = 0);
    void listar_contenido(int id_directorio);
    int calcular_espacio(int id);
    void imprimir_rutas(int id_actual, string ruta_actual); 
    // Agrega aquí los prototipos de las otras funciones (calcular_espacio, etc.)
};