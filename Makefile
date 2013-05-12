EXEC=cpbar
LIBS=xcb pangocairo

SRCS=$(wildcard src/**/*.c src/*.c *.c)
OBJS=$(patsubst %.c,%.o,${SRCS})

CFLAGS=-std=c99 -Wall -Wextra -pedantic -s -pipe -O3
CFLAGS+=`pkg-config --cflags $(LIBS)`
LDFLAGS=`pkg-config --libs $(LIBS)`

PREFIX?= /usr/local
BINDIR?= ${PREFIX}/bin

all: ${EXEC}

${EXEC}: $(OBJS)
	${CC} ${LDFLAGS} ${OBJS} -o $@

install: all
	install -Dm 755 ${EXEC} ${DESTDIR}${BINDIR}/${EXEC}

clean:
	rm -fv ${OBJS}
