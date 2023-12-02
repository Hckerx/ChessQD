#Easy way to build project with different configuration "all:" is the default

all:
ifeq ($(OS),Windows_NT)
	clang++ -g -c src/*.cpp -I include --std=c++17 -Wall -m64 && clang++ *.o -g -o bin/debug/main -L -lmingw32 -lSDL2 -lSDL2_image -lSDL2_ttf && start bin/debug/main.exe && del *.o
else 	
	clang++ -g -c src/*.cpp -I include -std=c++17 -Wall -m64 && clang++ *.o -g -o bin/debug/main -L -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf && ./bin/debug/main && rm *.o
endif

release:
	clang++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include && g++ *.o -o bin/release/main -s -L -lboost_system -lSDL2main -lSDL2 -lSDL2_image &&  ./bin/release/main

to_exe:
	x86_64-w64-mingw32-g++ -g -c src/*.cpp -I include -std=c++17 -Wall -m64 && x86_64-w64-mingw32-g++ *.o -g -o bin/debug/main -L -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lws2_32 && ./bin/debug/main && rm *.o

build:
	clang++ -g -c src/*.cpp -I include -std=c++17 -Wall -m64 && clang++ *.o -g -o bin/debug/main -L -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf && rm *.o

clean:
ifeq ($(OS),Windows_NT)
	del *.o	
else 
	rm *.o
endif

test:
ifeq ($(OS),Windows_NT)
	.\\bin\\debug\\main.exe
else 
	./bin/debug/main
endif
	

