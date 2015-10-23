
TARGET=base62-test

HFILES=
HFILES+=$(TARGET).h

CFILES=
CFILES+=$(TARGET).c

$(TARGET): $(HFILES) $(CFILES)
	gcc -std=c99 -o $(TARGET) $(CFILES) -I.

clean:
	rm -f $(TARGET)
