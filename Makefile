PROJECT = pb_companion
SOURCES = main.c
LIBS = -linkview
CC = arm-obreey-linux-gnueabi-gcc
STRIP = arm-obreey-linux-gnueabi-strip

all: $(PROJECT).app

$(PROJECT).app: $(SOURCES)
	$(CC) $(SOURCES) $(LIBS) -o $@
	$(STRIP) $@

clean:
	rm -f $(PROJECT).app