OUTPUT=main
CSV=csv_reader

all: csv
	g++ -Wall -Wextra -std=c++14 -o $(OUTPUT) $(CSV).o main.cpp && chmod 777 ./$(OUTPUT)

csv: csv.cpp
	g++ -Wall -Wextra -std=c++14 -c -o $(CSV).o $(CSV).cpp
