OVDEBUG=1

test: Textbar
	./Textbar < TextbarTests.txt

Textbar: Textbar.o VXTextBar.o VXUtility.o
	g++ -o Textbar -framework Carbon Textbar.o VXTextBar.o VXUtility.o

clean:
	rm -f Textbar *.o

VXTextBar.o: ../../Loaders/OSX/VXTextBar.cpp
	g++ -o $@ -c -I ../../Headers/OSX -I ../../Loaders/OSX -I ../.. $<

VXUtility.o: ../../Source/OSX/VXUtility.cpp
	g++ -o $@ -c -I ../../Headers/OSX -I ../.. $<

%.o: %.cpp
	g++ -c -I ../../OSX -I ../.. $<
