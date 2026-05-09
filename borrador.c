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
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(pelis_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", peli->title, peli->year);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}