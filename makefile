# Nombre del ejecutable final
TARGET = spotify_pirata

# Compilador a usar
CC = gcc

# Banderas de compilación (opcionales pero recomendadas)
# -Wall: muestra todas las advertencias
# -g: añade información de depuración
CFLAGS = -Wall -g

# Lista de todos tus archivos .c
SRCS = main.c spotifycola.c spotifypila.c spotifylistas.c

# Regla principal: compila el ejecutable
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET)
run:
	./$(TARGET) archivo.txt
