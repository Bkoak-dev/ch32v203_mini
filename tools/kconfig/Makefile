CFLAGS  := -I/usr/include/ncurses -DCURSES_LOC="<ncurses.h>" -DLOCALE -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer
LDFLAGS := -lncurses

cc := gcc

BASE_OBJS := lxdialog/menubox.o lxdialog/yesno.o lxdialog/textbox.o  \
	lxdialog/inputbox.o lxdialog/util.o lxdialog/checklist.o zconf.tab.o

all : mconf conf

%.o : %.c
	gcc -c $(CFLAGS) $(LDFLAGS) $< -o $@

zconf.tab.o :
	cp zconf.hash.c_shipped zconf.hash.c
	cp zconf.lex.c_shipped zconf.lex.c
	cp zconf.tab.c_shipped zconf.tab.c
	gcc -c $(CFLAGS) $(LDFLAGS) zconf.tab.c -o $@

mconf : $(BASE_OBJS) mconf.o
	gcc -o $@ $^ $(LDFLAGS)

conf : $(BASE_OBJS) conf.o
	gcc -o $@ $^ $(LDFLAGS)

clean :
	find -name "*.o" -exec rm "{}" \;
	-rm -rf conf mconf zconf.*.c
