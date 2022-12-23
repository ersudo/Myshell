shell: myshell.o
	   gcc -g -Wall -o myshell myshell.o
		 rm myshell.o
myhsell.o:myshell.c
	   gcc -g -Wall -c myshell.c
	 
