all:client
client:client.cpp
	g++ -o $@ $^ 