#include "MPointerGC.h"
#include "../MPointer.h"
#include <chrono>


/**
 * Inicialización de parámetros
 */
bool MPointerGC::active = false;
MPointerGC* MPointerGC::instance = nullptr;

/**
 * Revisa si la ya fue instanciado el singleton
 * @return true o false
 */
bool MPointerGC::isActive() {
    return active;
}

/**
 * Inicializa el singleton de la clase
 * @return instance
 */
MPointerGC *MPointerGC::getInstance() {
    if(!instance){
        instance = new MPointerGC();
    }
    active = true;
    return instance;
}


/**
 * Genera los ID de los nuevos datos agregados a la lista
 * @return ID
 */
string MPointerGC::generarID() {
    this->ID ++;
    return "GC-" + to_string(ID);
}

/**
 * Función para agregar MPointer a datos repetidos
 * @param nuevo dirección de memoria del dato repetido
 * @return el ID del dato repetido
 */
void MPointerGC::addRepitedPointer(string id) {
    listaMPointer.aumentarRef(id);
}

/**
 * Imprime la lista con las posiciones de memoria guardadas
 */
void MPointerGC::imprimirLista() {
    for(int i = 0; i<listaMPointer.getLenght(); i++){
        cout<<listaMPointer.getDato(i)<<endl;
        cout<<listaMPointer.getNodo(i).cantRef<<endl;
    }
}

/**
 * Cuando se elimina un puntero a un dato la función elimina la cantidad de referencias
 * que este posea, si esta llega a cero elimina el dato de la lista (separar a la función del Thread)
 * @param id ID del dato al que se le ha eliminado la referencia
 */
void MPointerGC::eliminarReferencia(string id) {
    if(listaMPointer.getCantRefPorID(id) == 0){
        return;
    }
    else {
        listaMPointer.disminuirRef(id);
    }
}

/**
 * Thread encargado de revisar las referencias de la lista y eliminarlos si
 * ya no son referenciados
 */
void MPointerGC::revisaLista() {
    cout <<endl<< "Acabé la revisión una vez" << endl;
    while(isActive()) {
        for (int i = 0; i < listaMPointer.getLenght(); i++) {
            if (listaMPointer.getNodo(i).cantRef == 0) {
                listaMPointer.eliminarNodo(i);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1/10000));
    }
}

void MPointerGC::threadFunc(MPointerGC *param) {
    param->revisaLista();
}

void MPointerGC::stopThread() {
    this->active = false;
    thread_m->join();
}

