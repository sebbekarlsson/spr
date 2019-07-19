exec = a.out
sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -Wall -g


$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

libspr.a: $(objects)
	ar rcs $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o
