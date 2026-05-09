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