
test: Textbar
	./Textbar

Textbar: Textbar.cpp ../../OSX/VXTextBar.cpp
	g++ -o Textbar -I ../../OSX -I ../.. ../../OSX/VXTextBar.cpp -framework Carbon ../../OSX/VXUtility.cpp Textbar.cpp

clean:
	rm -f Textbar

