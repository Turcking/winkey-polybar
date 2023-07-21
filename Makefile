#CFLAGS += -I .
winkey-polybar: winkey-polybar.o file_read.o
	gcc $^ -o $@
	sudo chown root $@
	sudo chmod 4575 $@

winkey-polybar.o: winkey-polybar.c
	gcc -c $^ -Wall

file_read.o: file_read.c
	gcc -c $^ -Wall

clean:
	-rm *.o
	-sudo rm winkey-polybar
