src = main.c practica3.c
object = main
flags = -g -Wall
#files = a.txt b.txt
files = ejemplo.txt
$(object): $(src)
	gcc $(flags) $(src) -o $(object)
run:
	clear
	./$(object) $(files)

rm:
	rm *.rep