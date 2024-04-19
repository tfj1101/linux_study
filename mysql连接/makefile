all:test1 test2
test1:test1.cpp
	g++ -o $@ $^ -std=c++11 -L/usr/lib64/mysql/ -lmysqlclient
test2:test2.cpp
	g++ -o $@ $^ -std=c++11 -L/usr/lib64/mysql/ -lmysqlclient
clean:
	rm -rf test1 test2