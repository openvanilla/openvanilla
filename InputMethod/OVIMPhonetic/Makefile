OBJS=OVIMPhonetic.o vanilladata.o vanillaphone.o
GOALS=OVIMPhonetic.dylib
INST_PATH=/Library/OpenVanilla/Development

all: $(GOALS)

$(GOALS): $(OBJS)
	g++ -o $@ -bundle $(OBJS)

install:
	mkdir -p $(INST_PATH)
	cp $(GOALS) $(INST_PATH)

clean:
	rm -f $(GOALS) *.o

%.o: %.cpp
	gcc -c -O2 $<
	
