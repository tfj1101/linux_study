CXX = gcc
AR = $(CC)ar
EXEC =  client
OBJS = client.c
all :$(EXEC)
$(EXEC):$(OBJS)
	$(CC) -g -o $@ $(OBJS) -lm -lpthread
clean:
	rm -f $(EXEC) *.elf *.gdb *.o
