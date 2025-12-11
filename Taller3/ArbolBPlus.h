#pragma once
#include "NodoBPlusBase.h" 
#include <iostream>

class ArbolBPlus {
private:
    NodoBPlusBase* raiz;
    int orden; 
    void insertar_en_nodo_interno_con_espacio(NodoBInterno* padre, int clave, NodoBPlusBase* hijo_derecho) {
        int k = padre->getCantidad_clave();
        int* claves = padre->getClaves();
        NodoBPlusBase** punteros = padre->getPunteros();

        // Encontrar la posición correcta desde el final
        while (k > 0 && claves[k - 1] > clave) {
            claves[k] = claves[k - 1];          // Desplazar clave
            punteros[k + 1] = punteros[k];      // Desplazar puntero
            k--;
        }

        claves[k] = clave;
        punteros[k + 1] = hijo_derecho;
        padre->setCantidad_claves(padre->getCantidad_clave() + 1);
    }
    void insertar_en_padre(NodoBPlusBase* n, int clave, NodoBPlusBase* nuevo_nodo) {
        // Caso Base: Si n es la raíz, el árbol crece en altura
        if (n == raiz) {
            NodoBInterno* nueva_raiz = new NodoBInterno(orden);
            nueva_raiz->agregar(clave); // Agregar la clave divisoria
            nueva_raiz->setPunteros(0, n);
            nueva_raiz->setPunteros(1, nuevo_nodo);

            n->setPadre(nueva_raiz);
            nuevo_nodo->setPadre(nueva_raiz);
            raiz = nueva_raiz;
            return;
        }

        // Caso Recursivo: Insertar en el padre existente
        NodoBInterno* padre = (NodoBInterno*)n->getPadre();

        if (padre->getCantidad_clave() < orden) {
            // Si hay espacio, insertamos ordenadamente y terminamos
            insertar_en_nodo_interno_con_espacio(padre, clave, nuevo_nodo);
        } else {
            // Si el padre también está lleno, debemos dividirlo
            split_interno(padre, clave, nuevo_nodo);
        }
    }
    void split_hoja(NodoBHoja* hoja, int id_extra, NodoGrafo* dato_extra) {
        // Crear temporales
        int* claves_temp = new int[orden + 1];
        NodoGrafo** datos_temp = new NodoGrafo*[orden + 1];

        int total = hoja->getCantidad_clave();
        int* claves_orig = hoja->getClaves();
        NodoGrafo** datos_orig = hoja->getDatos();

        // Copiar todo al temporal
        for (int i = 0; i < total; i++) {
            claves_temp[i] = claves_orig[i];
            datos_temp[i] = datos_orig[i];
        }

        // Insertar el nuevo ordenadamente
        int i = total - 1;
        while (i >= 0 && claves_temp[i] > id_extra) {
            claves_temp[i + 1] = claves_temp[i];
            datos_temp[i + 1] = datos_temp[i];
            i--;
        }
        claves_temp[i + 1] = id_extra;
        datos_temp[i + 1] = dato_extra;

        // Crear nueva hoja
        NodoBHoja* nueva_hoja = new NodoBHoja(orden);
        
        int split_idx = (orden + 1) / 2;

        // Llenar hoja izquierda (vieja) - sobreescribimos
        hoja->setCantidad_claves(0);
        for (int j = 0; j < split_idx; j++) {
            hoja->insertar_dato(claves_temp[j], datos_temp[j]);
        }

        // Llenar hoja derecha (nueva)
        for (int j = split_idx; j < orden + 1; j++) {
            nueva_hoja->insertar_dato(claves_temp[j], datos_temp[j]);
        }

        // Enlazar hojas (Lista enlazada)
        nueva_hoja->setSiguiente(hoja->getSiguiente());
        hoja->setSiguiente(nueva_hoja);
        
        nueva_hoja->setPadre(hoja->getPadre());

        // Clave a promocionar (COPIA de la primera clave de la derecha)
        int clave_promocion = nueva_hoja->getClaves()[0];

        delete[] claves_temp;
        delete[] datos_temp;

        // Llamar a inserción en el padre
        insertar_en_padre(hoja, clave_promocion, nueva_hoja);
    } 
    void split_interno(NodoBInterno* nodo, int clave_extra, NodoBPlusBase* hijo_extra) {
        // Usamos arrays temporales para ordenar todo (n + 1 claves)
        int* claves_temp = new int[orden + 1];
        NodoBPlusBase** punteros_temp = new NodoBPlusBase*[orden + 2];

        int total_claves = nodo->getCantidad_clave();
        int* claves_actuales = nodo->getClaves();
        NodoBPlusBase** punteros_actuales = nodo->getPunteros();

        // 1. Copiar y ordenar datos en temporales
        int i = 0, j = 0;
        // Encontrar dónde va la nueva clave
        while (i < total_claves && claves_actuales[i] < clave_extra) {
            claves_temp[j] = claves_actuales[i];
            punteros_temp[j] = punteros_actuales[i];
            i++; j++;
        }
        // Insertar lo nuevo
        claves_temp[j] = clave_extra;
        punteros_temp[j] = punteros_actuales[i]; // El puntero izquierdo de la clave extra es el actual[i]
        // (Corrección lógica: El puntero asociado a la clave insertada va a su derecha)
        // Re-ajuste: punteros_actuales[i] es el hijo que ya estaba. 
        // hijo_extra es el nuevo hijo derecho de clave_extra.
        punteros_temp[j+1] = hijo_extra; 
        
        // El puntero izquierdo de 'clave_extra' ya está en punteros_temp[j].
        // Necesitamos copiar el resto.
        
        // Rectificación de la copia para simplificar:
        // Copiamos todo tal cual y luego insertamos.
        for(int k=0; k<total_claves; k++) claves_temp[k] = claves_actuales[k];
        for(int k=0; k<total_claves+1; k++) punteros_temp[k] = punteros_actuales[k];
        
        // Inserción manual en el temporal (Insertion Sort)
        int k = total_claves;
        while(k > 0 && claves_temp[k-1] > clave_extra) {
            claves_temp[k] = claves_temp[k-1];
            punteros_temp[k+1] = punteros_temp[k];
            k--;
        }
        claves_temp[k] = clave_extra;
        punteros_temp[k+1] = hijo_extra;
        
        // 2. Crear nuevo nodo interno
        NodoBInterno* nuevo_interno = new NodoBInterno(orden);
        
        // 3. Distribuir
        int split_idx = (orden + 1) / 2; // Punto medio
        
        // La clave del medio SUBE al padre (no se queda en ninguno)
        int clave_que_sube = claves_temp[split_idx];

        // Reconstruir nodo izquierdo (viejo)
        nodo->setCantidad_claves(0); // Reiniciamos contador para llenar de nuevo
        // Ojo: puntero 0 se mantiene, llenamos claves y punteros 1..idx
        for(int z = 0; z < split_idx; z++) {
            nodo->agregar(claves_temp[z]);
            nodo->setPunteros(z+1, punteros_temp[z+1]); // Punteros derechos
        }
        // Nota: El puntero 0 original de nodo ya es correcto.

        // Construir nodo derecho (nuevo)
        // El puntero 0 del nuevo nodo es el puntero a la derecha de la clave que subió
        nuevo_interno->setPunteros(0, punteros_temp[split_idx+1]); 
        
        for(int z = split_idx + 1; z < orden + 1; z++) {
            nuevo_interno->agregar(claves_temp[z]);
            nuevo_interno->setPunteros(nuevo_interno->getCantidad_clave(), punteros_temp[z+1]);
        }

        // Actualizar padres de los hijos movidos
        for(int z = 0; z <= nuevo_interno->getCantidad_clave(); z++) {
            if(nuevo_interno->getPunteros()[z])
                nuevo_interno->getPunteros()[z]->setPadre(nuevo_interno);
        }

        nuevo_interno->setPadre(nodo->getPadre());

        delete[] claves_temp;
        delete[] punteros_temp;

        // 4. Subir recursivamente
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
        // 1. Árbol vacío
        if (raiz == nullptr) {
            raiz = new NodoBHoja(orden);
            ((NodoBHoja*)raiz)->insertar_dato(id, nodo);
            return;
        }

        // 2. Buscar la hoja correspondiente
        NodoBPlusBase* actual = raiz;
        while (!actual->getHoja()) {
            NodoBInterno* interno = (NodoBInterno*)actual;
            int idx = interno->buscar_siguiente(id);
            actual = interno->getPunteros()[idx];
        }
        NodoBHoja* hoja = (NodoBHoja*)actual;

        // 3. Insertar o Dividir
        if (hoja->getCantidad_clave() < orden) {
            hoja->insertar_dato(id, nodo);
        } else {
            // Hoja llena -> Split
            split_hoja(hoja, id, nodo);
        }
    }
};