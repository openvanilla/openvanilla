
all: build

build: $(GOALS)

install:
	$(MKDIR) $(INST_PATH)
	$(CP) $(GOALS) $(INST_PATH)
	$(CP) *.cin $(INST_PATH)

clean:
	$(RM) $(GOALS) $(OBJS)


