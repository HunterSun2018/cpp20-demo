#
# g++ version must be greater than 10.0
#
SRC = src/main.cpp
TARGET = demo

demo : $(SRC)
	g++ $(SRC) -o $(TARGET) -std=c++20 -fcoroutines

clean : 
	rm demo
