CC = g++
FLAGS = -std=c++20 -lfmt -lGL -lX11 -lXrandr
INCLUDES = -I ./src -I ./3rd_party/include

run: app
	./app

app: main.o texture.o renderer.o compute.o asset.o vec.o
	$(CC) $(INCLUDES) $(FLAGS) $^ -o app

main.o: src/main.cc
	bear -- $(CC) $(INCLUDES) $(FLAGS) $^ -c

texture.o: src/texture.cc src/texture.h
	$(CC) $(INCLUDES) $(FLAGS) $(firstword $^) -c

renderer.o: src/renderer.cc src/renderer.h
	$(CC) $(INCLUDES) $(FLAGS) $(firstword $^) -c

compute.o: src/compute.cc src/compute.h
	$(CC) $(INCLUDES) $(FLAGS) $(firstword $^) -c

asset.o: src/asset.cc src/asset.h
	$(CC) $(INCLUDES) $(FLAGS) $(firstword $^) -c

vec.o: src/vec.cc src/vec.h
	$(CC) $(INCLUDES) $(FLAGS) $(firstword $^) -c
