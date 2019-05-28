all:eluosi

eluosi:eluosi.c
	gcc $^ -o $@ -g -l pthread
clean:
	rm eluosi
