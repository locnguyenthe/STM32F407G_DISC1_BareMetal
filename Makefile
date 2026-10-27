CC = arm-none-eabi-gcc
CFLAGS = -mcpu=cortex-m4 -mthumb -O0
LDFLAGS = -T linkerscript.ld -nostartfiles

SRCS = main.c startup.c
OBJS = $(SRCS:.c=.o)

all: final.elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

final.elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f *.o *.elf