## Makefile para compilacion de TP Grupal realizado en lenguaje C++

# CONFIGURACION
################

# Nombre del ejecutable.
target = Worms

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp



# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = -Wall -pedantic #-Werror -pedantic-errors

# Para que explote lo mas posible
CFLAGS += -O0 -DNDEBUG

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline

# Opciones del enlazador.
#LDFLAGS =

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++98

# Archivo log de Valgrind.
VGLOGFILE = valgrind.out

# Flags para el Valgrind
VGFLAGS = --tool=memcheck --trace-children=yes --track-fds=yes --time-stamp=yes --num-callers=20 --error-exitcode=42 --db-attach=no --leak-check=full --leak-resolution=med --track-origins=yes --log-file=$(VGLOGFILE) 



# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################


# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
#CXXFLAGS += -I/usr/local/include/SDL2 -I/usr/local/include/SDL2_image -I/usr/local/include/SDL2_gfx -I/usr/local/include/yaml-cpp -I/usr/local/include/Box2D -I/usr/local/include/libpng16 -I/usr/local/include/boost
LD = $(CXX)
#LDFLAGS += -L/usr/local/lib/
LDLIBS += -lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2main -lSDL2_test -lyaml-cpp -lBox2D -lpng -lpng16 -ljpeg
endif

# Si no especifica archivos, tomo todos.
fuentes ?= $(wildcard *.$(extension))

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

all: $(target)

o_files = $(patsubst %.$(extension),%.o,$(fuentes))

$(target): $(o_files)
	@if [ -z "$(o_files)" ]; \
	then \
		echo "No hay archivos de entrada, recuerde que la extensión debe ser '.$(extension)'."; \
		false; \
	fi >&2
	$(LD) $(LDFLAGS) $(o_files) $(LOADLIBS) $(LDLIBS) -o $(target)

clean:
	$(RM) $(o_files) $(VGLOGFILE) $(target)

run:
	./$(target)

valgrind:
	valgrind $(VGFLAGS) ./$(target) 
