OVDEUBG=1
CFLAGS=-I../../Loaders/OSX -I../../Headers/OSX

test: Textbar
	./Textbar < TextbarTests.txt

Textbar: Textbar.o VXTextBar.o VXUtility.o
	g++ -o Textbar -framework Carbon Textbar.o VXTextBar.o VXUtility.o

clean:
	rm -f Textbar *.o

VXTextBar.o: ../../Loaders/OSX/VXTextBar.cpp
	g++ -o $@ -c $(CFLAGS) $<

VXUtility.o: ../../Source/OSX/VXUtility.cpp
	g++ -o $@ -c $(CFLAGS) $<

%.o: %.cpp
	g++ -c $(CFLAGS) $<
