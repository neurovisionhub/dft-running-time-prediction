#ifndef USERU_H
#define USERU_H

#include "../glob.h"


class USER{

public:
/** Esta funcion permite que el usuario experto implemente el codigo
 *  necesario para seleccionar el procesador que recibira la consulta 
 *  desde la maquina broker.
 *
 *  @param *terminos_largo mapa de IDTs y largo listas posting.
 *  @param *lista_procs lista de punteros a los procesadores.
 *  @param *lista_procs_lcache lista de punteros a los procesadores LCache.
 */
int User_select_manager( map<int/*id_termino*/,int/*largo*/> *terminos_largo,
                         set<int> *lista_procs, 
                         set<int> *lista_procs_lcache 
                       )
{
  int id_procesador = 0;
  return id_procesador;
}

/**
 * Esta funcion permite que el usuario tome la lista de terminos
 * de la consulta junto con los largos de sus posting lists y obtenga
 * en la variable de salida "particion" el identificador del termino junto
 * con el largo de este termino que sera asignado al procesador id_proc
 *
 * @param *terminos_largo mapa de IDTs y largo listas posting.
 * @param id_proc .
 * @param *particion .
 */
void User_select_particion( map<int/*id_termino*/,int/*largo*/> *terminos_largo,
                            int id_proc,
                            map<int,int> *particion
                          )
{

}    
};
#endif
