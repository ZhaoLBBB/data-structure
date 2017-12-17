TARGET=set-test rbtree-test list-test
all:$(TARGET)
%.o:%.c
	$(CC) -c $(CFLAGS) $^ -o $@
set-test:set-test.o
	$(CC) $^ $(LDFLAGS)  -o $@
rbtree-test:rbtree-test.o
	$(CC) $^ $(LDFLAGS)  -o $@
list-test:list-test.o
	$(CC)  $^ $(LDFLAGS) -o $@
clean:
	rm -rf *.o $(TARGET)
