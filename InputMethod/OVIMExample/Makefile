OBJS=OVIMExample.o
GOALS=OVIMExample.dylib
INST_PATH=/Library/OpenVanilla/Development

all: $(GOALS)

$(GOALS): $(OBJS)
	g++ -O2 -o $@ -bundle $<

install: $(GOALS)
	mkdir -p $(INST_PATH)
	cp $(GOALS) $(INST_PATH)

clean:
	rm -f $(GOALS)

%.o: %.cpp
	gcc -O2 -c $<
	
