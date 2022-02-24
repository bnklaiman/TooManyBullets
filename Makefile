#
# Makefile for saucer shoot game using Dragonfly
#
# Copyright Mark Claypool and WPI, 2016-2019
#
# 'make' to build executable
# 'make depend' to generate new dependency list
# 'make clean' to remove all constructed files
#
# Variables of interest:
#   GAMESRC is the source code files for the game
#   GAME is the game main() source
#   EXECUTABLE is the name of the runnable game
#   ENG is the name of the Dragonfly engine
#

# Compiler
CC= g++ 

# Libraries and includes.
LINKDIR= -L../dragonfly/lib # path to dragonfly library
INCDIR= -I../dragonfly/include # path to dragonfly includes

### Uncomment and update below if using local SFML installation.
# Note: if homebrew on Mac, sfml install, may be in:
#   /usr/local/Cellar/sfml/2.5.1
LOCALSFML= $(HOME)/src/SFML-2.5.1
LINKDIR:= $(LINKDIR) -L $(LOCALSFML)/lib
INCDIR:= $(INCDIR) -I$(LOCALSFML)/include

### Uncomment either 1) or 2) below! ###

## 1) For Linux:
CFLAGS= -Wall
LINKLIB= -ldragonfly-linux64 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lrt

## 2) For Mac, uncomment one of two below based on arch:
#ENG= dragonfly-x64-mac
#ENG= dragonfly-arm64-mac
#CFLAGS= -MD
#LINKLIB= -l$(ENG) -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio 

######

GAMESRC= \

GAME= game.cpp
EXECUTABLE= game
OBJECTS= $(GAMESRC:.cpp=.o)
CFLAGS:= $(CFLAGS) -std=c++11

.PHONY: all clean

all: $(EXECUTABLE) Makefile

$(EXECUTABLE): $(OBJECTS) $(GAME) $(GAMESRC) 
	$(CC) $(CFLAGS) $(GAME) $(OBJECTS) -o $@ $(INCDIR) $(LINKDIR) $(LINKLIB) 

.cpp.o: 
	$(CC) $(CFLAGS) -c $(INCDIR) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) core dragonfly.log Makefile.bak *~
# DO NOT DELETE
