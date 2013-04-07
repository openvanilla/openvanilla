#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

def comp(a,b):
	if b[1] < a[1]:
		return -1
	if b[1] > a[1]:
		return 1
	return 0

if __name__=='__main__':
    if len(sys.argv) != 3:
    	sys.exit('usage: assocphrase.py input output')
    	
    try:
        handle = open(sys.argv[1], "r")
        output = open(sys.argv[2], "w")
    except IOError as e:
        print(e)
        sys.exit(1)

    phrases = {}

    output.write('''%gen_inp
%cname 聯想詞
%ename Associated Phrases
%keyname begin
%keyname end
%chardef begin
''')

    while True:
        line = handle.readline()
        if not line:
    		break
        if line[0] == '#':
        	continue

        elements = line.rstrip().split()
        k = elements[0].decode('utf-8')

        if len(k) == 1:
        	continue

        v = int(elements[1])
        head = k[0]
        tail = k[1:]

        if head in phrases:
        	phrases[head].append((tail, v))
        else:
        	phrases[head] = [(tail, v)]

    handle.close()


    for k in phrases.keys():
        # Select at most 20 entries, as more is meaningless.
        phrases[k] = sorted(phrases[k], comp)[:20]

    	for v in phrases[k]:
    		output.write("%s %s\n" % (k.encode('utf-8'), v[0].encode('utf-8')))


    output.write("%chardef end\n")
    output.close()
