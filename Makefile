PREFIX=/usr/
CC=gcc
W=-Wall -Wextra -Wstrict-prototypes -Wshadow -Wwrite-strings \
-Wold-style-definition -Wredundant-decls -Wnested-externs \
-Wmissing-include-dirs
CFLAGS=-lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf -lm -g $(W)
objects = list.o fsm.o geometry.o sprite.o text.o texture.o scaffold.o menustate.o playstate.o pausestate.o gameoverstate.o

scaffold : $(objects)
	$(CC) $(CFLAGS) -o scaffold $(objects)

list.o : list.h

gameoverstate.o : gamestate.h texture.h geometry.h
geometry.o : geometry.h
menustate.o : gamestate.h texture.h geometry.h
pausestate.o : gamestate.h texture.h geometry.h
playstate.o : gamestate.h texture.h geometry.h sprite.h
scaffold.o : list.h scaffold.h gamestate.h
fsm.o : gamestate.h geometry.h list.h
sprite.o : sprite.h geometry.h
text.o : text.h
texture.o : texture.h

.PHONY : cleano

run :
	./scaffold

install :
	cp ./scaffold $(PREFIX)/bin

clean :
	rm scaffold $(objects)
