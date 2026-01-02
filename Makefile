CC = g++
FLAGS = -std=c++20 -lfmt -lGL -lX11 -lXrandr
INCLUDES = -I ./src -I ./3rd_party/include

app: main.o
	$(CC) $(INCLUDES) $(FLAGS) $^ -o app

main.o: ./src/main.cc
	bear -- $(CC) $(INCLUDES) $(FLAGS) $^ -c
