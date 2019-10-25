TARGET=list-test rbtree-test  set-test vector-test radix-tree-test fifo_test fifo_r_test
all:$(TARGET)
%.o:%.c
	$(CC) -c $(CFLAGS) $^ -o $@
rbtree-test:rbtree-test.o
	$(CC) $^ $(LDFLAGS)  -o $@
list-test:list-test.o
	$(CC)  $^ $(LDFLAGS) -o $@
set-test:set-test.o
	$(CC) $^ $(LDFLAGS)  -o $@
vector-test:vector-test.o
	$(CC)  $^ $(LDFLAGS) -o $@
radix-tree-test:radix-tree-test.o
	$(CC)  $^ $(LDFLAGS) -o $@
fifo_test:inttype-example.o
	$(CC) $^ $(LDFLAGS) -o $@
fifo_r_test:record-example.o
	$(CC) $^ $(LDFLAGS) -o $@
clean:
	rm -rf *.o $(TARGET)
