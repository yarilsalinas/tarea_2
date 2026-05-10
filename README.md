# Tarea 2
 ## objetivo:
El objetivo de este programa es procesar un catálogo de películas desde un archivo CSV (Top1500.csv) que permitir al usuario interactuar con el de manera eficiente mediante el uso de (TDAs) como Listas y Mapas. El sistema permite búsquedas rápidas, gestión de una lista personal y un sistema de calificaciones por usuario.
  
 ## en este programa lo que hacemos es darle al usuario 8 opciones:
  1) Cargar el archivo: Al seleccionar esta opcion el programa lee el csv y mientras lee cada crea una estructura del tipo Film, asigna su ID, asigna su Tituo, asigna el director o directores en una lista al igual que los generos y tambien asigna la fecha de lanzamiento como un numero entero.
  2) Buscar por genero: Se le pide al usuario 
     
 
 
 ## Comandos (tarea_2)
 Para compilar usamos:
 ````
 gcc tdas/*.c tarea_2.c -Wno-unused-result -o tarea_2
 ````

 Y luego ejecutar:
 ````
 ./tarea_2
 ````
 Subir cambios:
  ````
  git add .
  git commit -m "..."
  git push
  ````
  donde comit "..." es donde escribimos los cambios que subimos

actualizar:
  ````
  git pull
  ````