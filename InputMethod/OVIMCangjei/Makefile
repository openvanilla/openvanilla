OBJS=OVIMCangjei.o vanillacj.o vanillasymbol.o vcdata.o vcdata_simplex.o
GOALS=OVIMCangjei.dylib
INST_PATH=/Library/OpenVanilla/Development
CONF_PATH=$(INST_PATH)/OVXSimpleLoader.config

all: $(GOALS)

$(GOALS): $(OBJS)
	g++ -o $@ -bundle $(OBJS)

install:
	mkdir -p $(INST_PATH)
	cp $(GOALS) $(INST_PATH)
	echo "# This loads $(GOALS)" >  $(CONF_PATH)
	echo $(INST_PATH)/$(GOALS)   >> $(CONF_PATH)

clean:
	rm -f $(GOALS) *.o

%.o: %.cpp
	gcc -c -O2 $<
	
