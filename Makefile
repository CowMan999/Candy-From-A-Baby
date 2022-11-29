CXX= ccache g++
CC= gcc
CXXFLAGS= -std=c++20 -Wall -Wextra -Werror -O3 -s -DSFML_STATIC -Wpedantic -DTAGLIB_STATIC -DNDEBUG -mwindows
CFLAGS= -std=c99 -Wall -Wextra -Werror -s -Ofast -mwindows
CXXLIBS= -lsfcollider -limgui-SFML -limgui -lsfml-graphics-s -lsfml-window-s -lopengl32 -ljpeg -lfreetype -lgdi32 -lsfml-audio-s -lopenal32 -lFLAC -lvorbisenc -lvorbisfile -lvorbis -logg -lsfml-network-s -lws2_32 -lsfml-system-s -lwinmm 
CXXLIBPATHS= -L"C:\Users\famma\Documents\My_Programs\SFML\SFML-2.6.x\extlibs\libs-mingw\x64" -L"C:\Users\famma\Documents\My_Programs\SFML\SFML-2.6.x\staticbuild\lib" -L"C:\Users\famma\Documents\My_Programs\Dear_ImGUI\lib" -L"C:\Users\famma\Documents\PATH"

SRC = $(wildcard *.cpp)
SRH = $(wildcard *.hpp)
OBJ = $(patsubst %.cpp, make/%.o, $(SRC))

FINAL= candy.exe

$(FINAL): $(OBJ)
	@echo "Linking $@"
	@$(CXX) $(CXXFLAGS) $(CXXLIBPATHS) $(OBJ) $(CXXLIBS) -o $(FINAL)

make/%.o: %.cpp $(SRC) Makefile $(SRH)
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean: 
	@del .\make\*.o
