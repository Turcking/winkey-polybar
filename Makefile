#CFLAGS += -I .
LDLIBS = $(shell pkg-config --libs x11)

ifdef KEYCODE
	CPPFLAGS += -D KEYCODE=$(KEYCODE)
endif

ifdef REGEX
	CPPFLAGS += -D REGEX_EXPRESSION=\"$(REGEX)\"
endif

winkey-polybar: winkey-polybar.o file_read.o xwindow.o
	$(CC) $(CPPFLAGS) $(LDLIBS) $^ -o $@ -Wall
	sudo chown root $@
	sudo chmod 4111 $@

winkey-polybar.o: winkey-polybar.c
	$(CC) $(CPPFLAGS) $(LDLIBS) -c $^ -Wall

file_read.o: file_read.c
	$(CC) $(CPPFLAGS) $(LDLIBS) -c $^ -Wall

xwindow.o: xwindow.c
	$(CC) $(CPPFLAGS) $(LDLIBS) -c $^ -Wall

clean:
	-$(RM) *.o
	-sudo $(RM) winkey-polybar

.PHONY: clean
