
all: build ;

build: $(GOALS) ;

install: install-mkdir install-goal install-data ;

install-mkdir:
	$(MKDIR) $(INST_PATH)
	$(MKDIR) $(DATA_INST_PATH)

install-goal:
	$(CP) $(GOALS) $(INST_PATH)

install-data:
	if [ "$(DATA)" ]; then for i in $(DATA); do $(CP) $$i $(DATA_INST_PATH); done; fi

clean:
	$(RM) $(GOALS) $(OBJS)


