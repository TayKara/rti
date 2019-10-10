.SILENT:
GPP = g++ -Wall -m64 -pthread -fpermissive -std=c++11
Class = FileReader.o Socket.o SocketServeur.o SocketClient.o

All: Serveur_Terminaux Applic_Terminaux 
	
# ------------------------ CLIENT - SERVER ------------------------
	
Serveur_Terminaux: $(Class) Serveur_Terminaux.cpp 
	echo Creation du Serveur_Terminaux
	$(GPP) Serveur_Terminaux.cpp -o Serveur_Terminaux $(Class)

Applic_Terminaux: $(Class) Applic_Terminaux.cpp
	echo Creation Applic_Terminaux
	$(GPP) Applic_Terminaux.cpp -o Applic_Terminaux $(Class)

# ------------------------ SOCKET ------------------------

FileReader.o: FileReader.cpp FileReader.h
	echo Creation FileReader
	$(GPP) -c FileReader.cpp -o FileReader.o

Socket.o: Socket.cpp Socket.h
	echo Creation Socket
	$(GPP) -c Socket.cpp -o Socket.o

SocketServeur.o: SocketServeur.cpp SocketServeur.h
	echo Creation SocketServeur
	$(GPP) -c SocketServeur.cpp FileReader.o -o SocketServeur.o

SocketClient.o: SocketClient.cpp SocketClient.h 
	echo Creation SocketClient
	$(GPP) -c SocketClient.cpp -o SocketClient.o

# ------------------------ OTHER ------------------------
clean: 
	rm Serveur_Terminaux Applic_Terminaux $(Class)