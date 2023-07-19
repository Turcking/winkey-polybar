winkey-polybar: winkey-polybar.o
	gcc $^ -o $@
	sudo chown root $@
	sudo chmod 4575 $@

winkey-polybar.o: winkey-polybar.c
	gcc -c $^ -Wall

clean:
	-rm *.o
	-sudo rm winkey-polybar
