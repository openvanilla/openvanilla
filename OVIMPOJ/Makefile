OBJS=OVIMPOJ.o
GOALS=OVIMPOJ.dylib
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
	rm $(GOALS)

%.o: %.cpp
	gcc -c $<
	
