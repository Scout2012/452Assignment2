all:	cipher 

cipher:	cipher.o DES.o AES.o
	g++ cipher.o DES.o AES.o -o cipher -lcrypto

cipher.o:	cipher.cpp
	g++ -g -c cipher.cpp 

DES.o:	DES.cpp DES.h
	g++ -g -c DES.cpp

AES.o:	AES.cpp AES.h
	g++ -g -c AES.cpp

clean:
	rm -rf *.o cipher myaes mydes
