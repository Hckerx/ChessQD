#Easy way to build project with different configuration "all:" is the default

all:
ifeq ($(OS),Windows_NT)
	clang++ -g -c src/*.cpp -I include --std=c++17 -Wall -m64 && clang++ *.o -g -o bin/debug/main -L -lmingw32 -lSDL2 -lSDL2_image -lSDL2_ttf && start bin/debug/main.exe && del *.o
else 	
	clang++ -g -c src/*.cpp -I include -std=c++17 -Wall -m64 && clang++ *.o -g -o bin/debug/main -L -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf && ./bin/debug/main && rm *.o
endif


g:
ifeq ($(OS),Windows_NT)
	g++ -g -c src/*.cpp -I include --std=c++17 -Wall -m64 && g++ *.o -g -o bin/debug/main -L -lmingw32 -lSDL2 -lSDL2_image -lSDL2_ttf && start bin/debug/main.exe && del *.o
else 	
	g++ -g -c src/*.cpp -I include -std=c++17 -Wall -m64 && g++ *.o -g -o bin/debug/main -L -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf && ./bin/debug/main && rm *.o
endif


release:
	clang++ -c src/*.cpp -std=c++17 -O3 -m64 -I include && clang++ *.o -o bin/release/main -s -L -lboost_system -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf


to_static_win:
	x86_64-w64-mingw32-g++ -O3 -c src/*.cpp -I include -std=c++17 -m64 && x86_64-w64-mingw32-g++ *.o  -o bin/debug/main -static -Bstatic -L -lSDL2_ttf -lmingw32 -lSDL2_ttf -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mwindows -lws2_32 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -lwsock32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lws2_32 -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -lcomdlg32 -lhid -lsetupapi -lrpcrt4 -lSDL2_ttf -lrpcrt4 -lsetupapi -lole32 -lgdi32 -lole32 -loleaut32 -limm32 -lwinmm -lversion -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive
to_static_lin:
	clang++ -c src/*.cpp -std=c++17 -O3 -m64 -I include && clang++ -v *.o -o bin/release/main -I/usr/local/include/SDL2 -D_REENTRANT -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -Wl,-Bstatic -lSDL2 -lSDL2main -Wl,-Bdynamic -lm -ldl -lpthread -lrt 


build:
ifeq ($(OS),Windows_NT)
	clang++ -g -c src/*.cpp -I include --std=c++17 -Wall -m64 && clang++ *.o -g -o bin/main -L -lmingw32 -lSDL2 -lSDL2_image -lSDL2_ttf && del *.o
else 	
	clang++ -g -c src/*.cpp -I include -std=c++17 -Wall -m64 && clang++ *.o -g -o bin/main -L -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf && rm *.o
endif

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
	
