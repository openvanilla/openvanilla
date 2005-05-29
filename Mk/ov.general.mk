
%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CPP) $(CFLAGS) -c $<

%.o: %.mm
	$(CPP) $(CFLAGS) -c $<

%.lo: %.cpp
	$(LIBTOOL) --mode=compile --tag=CXX $(CPP) $(CFLAGS) -c $<

%.lo: %.c
	$(LIBTOOL) --mode=compile --tag=CC $(CC) $(CFLAGS) -c $<
