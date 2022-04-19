OBJETOS = main.o stats.o client.o memory.o process.o proxy.o server.o synchronization.o sosignal.o sotime.o log.o configuration.o

out: $(OBJETOS)
	gcc $(addprefix obj/,$(OBJETOS)) -o bin/sovac -lrt -lm -pthread
%.o: src/%.c$($@)
	gcc -Wall -g -I include  -o obj/$@ -c $<

clean:
	rm -f bin/sovac