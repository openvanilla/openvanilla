OVDEUBG=1

test: Textbar
	./Textbar < TextbarTests.txt

Textbar: Textbar.o VXTextBar.o VXUtility.o
	g++ -o Textbar -framework Carbon Textbar.o VXTextBar.o VXUtility.o

clean:
	rm -f Textbar *.o

VXTextBar.o: ../../OSX/VXTextBar.cpp
	g++ -o $@ -c -I ../../OSX -I ../.. $<

VXUtility.o: ../../OSX/VXUtility.cpp
	g++ -o $@ -c -I ../../OSX -I ../.. $<

%.o: %.cpp
	g++ -c -I ../../OSX -I ../.. $<
