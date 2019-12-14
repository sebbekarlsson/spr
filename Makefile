exec = a.out
sources = $(filter-out src/main.c, $(wildcard src/*.c))
objects = $(sources:.c=.o)
flags = -Wall -g


#$(exec): $(objects)
#	gcc $(objects) $(flags) -o $(exec)

libspr.a: $(objects)
	ar rcs $@ $^

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@


install:
	make libspr.a
	mkdir -p /usr/local/include/spr
	cp -r ./src/include/* /usr/local/include/spr/.
	cp ./libspr.a /usr/local/lib/.

clean:
	-rm *.a
	-rm *.out
	-rm *.o
	-rm src/*.o
