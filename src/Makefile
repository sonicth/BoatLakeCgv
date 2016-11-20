#LIBPATH += -L"/System/Library/Frameworks/OpenGL.framework/Libraries"

FRAMEWORK = -framework GLUT
FRAMEWORK += -framework OpenGL 

COMPILERFLAGS = -Wall -W
CC = cc
CFLAGS = $(COMPILERFLAGS) 
#LIBRARIES = -lGL -lGLU -lm -lobjc -lstdc++ 
#COMPILE = $(CC) $(CFLAGS) -c

#SOURCES = $(wildcard *.c) 		#alternative, if ALL files need to be used
SRCFILES = boat_model.c \
	geometry.c \
	landscape.c \
	open.c \
	perlin.c
	
HFILES = open.h

TARGET=boat						#name of the executable

OBJFILES = $(SRCFILES:.c=.o)
								#alternative
#OBJFILES := $(patsubst %.c,%.o,$(SOURCES))
	
All: $(TARGET)

$(TARGET): $(OBJFILES) $(HFILES)
	$(CC) $(FRAMEWORK) $(CFLAGS) -o $@ $(OBJFILES)

%.o: %.c $(HFILES)					#alternative
#.c.o: 					#this line does not get to recompile when header file(s) are changed
	$(CC) $(CFLAGS) -c $<		#no need for $@; doing like "cc fish.c -o fish" complaines about _main

#$(LIBRARIES) 
#	./boat

clean:
	rm *.o $(TARGET)

