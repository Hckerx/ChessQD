all:
ifeq ($(OS),Windows_NT)
	clang++ -c -g src/*.cpp -I include --std=c++17 -g -Wall -m64 && clang++ *.o -o bin/debug/main -L -lmingw32 -lSDL2 -lSDL2_image -lSDL2_ttf && start bin/debug/main.exe && del *.o
else 	
	clang++ -g -c src/*.cpp -std=c++17 -g -Wall -m64 -I include && clang++ *.o -g -o bin/debug/main -L -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf && ./bin/debug/main && rm *.o
endif

release:
	clang++ -c src/*.cpp -std=c++14 -O3 -Wall -m64 -I include && g++ *.o -o bin/release/main -s -L -lSDL2main -lSDL2 -lSDL2_image &&  ./bin/release/main

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
	

