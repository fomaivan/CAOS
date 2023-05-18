.PHONY: all clean

NUM=$(shell sed '/^\#/d; /^$$/d; s|.*=||; s/[^0-9]//g; /^$$/d' compile.conf | tr "\n" ",")
STRING=$(shell sed '/^\#/d;/^$$/d;s|.*=||;/^[[:digit:]]/d; s/$$/\\\\n/g' compile.conf)

all: program.exe

program.exe:
	gcc -D string="\"$(STRING)\"" -D number="{$(NUM)}" -o program.exe solution.c
