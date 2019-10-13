src = main.c practica1.c
object = practica1
#file = c.txt
file = a.txt  b.txt  c.txt

$(object) : $(src)
	gcc -Wall -g $(src) -o $(object)

run: $(object)
	clear
	./$(object) $(file) 2> error

eraser:
	rm *.rep