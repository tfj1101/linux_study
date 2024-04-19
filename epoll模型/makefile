all:tcpepoll client
tcpepoll:tcpepoll.cpp InetAddress.cpp Socket.cpp Epoll.cpp
	g++ -o $@ $^ -std=c++11
client:client.cpp
	g++ -o $@ $^ -std=c++11

clean:
	rm -f tcpepoll client