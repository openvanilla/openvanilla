#include "OVIME.h"
#include <stdio.h>

HFONT hUIFont;
HINSTANCE hInst;
HGLOBAL hUIExtra;

void
DebugLog(char *format, void *p) {
	static char first_time=1;
	FILE *fp;

	if (first_time) {
		first_time=0;
		if (fp=fopen("f:\\orz.txt", "w")) {
			fclose(fp);
		}
	}

	if (fp=fopen("f:\\orz.txt", "a")) {
		fprintf(fp, format, p);
		fclose(fp);
	}
}

void
DebugLogW(const LPTSTR format, void *p) {
	static char first_time=1;
	FILE *fp;

	if (first_time) {
		first_time=0;
		if (fp=fopen("f:\\orz.txt", "w")) {
			fclose(fp);
		}
	}

	if (fp=fopen("f:\\orz.txt", "a")) {
		_ftprintf(fp, format, p);
		fclose(fp);
	}
}