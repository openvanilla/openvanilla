#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
if len(sys.argv) < 4:
	sys.stderr.write("usage: combine-cin.py <base> <order reference> <output>\n")
	sys.exit(1)

base = open(sys.argv[1])
ref = open(sys.argv[2])
output = open(sys.argv[3], "w")

baseLines = base.readlines()
refLines = ref.readlines()

data = {}
exist = {}
order = []

cd = False

for line in refLines:
	if line.startswith("%chardef"):
		cd = not cd
	else:
		if not cd:
			pass
		else:
			kv = line.strip().split()
			if data.has_key(kv[0]):
				data[kv[0]] += [kv[1]]
				exist[kv[0]][kv[1]] = True
			else:
				data[kv[0]] = [kv[1]]
				exist[kv[0]] = { kv[1]: True }
				order += [kv[0]]

for line in baseLines:
	if line.startswith("%chardef"):
		if cd:
			for k in order:
				for v in data[k]:
					output.write("%s %s\n" % (k, v))
		else:
			output.write(line)

		cd = not cd

	else:
		if not cd:
			output.write(line)
		else:
			kv = line.strip().split()
			if data.has_key(kv[0]):
				if not exist[kv[0]].has_key(kv[1]):
					data[kv[0]] += [kv[1]]
					exist[kv[0]][kv[1]] = True
			else:
				data[kv[0]] = [kv[1]]
				exist[kv[0]] = { kv[1]: True }
				order += [kv[0]]

