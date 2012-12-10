CC = gcc
CFLAGS = -Wall -g
SRC = vm/block.c vm/compiler.c vm/vm.c vm/string.c vm/array.c vm/object.c vm/grammar.c vm/volang.c
OBJ = ${SRC:.c=.o}
LEG = vendor/peg/leg

all: volang

.c.o:
	@echo "   cc $<"
	@${CC} -c ${CFLAGS} -o $@ $<

volang: ${OBJ}
	@echo " link volang"
	@${CC} ${CFLAGS} ${OBJ} -o volang

vm/grammar.c: ${LEG} vm/grammar.leg
	@echo "  leg grammar.leg"
	@${LEG} -o vm/grammar.c vm/grammar.leg

clean:
	$(RM) vm/*.o vm/grammar.c
	$(RM) volang
rebuild: clean volang
