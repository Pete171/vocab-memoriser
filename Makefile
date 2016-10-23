OUTPUT=main

all:
	g++ -Wall -Wextra -std=c++14 -o $(OUTPUT) main.cpp && chmod 777 ./$(OUTPUT) && ./$(OUTPUT)
