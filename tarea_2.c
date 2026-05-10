#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  List *directors;
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
  puts("3) Buscar por género");
  puts("4) Buscar por autor");
  puts("5) ...");
  puts("6) ...");
  puts("7) ...");
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
        if (item != NULL) printf(", "); // Separa con coma si hay más
    }
    printf("\n");
}

void cargar_peliculas(Map *pelis_byid, Map *pelis_bygenres, Map *pelis_bydirectors) { 
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
      // Crea una nueva estructura Film y almacena los datos de cada película
      Film *peli = (Film *)malloc(sizeof(Film));
      strcpy(peli->id, campos[1]);        // Asigna ID
      strcpy(peli->title, campos[5]);     // Asigna título
      peli->directors = split_string(campos[14], ",");// Asigna director
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

    }
    fclose(archivo); // Cierra el archivo después de leer todas las líneas
  }

void buscar_por_id(Map *pelis_byid) {
  char id[10];
  printf("Ingrese el ID a buscar: ");
  scanf("%s", id); 

  MapPair *pair = map_search(pelis_byid, id);
  if (pair == NULL) {
    printf("\nNo se encontraron peliculas para el ID: %s\n", id);
    return;
  }
  if (pair != NULL) {
    Film *peli = pair->value;
  
    printf("\nResultados para el ID '%s':\n", id);
    printf("---------------------------\n");
    printf("ID      : %s\n", peli->id);
    printf("Titulo  : %s\n", peli->title);
    printf("---------------------------\n");
    } else {
      // Si no se encuentra la película, informa al usuario
      printf("La película con id %s no existe\n", id);
    }
}

void buscar_por_genero(Map *pelis_bygenres) {
  char genre[100];
  printf("Ingrese el género a buscar: ");
  scanf(" %99[^\n]", genre);

  MapPair *pair = map_search(pelis_bygenres, genre);
  if (pair == NULL) {
      printf("\nNo se encontraron peliculas para el genero: %s\n", genre);
      return;
  }

  List *lista = (List *)pair->value;
  Film *peli = list_first(lista);

  printf("\nResultados para el genero '%s':\n", genre);
  while (peli != NULL) {
    printf("---------------------------\n");
    printf("ID      : %s\n", peli->id);
    printf("Titulo  : %s\n", peli->title);
    printf("Generos : ");
    imprimir_lista_strings(peli->genres);
      peli = list_next(lista);
  }
  printf("---------------------------\n");
}

void buscar_por_director(Map *pelis_bydirectors) {
  char director[100];
  printf("Ingrese el director a buscar: ");
  if (scanf(" %99[^\n]", director) != 1) return;

  MapPair *pair = map_search(pelis_bydirectors, director);
  if (pair == NULL) {
      printf("\nNo se encontraron peliculas para el director: %s\n", director);
      return;
  }

  List *lista = (List *)pair->value;
  Film *peli = list_first(lista);

  printf("\nResultados para el director '%s':\n", director);
  while (peli != NULL) {
    printf("---------------------------\n");
    printf("ID         : %s\n", peli->id);
    printf("Titulo     : %s\n", peli->title);
    printf("Directores : ");
    imprimir_lista_strings(peli->directors);
    peli = list_next(lista);
  }
  printf("---------------------------\n");
}

int main() {
  char opcion;
  Map *pelis_byid = map_create(is_equal_str);
  Map *pelis_bygenres = map_create(is_equal_str);
  Map *pelis_bydirectors = map_create(is_equal_str);

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_byid, pelis_bygenres, pelis_bydirectors);
      break;
    case '2':
      buscar_por_id(pelis_byid);
      break;
    case '3':
      buscar_por_genero(pelis_bygenres);
      break;
    case '4':
      buscar_por_director(pelis_bydirectors);
      break;
    case '5':
      break;
    case '6':
      break;
    case '7':
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}