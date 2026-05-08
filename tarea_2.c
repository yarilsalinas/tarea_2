#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *genres;
  int year;
} Film;

void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) ...");
  puts("4) ...");
  puts("5) ...");
  puts("6) ...");
  puts("7) ...");
  puts("8) Salir");

  int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
  }

  int is_equal_int(void *key1, void *key2) {
    return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
  }

  void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres) { 
    FILE *archivo = fopen("data/Top1500.csv", "r");
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
      // Crea una nueva estructura Film y almacena los datos de cada película
      Film *peli = (Film *)malloc(sizeof(Film));
      strcpy(peli->id, campos[1]);        // Asigna ID
      strcpy(peli->title, campos[5]);     // Asigna título
      peli->genres = split_string(campos[11], ",");       // Inicializa la lista de géneros
      peli->year = atoi(campos[10]); // Asigna año, convirtiendo de cadena a entero


      // Inserta la película en el mapa usando el ID como clave
      map_insert(pelis_byid, peli->id, peli);

      // Obtiene el primer género de la lista de géneros de la película
      char *genre = list_first(peli->genres);
      // Itera sobre cada género de la película
      while (genre != NULL) {
          // Busca el género en el mapa pelis_bygenres
          MapPair *genre_pair = map_search(pelis_bygenres, genre);

          // Si el género no existe en el mapa, crea una nueva lista y agrégala al mapa
          if (genre_pair == NULL) {
              List *new_list = list_create();
              list_pushBack(new_list, peli);
              map_insert(pelis_bygenres, genre, new_list);
          } else {
              // Si el género ya existe en el mapa, obtén la lista y agrega la película
              List *genre_list = (List *)genre_pair->value;
              list_pushBack(genre_list, peli);
          }

          // Avanza al siguiente género en la lista
          genre = list_next(peli->genres);
      }

    }
    fclose(archivo); // Cierra el archivo después de leer todas las líneas


    // Itera sobre el mapa para mostrar las películas cargadas
    MapPair *pair = map_first(pelis_byid);
    while (pair != NULL) {
      Film *peli = pair->value;
      printf("ID: %s, Título: %s, Año: %d\n", peli->id, peli->title,
             peli->year);

      printf("Géneros: ");
      for(char *genre = list_first(peli->genres); genre != NULL; genre = list_next(peli->genres))
        printf("%s, ", genre);
      printf("\n");

      pair = map_next(pelis_byid); // Avanza al siguiente par en el mapa
    }
  }