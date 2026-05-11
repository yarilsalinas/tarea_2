# Tarea 2
## objetivo:
El objetivo de este programa es procesar un catálogo de películas desde un archivo CSV (Top1500.csv) que permitir al usuario interactuar con el de manera eficiente mediante el uso de (TDAs) como Listas y Mapas. El sistema permite búsquedas rápidas, gestión de una lista personal y un sistema de calificaciones por usuario.
  
 ## en este programa lo que hacemos es darle al usuario 8 opciones:
  1) Cargar el archivo: Al seleccionar esta opcion el programa lee el csv y mientras lee cada crea una estructura del tipo Film, asigna su ID, asigna su Tituo, asigna el director o directores en una lista al igual que los generos y tambien asigna la fecha de lanzamiento como un numero entero.
     
  2) Buscar por Género: Permite al usuario ingresar un género (ej: "Action") y muestra todas las películas que contengan ese genero.

  3) Buscar por Director: Muestra por pantalla todos los titulos dirigidos por el autor ingresado, ademas de el ID y los demas directores si es que tiene.

  4) Buscar por Década: Filtra las películas según la década de lanzamiento (ej: 1990).

  5) Búsqueda Avanzada (Doble Criterio): Filtra de forma simultánea por género y década, mostrando solo los títulos que cumplan ambos requisitos.

  6) Gestión de Watchlist:
Agregar: Añade una película a tu lista personal mediante su ID.
Eliminar: Quita una película de la lista usando su ID.
Mostrar: Lista todos los detalles de las películas guardadas.

  7) Calificar Película: Permite asignar una nota (1-10) a una película. Se solicita el ID y un nombre de usuario. Si el usuario ya calificó esa película, la nueva nota sobrescribirá la anterior. Al mostrar las películas en cualquier menú, se desplegarán las calificaciones de los usuarios si existen.

  8) Salir: Finaliza la ejecución del programa.

## Problemas con las opciones:
### Rigidez en la Búsqueda Avanzada: 
no se puede realizar una busqueda de filtro avanzado que filtre por autor y un criterio mas.
### Problema de Carga Duplicada (Consistencia):
Si el usuario selecciona la opción 1 (Cargar Películas) más de una vez, el programa volverá a leer el CSV y duplicará las entradas en las listas de los mapas y en la watchlist
### Sensibilidad a Mayúsculas y Espacios:
Las funciones map_search utilizan strcmp. Esto significa que si el usuario busca el género "action" (en minúsculas) y en el CSV aparece como "Action", el programa dirá que no se encontraron resultados, pasa igual con autores y titulos.
### Limitaciones en la Watchlist:
El usuario puede agregar la misma película 10 veces a su watchlist. No hay una comprobación de "Si la película ya está en la lista, no agregar".

## Estructura de Datos (TDAs)

### Para garantizar un rendimiento óptimo, el programa utiliza:
1) Mapas (Hash Maps): Para acceder de forma instantánea complejidad: O(1) promedio a las películas por su ID, y para agrupar listas de películas por Género, Director y Década.
2) Listas Enlazadas: Utilizadas dentro de los mapas para manejar colisiones de categorías (varias películas en un mismo género) y para la gestión de la Watchlist.
 
 
## Instrucciones de Compilación y Ejecución
### Compilación
Usa el siguiente comando en la terminal para compilar el programa junto con las librerías de TDAs:
 ````
 gcc tdas/*.c tarea_2.c -Wno-unused-result -o tarea_2
 ````

 ### Ejecución
Una vez compilado, ejecuta el programa con:
 ````
 ./tarea_2
 ````

## integrantes y aportaciones:

## Sergio torres: 
 Crear las funciones:
1) buscar_por_genero
2) busqueda_avanzada
3) calificar_pelicula
4) mostrar_calificaciones
5) cargar archivo csv

## Yaril Salinas:
  Crear las funciones:
1) mi_watchlist
2) buscar_por_director
3) buscar_por_decada
4) imprimir_lista_strings
5) cargar la tarea de ejemplo para empezar a trabajar desde ahi

## En conjunto:
  completar el main.
  ajustar el texto que se muestra por pantalla y los mensajes.
 
### Comandos Git (Flujo de Trabajo)
Para mantener el repositorio actualizado y subir tus cambios:

### Subir cambios:
  ````
  git add .
  git commit -m "..."
  git push
  ````
  donde comit "..." es donde escribimos los cambios que subimos

### Actualizar repositorio local:
  ````
  git pull
  ````