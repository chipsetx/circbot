.PHONY: clean

all: circbot

circbot: utility.o circbot.o
	gcc utility.o circbot.o -o bot

utility.o: utility.c
	gcc -c utility.c

circbot.o: circbot.c
	gcc -c circbot.c

clean:
	rm -f *.o bot.exe output
