
FILES = wavelets_smart.cpp

EXES = $(addsuffix .exe, $(basename $(FILES)))

OPT = -std=c++11 


all: $(EXES)


%.exe: src/%.cpp
	$(CXX) $(OPT) -o $@ $<

clean:
	rm -f $(EXES) *~ .*~ 

cleanall: clean

