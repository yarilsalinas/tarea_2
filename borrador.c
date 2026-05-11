//borrado de carar csv
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


//borrado buscar por ID
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

// void mostrar_calificaciones(Film *peli){//muestra todas las calificaciones de una pelicula para no hacer repetitivo el codigo
  MapPair *pair = map_first(peli -> calificaciones);
  if(pair == NULL){ //si no tiene calificacion
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