compile: build run

build:
	gcc ./src/*  -o ./out/VulpineTextEditor.exe -lSDL3 -lSDL3_image -lSDL3_ttf

run:
	./out/VulpineTextEditor.exe