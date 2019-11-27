src = main.c practica1.c
object = practica1
file = a.txt  b.txt  c.txt d.txt
#file = fichero.txt 
#file = normalizar.txt
$(object) : $(src)
	gcc -Wall -g $(src) -o $(object)

run: $(object)
	clear
	./$(object) $(file) 2> error

eraser:
	rm *.rep

valgrid: $(object) $(file)
	valgrind --leak-check=full ./$(object) $(file)
 