#include "MinimalLoader.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <string>

using namespace std;

void set_keypress(void);
void reset_keypress(void);
void echo_off(void);
void echo_on(void);

int main() {
	echo_off();
	set_keypress();
	
	fprintf(stderr, "minimal BPMF engine tester\n");
	fprintf(stderr, "type BPMF with standard layout, space to compose\n");
	
	MLInitialize();
	
	int c;
	while(c = fgetc(stdin)) {
		
		if (isprint(c)) {
			fprintf(stderr, "key received, code = %d, char = %c\n", c, c);
		}
		else {
			fprintf(stderr, "key received, code = %d\n", c);			
		}
		
		if (c == 27) {
			fprintf(stderr, "ESC pressed, canceled\n");
			MLCancel();
			MLStart();
		}
		else {
			if (MLKey(c)) {
				if (!MLIsCommited()) {
					cerr << "[CURRENTLY COMPOSING] " << MLComposingBuffer() << "\n";
				}
				else {
					cerr << "[OUTPUT STRING] " << MLCommitedString() << "\n";
					MLCancel();
					MLStart();
				}
			}
		}
		
		if (MLCandidatesString().length() > 0) {
			cerr << "[CANDIDATES] " << MLCandidatesString() << "\n";
		}
	}
	
	reset_keypress();
	echo_on();
	return 0;
}

     
static struct termios stored_settings;

void set_keypress(void)
{
    struct termios new_settings;

    tcgetattr(0,&stored_settings);

    new_settings = stored_settings;

    /* Disable canonical mode, and set buffer size to 1 byte */
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(0,TCSANOW,&new_settings);
    return;
}

void reset_keypress(void)
{
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}

void echo_off(void)
{
    struct termios new_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ECHO);
    tcsetattr(0,TCSANOW,&new_settings);
    return;
}

void echo_on(void)
{
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}