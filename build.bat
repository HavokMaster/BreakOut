g++ main.cpp -c -o build/main.o -Isrc/include
g++ build/main.o -o build/breakout -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system