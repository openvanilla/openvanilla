
%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<

%.o: %.mm
	$(CPP) $(CFLAGS) -c $<


