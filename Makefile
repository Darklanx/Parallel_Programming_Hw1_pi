TARGET = pi
all: $(TARGET)
$(TARGET): pi.cpp
	g++ -pthread -std=c++11 -O2 -s pi.cpp -o pi

.PHONY: clean
clean:
	rm `ls | grep "result_"`