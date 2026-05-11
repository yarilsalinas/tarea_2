#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *directors; //lista de directores
  List *genres;//Lista de géneros
  int year;
  Map *calificaciones;//Mapa de calificaciones Key: Usuario ; data: Calificación
} Film;

void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por género");
  puts("3) Buscar por autor");
  puts("4) Buscar por decada");
  puts("5) Buscar por Genero y Decada");
  puts("6) Gestión de watchlist");
  puts("7) Calificar pelicula");
  puts("8) Salir");
}

int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
  }

int is_equal_int(void *key1, void *key2) {
    return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
  }

void imprimir_lista_strings(List *lista) {
    char *item = list_first(lista);
    while (item != NULL) {
        printf("%s", item);
        item = list_next(lista);
        if (item != NULL) printf(", ");
    }
    printf("\n");
}

void mostrar_calificaciones(Film *peli){//muestra todas las calificaciones de una pelicula para no hacer repetitivo el codigo
  MapPair *pair = map_first(peli -> calificaciones);
  if(pair == NULL){
    printf("No a recibido calificaciones aún\n");
    return;
  }
  printf("Calificaciones : ");
  while(pair != NULL){
    char *usuario = pair -> key;
    int *nota = pair -> value;
    printf("%s : %d \n", usuario, *nota);

    pair = map_next(peli -> calificaciones);
  }
}

void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirectors, Map *pelis_bydecada) { 
  FILE *archivo = fopen("Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); 
    return;
  }
  char **campos;
    // Leer y parsear una línea del archivo CSV. La función devuelve un array de
    // strings, donde cada elemento representa un campo de la línea CSV procesada.
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

    // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Film *peli = (Film *)malloc(sizeof(Film));// Crea una nueva estructura Film y almacena los datos de cada película
    strcpy(peli->id, campos[1]);        // Asigna ID
    strcpy(peli->title, campos[5]);     // Asigna título
    peli->directors = split_string(campos[14], ",");// Asigna director
    peli->genres = split_string(campos[11], ",");       // Inicializa la lista de géneros
    peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero
    peli->calificaciones = map_create(is_equal_str);

    
    map_insert(pelis_byid, peli->id, peli);// Inserta la película en el mapa usando el ID como clave
    char *genre = list_first(peli->genres);// Obtiene el primer género de la lista de géneros de la película  
    while (genre != NULL) {// Itera sobre cada género de la película
      MapPair *genre_pair = map_search(pelis_bygenres, genre);// Busca el género en el mapa pelis_bygenres
      if (genre_pair == NULL) {// Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
        List *new_list = list_create();
        list_pushBack(new_list, peli);
        map_insert(pelis_bygenres, genre, new_list);
      } else {// Si el género ya existe en el mapa, obtén la lista y agrega la película
          List *genre_list = (List *)genre_pair->value;
          list_pushBack(genre_list, peli);
        }
      genre = list_next(peli->genres);// Avanza al siguiente género en la lista
    }

    char *director = list_first(peli->directors);
    while (director != NULL) {
      MapPair *director_pair = map_search(pelis_bydirectors, director);
      if (director_pair == NULL) {
        List *new_list = list_create();
        list_pushBack(new_list, peli);
        map_insert(pelis_bydirectors, director, new_list);
        } else {
            List *director_list = (List *)director_pair->value;
            list_pushBack(director_list, peli);
        }
        director = list_next(peli->directors);
      }

    int decada_val = (peli->year / 10) * 10;  
    char *decada_key = malloc(5 * sizeof(char));
    sprintf(decada_key, "%d", decada_val); 
    MapPair *decada_pair = map_search(pelis_bydecada, decada_key);

    if (decada_pair == NULL) { 
      List *new_list = list_create();
      list_pushBack(new_list, peli);
      map_insert(pelis_bydecada, decada_key, new_list);
    } else { 
        List *decada_list = (List *)decada_pair->value;
        list_pushBack(decada_list, peli); 
        free(decada_key); 
    }
    }
    printf("archivo cargado correctamente\n");
    fclose(archivo); // Cierra el archivo después de leer todas las líneas
  }

void buscar_por_genero(Map *pelis_bygenres) {
  char genre[100];
  printf("Ingrese el género a buscar: ");
  scanf(" %99[^\n]", genre);

  MapPair *pair = map_search(pelis_bygenres, genre); // busca el género en el mapa
  if (pair == NULL) {
      printf("\nNo se encontraron peliculas para el genero: %s\n", genre);
      return;
  }

  List *lista = (List *)pair->value; // lista de peliculas de el genero a buscar
  Film *peli = list_first(lista); 

  printf("\nResultados para el genero '%s':\n", genre);
  while (peli != NULL) { // recorre la lista
    printf("---------------------------\n");
    printf("ID      : %s\n", peli->id);
    printf("Titulo  : %s\n", peli->title);
    printf("Generos : ");
    imprimir_lista_strings(peli->genres);
    mostrar_calificaciones(peli);
    peli = list_next(lista);
  }
  printf("---------------------------\n");
}

void buscar_por_director(Map *pelis_bydirectors) {
  char director[100];
  printf("Ingrese el director a buscar: ");
  if (scanf(" %99[^\n]", director) != 1) return;

  MapPair *pair = map_search(pelis_bydirectors, director); // buscar el director en el mapa
  if (pair == NULL) {
      printf("\nNo se encontraron peliculas para el director: %s\n", director);
      return;
  }

  List *lista = (List *)pair->value; // lista de peliculas de ese director
  Film *peli = list_first(lista);

  printf("\nResultados para el director '%s':\n", director);
  while (peli != NULL) { // recorre la lista
    printf("---------------------------\n");
    printf("ID         : %s\n", peli->id);
    printf("Titulo     : %s\n", peli->title);
    printf("Directores : ");
    imprimir_lista_strings(peli->directors);
    mostrar_calificaciones(peli);
    peli = list_next(lista);
  }
  printf("---------------------------\n");
}

void buscar_por_decada(Map *pelis_bydecada){
  char decada[10];
  printf("Ingrese la decada a buscar: ");
  scanf("%s", decada);

  MapPair *pair = map_search(pelis_bydecada, decada); // buscar la decada en el mapa
  if (pair == NULL) {
      printf("\nNo se encontraron peliculas para la decada: %s\n", decada);
      return;
  }

  List *lista = (List *)pair->value; // Lista de peliculas de esa decada
  Film *peli = list_first(lista);

  printf("\nResultados para la decada '%s':\n", decada);
  while (peli != NULL) { // recorre la lista
    printf("---------------------------\n");
    printf("ID     : %s\n", peli->id);
    printf("Titulo : %s\n", peli->title);
    printf("Fecha  : %d\n", peli->year);
    mostrar_calificaciones(peli);
    peli = list_next(lista);
  }
  printf("---------------------------\n");
}

void busqueda_avanzada(Map *pelis_bygenre){
  char genre[100];
  int decada;
  
  printf("Ingrese el genero a buscar: ");
  scanf(" %[^\n]", genre);

  MapPair *pair = map_search(pelis_bygenre, genre); // buscar el genero en el mapa
  if(pair == NULL){ // si el genero no existe se retorna
    printf("No existe el genero: %s\n", genre);
    return;
  }
  // si si existe el genero, pedimos que ingrese la decada
  printf("Ingrese la decada a buscar: ");
  scanf("%d", &decada);
  if(decada % 10 != 0){ // si la decada ingresada termina en un numero que no es 0 entonces se termina el ciclo al no ser valida.
    printf("Ingrese una decada valida\n");
    return;
  }


  List *lista = pair -> value; // lista de peliculas del genero a buscar
  Film *peli = list_first(lista);

  int contador = 0; //Peliculas q cumplen ambos parametros
  while(peli != NULL){
    if(peli -> year >= decada && peli -> year <= decada + 9){ // verifica si la pelicula es de la decada ingresada
      printf("---\n");
      printf("ID: %s\n", peli -> id);
      printf("Titulo: %s\n", peli -> title);
      printf("Año: %d\n", peli -> year);
      mostrar_calificaciones(peli);
      contador++; // aumenta si cumple ambos parametros(genero y decada)
    }
    peli = list_next(lista);
  }
  if(contador == 0){
    printf("No se encontraron peliculas que cumplan ambos parametros\n");//no existen peliculas de ese genero en esa decada
  }
}

void mi_watchlist(Map *pelis_byid, List *watchlist) {
  char opcion;
  char id_buscado[10];
  Film *p;
  do {
    printf("\n--- GESTIÓN DE WATCHLIST ---\n");
    printf("1) Agregar película\n");
    printf("2) Eliminar película\n");
    printf("3) Mostrar mi lista\n");
    printf("4) Volver al menú principal\n");
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
      case '1': // agregar pelicula
        printf("Ingrese el ID de la película: ");
        scanf("%s", id_buscado);

        MapPair *pair = map_search(pelis_byid, id_buscado); // busca la pelicula a ingresar
        if (pair != NULL) { // si la pelicula existe, se agrega al final de la watchlist
          Film *peli_encontrada = (Film *)pair->value;
          list_pushBack(watchlist, peli_encontrada);
          printf("'%s' agregada con éxito\n", peli_encontrada->title);
        } else {// si no existe, se muestra un mensaje
            printf("La película con ID %s no existe.\n", id_buscado);
        }
        break;

      case '2': // eliminar pelicula
        printf("Ingrese el ID de la película a eliminar: ");
        scanf("%s", id_buscado);

        p = list_first(watchlist);
        int encontrado = 0; // no se a encontrado la pelicula
        while (p != NULL) { // recorre toda la watchlist, comparando ids.
        if (strcmp(p->id, id_buscado) == 0) { //si los id son iguales elimina la pelicula
          printf("Película '%s' eliminada de tu Watchlist.\n", p->title); 
          list_popCurrent(watchlist); 
          encontrado = 1;// se encontro la pelicula a buscar
          break;
        }
        p = list_next(watchlist);
        }
        if (!encontrado) { // si no encuentra muestra un mensaje en pantalla
          printf("No se encontró película con id: %s\n", id_buscado);
        }
        break;

      case '3': // mostrar watchlist
        printf("\n--- MI WATCHLIST ---\n");
        p = list_first(watchlist);
        if (p == NULL) {
          printf("Tu lista está vacía.\n");
        } else {
            while (p != NULL) {
            printf("---------------------------\n");
            printf("ID     : %s\n", p->id);
            printf("Titulo : %s\n", p->title);
            printf("Fecha  : %d\n", p->year);
            mostrar_calificaciones(p);
            p = list_next(watchlist);
            }
        printf("---------------------------\n");
        }
        break;
      }
        if (opcion != '4') presioneTeclaParaContinuar();

    } while (opcion != '4');
}

void calificar_pelicula(Map *pelis_byid){
  char id[100];
  char usuario[100];
  int nota;

  printf("Ingrese el id de la pelicula a calificar: ");
  scanf("%s", id);
  MapPair *pair = map_search(pelis_byid, id); // buscar la pelicula por id

  if(pair == NULL){ // si la pelicula no existe
    printf("Pelicula no encontrada\n");
    return;
  }
  Film *peli = pair -> value; // valor de la pelicula encontrada
  printf("Ingrese el nombre del usuario: ");
  scanf(" %99[^\n]", usuario);

  printf("Ingrese la nota de 1 a 10: ");
  scanf("%d", &nota);

  if(nota < 1 || nota > 10){ // verifica si la nota es valida
    printf("Nota no valida\n");
    return;
  }
  MapPair *cali_pair = map_search(peli -> calificaciones, usuario); // busca si el usuario ya califico la pelicula anteriormente

  if(cali_pair != NULL){ // si ya existe se sobreescribe
    *((int *)cali_pair -> value) = nota;
    printf("Se a ingresado correctamente la nota\n");
  }
  else{// sino a calificado la pelicula
    int *nuevaNota = malloc(sizeof(int)); // se reserva memoria para ingresar la calificacion
    *nuevaNota = nota;

    char *nuevoUsuario = malloc(strlen(usuario) + 1); // se reserva memoria para el nombre del usuario
    strcpy(nuevoUsuario, usuario);
    map_insert(peli -> calificaciones, nuevoUsuario, nuevaNota); // se inserta el usuario y la nota
    printf("nota agregada correctamente\n");
  }
}

int main() {
  char opcion;
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);
  Map *pelis_bydirectors = map_create(is_equal_str);
  Map *pelis_bydecada = map_create(is_equal_str);
  List *watchlist = list_create();
  
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid, pelis_bygenres, pelis_bydirectors, pelis_bydecada);
      break;
    case '2':
      buscar_por_genero(pelis_bygenres);
      break;
    case '3':
      buscar_por_director(pelis_bydirectors);
      break;
    case '4':
      buscar_por_decada(pelis_bydecada);
      break;
    case '5':
      //Búsqueda Avanzada (Doble Criterio): La aplicación solicita a la usuaria un género y una década simultáneamente. El sistema debe mostrar solo los títulos que cumplan con ambos requisitos (ej. "Terror" de los "1990").
      busqueda_avanzada(pelis_bygenres);
      break;
    case '6':
      //6. **Gestionar Mi Watchlist:** El sistema permite mantener una lista de películas para ver más tarde:
      //- **Agregar:** Se solicita el **ID** de la película y se agrega a la lista.
      //- **Eliminar:** Se solicita el **ID** de la película y se retira de la lista.
      //- **Mostrar:** Despliega la información detallada de todas las películas guardadas en la Watchlist.
      mi_watchlist(pelis_byid, watchlist);
      break;
    case '7':
      //1. **Calificar Película:** La usuaria ingresa el **ID** de una película, su **nombre de usuario** (ej. "Javiera") y una         calificación (ej. del 1 al 10). La aplicación debe registrar esta nota asociada a la usuaria para la película seleccionada.
      //Nota:* Si la usuaria ya había calificado la película, la nueva nota debe sobrescribir la anterior.
      calificar_pelicula(pelis_byid);
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}