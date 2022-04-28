LEDALIB = '/usr/local/LEDA'
LEDAINCL = '/usr/local/LEDA/incl'

f = bipartite.cpp

compile: $(f)

	g++ $(f) -o bipart -O3 -std=c++0x -fno-strict-aliasing -I $(LEDAINCL) -lleda -L$(LEDALIB)

clean:
	rm -f bipart

run:
	./bipart
