Clipped: Aleatoric.o
	g++ Aleatoric.o -o Aleatoric -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

main.o:
	g++ -c Aleatoric.cpp
