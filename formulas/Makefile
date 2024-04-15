CC = gcc
AS = as
CFLAGS = -w -std=c11 -g -march=native -O1 -mavx512f -msse4.2 -mavx2 -mavx512cd -mavx512er -mavx512pf -mavx512dq -mavx512bw -mavx512vl -mavx512vbmi -mavx512vbmi2 -mavx512vnni -mavx512bitalg -mavx512vpopcntdq -mavx5124fmaps -mavx5124vnniw -mavx512ifma -mavx512vp2intersect


SRCS = main.c formulas.h formula1.c formula2.s
OBJS = main.o formula1.o formula2.o

.PHONY: all clean run debug

all: part2.out

part2.out: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -no-pie -lm

main.o: main.c formulas.h
	$(CC) $(CFLAGS) -c -o $@ main.c

formula1.o: formula1.c formulas.h
	$(CC) $(CFLAGS) -c -o $@ formula1.c

formula2.o: formula2.s
	$(AS) -g formula2.s -o formula2.o

run: part2.out
	./part2.out

debug: part2.out
	gdb run --args ./part2.out

clean:
	rm  -f  part2.out  *.o
