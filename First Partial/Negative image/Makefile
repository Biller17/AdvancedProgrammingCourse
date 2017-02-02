#simple example of a Makefile


#variables for the compilation rules
CC = gcc #means that i want to compile with gcc instead of c


TARGET = imageModifier
CFILE = imageManipulator-AdrianBiller
CFLAGS = -std=c99
FILE = skull-icon

$(TARGET): $(CFILE).c ppmManipulator.h $(FILE).ppm
		$(CC) $(CFILE).c $(CFLAGS) -o $(TARGET)
		./$(TARGET) $(FILE).ppm

.PHONY :all
