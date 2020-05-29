all: GridServer GridDisplay GridClient

GridServer: mainGridserver.cpp
	g++ mainGridserver.cpp gridserver.cpp -o GridServer

GridDisplay: mainDisplay.cpp
	g++ mainDisplay.cpp -o GridDisplay

GridClient: mainClient.cpp
	g++ mainClient.cpp clientLogic.cpp -o GridClient

clean:
	rm GridServer GridDisplay GridClient
