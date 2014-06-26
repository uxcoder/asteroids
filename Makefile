# Makefile for MacOS and Linux

UNAME := $(shell uname)

P=asteroids
CC=gcc
CFLAGS=-std=c99 -Wall -D_BSD_SOURCE
LDFLAGS=
SOURCES=asteroids.c llist.c
OBJECTS=asteroids.o


ifeq ($(UNAME), Darwin)
	LDFLAGS=-lm -framework GLUT -framework OPENGL -framework FOUNDATION
endif

ifeq ($(UNAME), Linux)
	LDFLAGS=-lm -lGL -lGLU -lglut
endif

all: $(SOURCES)
	$(CC) -o $(P) $(SOURCES) $(CFLAGS) $(LDFLAGS)