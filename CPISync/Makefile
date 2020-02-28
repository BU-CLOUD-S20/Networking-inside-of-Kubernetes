#make file for test_cpisync3node
CC=g++ -std=c++11
all: test_cpisync3node.cc
	$(CC) test_cpisync3node.cc -o 3node -lcpisync -lntl -lpthread -lgmp -lcppunit -lleveldb
clean:
	rm 3node
	rm -r node1
	rm -r node2
	rm -r node3
