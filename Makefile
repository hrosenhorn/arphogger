OBJS=main.o raw.o 
BIN=hogger
CC=gcc
LDFLAGS=

$(BIN): $(OBJS)
	$(CC) -g $(LDFLAGS) $(OBJS) -o $(BIN)

%.o:	%.c
	$(CC) -c $< -o $(@) -g

clean:
	rm $(OBJS); make
