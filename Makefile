compile: build run

build:
	gcc ./src/*  -o ./out/Vulp.exe -lSDL3 -lSDL3_image -lSDL3_ttf

run:
	./out/Vulp.exe