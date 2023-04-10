.PHONY: all

Folders = obj/ build/

OPT_FLAGS = -O3 -march=native -mtune=native -flto

DEBUG_FLAGS = -march=native -D _DEBUG -ggdb3 -std=c++2a -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

FLAGS = $(DEBUG_FLAGS)

all: m1

clean:
	find . -name "*.o" -delete

folders:
	mkdir -p $(Folders)

example: example.cpp
	g++ example.cpp -o example -lSDL2 

m1: obj/mandelbrot1.o obj/screen.o obj/main.o
	g++ obj/mandelbrot1.o obj/screen.o obj/main.o -o build/m1 -lSDL2 $(FLAGS)

m2: obj/mandelbrot2.o obj/screen.o obj/main.o
	g++ obj/mandelbrot2.o obj/screen.o obj/main.o -o build/m2 -lSDL2 $(FLAGS)

obj/mandelbrot1.o: mandelbrot1.cpp mandelbrot1.h common.h
	g++ -c mandelbrot1.cpp -o obj/mandelbrot1.o  $(FLAGS)

obj/mandelbrot2.o: mandelbrot2.cpp mandelbrot2.h common.h
	g++ -c mandelbrot2.cpp -o obj/mandelbrot2.o  $(FLAGS)

obj/main.o: main.cpp mandelbrot1.h screen.h common.h
	g++ -c main.cpp -o obj/main.o  $(FLAGS)

obj/screen.o: screen.cpp screen.h common.h
	g++ -c screen.cpp -o obj/screen.o $(FLAGS)