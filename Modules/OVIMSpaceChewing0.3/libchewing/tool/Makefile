CC=     gcc
SRC = sort_word.c ../chewing-utf8-util.c  ../key2pho.c
SORT_WORD_SRC = sort_word.c ../chewing-utf8-util.c  ../key2pho.c
SORT_WORD_OBJ = sort_word.o chewing-utf8-util.o  key2pho.o
SORT_DIC_SRC = sort_dic.c ../chewing-utf8-util.c  ../key2pho.c
SORT_DIC_OBJ = sort_dic.o chewing-utf8-util.o  key2pho.o
MAKETREE_SRC = maketree.c ../chewing-utf8-util.c  ../key2pho.c
MAKETREE_OBJ = maketree.o chewing-utf8-util.o  key2pho.o
CFLAGS=-pipe -Wall -O3 -I../

all: sort_word sort_dic maketree

sort_word:
	$(CC) $(CFLAGS) -c $(SORT_WORD_SRC)
	$(CC) $(LDFLAGS) -o sort_word $(SORT_WORD_OBJ) $(LIBS) 

sort_dic:
	$(CC) $(CFLAGS) -c $(SORT_DIC_SRC)
	$(CC) $(LDFLAGS) -o sort_dic $(SORT_DIC_OBJ) $(LIBS) 

maketree:
	$(CC) $(CFLAGS) -c $(MAKETREE_SRC)
	$(CC) $(LDFLAGS) -o maketree $(MAKETREE_OBJ) $(LIBS) 

clean:
	rm sort_word sort_dic maketree *.o
