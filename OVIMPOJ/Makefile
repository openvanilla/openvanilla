OBJS=OVIMPOJ.o VXCIN.o VXUtility.o
GOALS=OVIMPOJ.dylib 
INST_PATH=/Library/OpenVanilla/Development
CONF_PATH=$(INST_PATH)/OVXSimpleLoader.config

all: $(GOALS)

$(GOALS): $(OBJS)
	g++ -framework Cocoa -o $@ -bundle $(OBJS)

install:
	mkdir -p $(INST_PATH)
	cp $(GOALS) $(INST_PATH)
    cp poj.cin $(INST_PAT)
	echo "# This loads $(GOALS)" >  $(CONF_PATH)
	echo $(INST_PATH)/$(GOALS)   >> $(CONF_PATH)

VXCIN.o: ../OSX/VXCIN.mm
	gcc -o VXCIN.o -c $<

VXUtility.o: ../OSX/VXUtility.cpp
	gcc -o VXUtility.o -c $<

clean:
	rm $(GOALS)

%.o: %.cpp
	gcc -c $<

%.o: %.mm
	gcc -c $<
