.PHONY: all

Folders = ../obj/ ../build/

OPT_FLAGS = -O3 -mavx -mavx2

DEBUG_FLAGS = -march=native -D _DEBUG -ggdb3 -std=c++2a -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,leak,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

FLAGS = $(OPT_FLAGS)

all: m2

clean:
	find . -name "*.o" -delete

folders:
	mkdir -p $(Folders)

example: example.cpp
	g++ example.cpp -o example -lSDL2_ttf -lSDL2 

m1: ../obj/scalar.o ../obj/screen.o main.cpp
	g++ -DM1 ../obj/scalar.o ../obj/screen.o main.cpp -o ../build/m1 -lSDL2 $(FLAGS)

m2: ../obj/vector.o ../obj/screen.o main.cpp
	g++ -DM2 ../obj/vector.o ../obj/screen.o main.cpp -o ../build/m2 -lSDL2 $(FLAGS)

../obj/scalar.o: scalar.cpp scalar.h common.h
	g++ -c scalar.cpp -o ../obj/scalar.o  $(FLAGS)

../obj/vector.o: vector.cpp vector.h common.h
	g++ -c vector.cpp -o ../obj/vector.o  $(FLAGS)

../obj/screen.o: screen.cpp screen.h common.h
	g++ -c screen.cpp -o ../obj/screen.o $(FLAGS)