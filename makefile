#opciones de compilacion, muestra todos los warnings (-Wall)
CC=gcc -Wall

#carpeta de las cabeceras
HEADER_FILES_DIR = ./headerFiles
#opciones de compilacion, indica donde estan los archivos .h
INCLUDES = -I $(HEADER_FILES_DIR)

#nombre del ejecutable o archivo de salida
OUTPUT = main

#ficheros .h
HEADERS = $(HEADER_FILES_DIR)/abb.h $(HEADER_FILES_DIR)/alex.h $(HEADER_FILES_DIR)/asin.h $(HEADER_FILES_DIR)/definiciones.h $(HEADER_FILES_DIR)/errores.h $(HEADER_FILES_DIR)/se.h $(HEADER_FILES_DIR)/token.h $(HEADER_FILES_DIR)/ts.h

#archivos .c necesarios
SRCS = abb.c alex.c asin.c errores.c main.c se.c ts.c

#todos los .o con un analogo .c en SRCS
OBJS = $(SRCS:.c=.o)

#REGLA 1: genera el ejecutable, dependencia de los .o 
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS)

#REGLA 2: genera los .o cuando es necesario, dependencia de los .c y .h
#solo genera los .o necesarios por cambios en los .c o .h
%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)
	
# REGLA 3: borra el ejecutable (prerrequisito: clean)
cleanall: clean
	rm   -f $(OUTPUT) 

# REGLA 4: borra los ficheros .o y los de backup
clean:
	rm -f *.o *~
