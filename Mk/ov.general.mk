
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

%.o: %.mm
	$(CPP) $(CFLAGS) -c -I../../Source/OSX/ -I../../Loaders/OSX $<


