//@author b6s
//@warning This file contains UTF-8 Chinese comments.

#include <string>
#include <vector>

string KEY_B =	"\xe3\x84\x85";		//??
string KEY_I =	"\xe3\x84\x9e";		//??
string KEY_O =	"\xe3\x84\xa1";		//??
string KEY_P =	"\xe3\x84\x86";		//??
string KEY_T =	"\xe3\x84\x8a";		//??
string KEY_U =	"\xe3\x84\xa9";		//??
string KEY_W =	"\xe3\x84\xa0";		//??
string KEY_X =	"\xe3\x84\xa8";		//??
string KEY_Y =	"\xe3\x84\x9a";		//??
string KEY_Z =	"\xe3\x84\x97";		//??

string HSU_A_01	= "\xe3\x84\x9f";	//??
string HSU_A_02	= "\xe3\x84\x98";	//??
string HSU_C_03	= "\xe3\x84\x95";	//??
string HSU_C_04	= "\xe3\x84\x92";	//??
string HSU_D_05	= "\xcb\x8a";		//?
string HSU_D_06	= "\xe3\x84\x89";	//??
string HSU_E_07	= "\xe3\x84\xa7";	//??
string HSU_E_08	= "\xe3\x84\x9d";	//??
string HSU_E_09	= "\xe3\x84\xa7";	//??
string HSU_F_10	= "\xcb\x87";		//?
string HSU_F_11	= "\xe3\x84\x88";	//??
string HSU_G_12	= "\xe3\x84\x9c";	//??
string HSU_G_13	= "\xe3\x84\x8d";	//??
string HSU_H_14	= "\xe3\x84\x9b";	//??
string HSU_H_15	= "\xe3\x84\x8f";	//??
string HSU_J_16	= "\xcb\x8b";		//?
string HSU_J_17	= "\xe3\x84\x93";	//??
string HSU_J_18	= "\xe3\x84\x90";	//??
string HSU_K_19	= "\xe3\x84\xa4";	//??
string HSU_K_20	= "\xe3\x84\x8e";	//??
string HSU_L_21	= "\xe3\x84\xa5";	//??
string HSU_L_22	= "\xe3\x84\xa6";	//??
string HSU_L_23	= "\xe3\x84\x8c";	//??
string HSU_M_24	= "\xe3\x84\xa2";	//??
string HSU_M_25	= "\xe3\x84\x87";	//??
string HSU_N_26	= "\xe3\x84\xa3";	//??
string HSU_N_27	= "\xe3\x84\x8b";	//??
string HSU_R_28	= "\xe3\x84\x9a";	//??
string HSU_R_29	= "\xe3\x84\x96";	//??
string HSU_S_30	= "\xcb\x99";		//?
string HSU_S_31	= "\xe3\x84\x99";	//??
string HSU_V_32	= "\xe3\x84\x94";	//??
string HSU_V_33	= "\xe3\x84\x91";	//??
string HSU_G_34	= "\xe3\x84\x8d";	//??
string HSU_E_35	= "\xe3\x84\xa7";	//??
string HSU_U_36	= "\xe3\x84\xa9";	//??
string HSU_J_37	= "\xe3\x84\x90";	//??

const static char tones[] ="dfjs";

bool hsuKeyToBpmf (const char* keyStr, string& bpmfStr) {
	const static unsigned int
		isUnique[] = {
			0, 1, 0, 0, 0, 0, 0,
			0, 1, 0, 0, 0, 0, 0,
			1, 1, 0, 0, 0, 1,
			1, 0, 1, 1, 1, 1};

	vector<string> bpmfVec;
	size_t length = bpmfStr.length();
	if(length >= 2)
		bpmfVec.push_back(" ");
	if(length >= 4)
		bpmfVec.push_back(" ");
	if(length >= 7)
		bpmfVec.push_back(" ");
	if(length >= 10)
		bpmfVec.push_back(" ");
	if(length > 12)
		return false;

	for (size_t i = 0; i < strlen(keyStr); i++) {
		char key = keyStr[i], keyNext;
		if(i < strlen(keyStr) - 1) keyNext = keyStr[i+1];
		else keyNext = 0;
		//if ( (pb[phi] =one2one[ch -'a']) == 0 )
		if (!isUnique[key - 'a']) {
			bool isNotIyNext = (keyNext != 'e' && keyNext != 'u');
			bool isToneNext = false;			
			for (int j = 0; j < 4; j++) {
				if (tones[j] == keyNext || !keyNext) {
					isToneNext = true;
					break;
				}
			}

			switch (key) {
				case 'a':
					bpmfVec[i] = (i > 0)? HSU_A_01: HSU_A_02;
					break;
				case 'c':
					bpmfVec[i] = isNotIyNext? HSU_C_03: HSU_C_04;
					break;
				case 'd':
					bpmfVec[i] = (i > 0)? HSU_D_05: HSU_D_06;
					break;
				case 'e':
					bpmfVec[i] =
						(i == 0)? HSU_E_07:
							((key = keyStr[i-1], key == 'e' || key == 'u')? HSU_E_08: HSU_E_09);
					break;
				case 'f':
					bpmfVec[i] = (i > 0)? HSU_F_10: HSU_F_11;
					break;
				case 'g':
					bpmfVec[i] = (i > 0 || isToneNext)? HSU_G_12: HSU_G_13;
					break;
				case 'h':
					bpmfVec[i] = (i > 0 || isToneNext)? HSU_H_14: HSU_H_15;
					break;
				case 'j':
					bpmfVec[i] =
						(i > 0)? HSU_J_16:
							(isNotIyNext? HSU_J_17: HSU_J_18);
					break;
				case 'k':
					bpmfVec[i] = (i > 0 || isToneNext)? HSU_K_19: HSU_K_20;
					break;
				case 'l':
					bpmfVec[i] =
						(i > 0)? HSU_L_21:
							(isToneNext? HSU_L_22: HSU_L_23);
					break;
				case 'm':
					bpmfVec[i] = (i > 0 || isToneNext || !keyNext)? HSU_M_24: HSU_M_25;
					break;
				case 'n':
					bpmfVec[i] = (i > 0 || isToneNext)? HSU_N_26: HSU_N_27;
					break;
				case 'r':
					bpmfVec[i] = (i > 0)? HSU_R_28: HSU_R_29;
					break;
				case 's':
					bpmfVec[i] = (i > 0)? HSU_S_30: HSU_S_31;
					break;
				case 'v':
					bpmfVec[i] = isNotIyNext? HSU_V_32: HSU_V_33;
					break;
				default:
                    return false;
			} //end of switch()
		} //end of if()
		else {
			switch (key) {
				case 'b':
					bpmfVec[i] = KEY_B;
					break;
				case 'i':
					bpmfVec[i] = KEY_I;
					break;
				case 'o':
					bpmfVec[i] = KEY_O;
					break;
				case 'p':
					bpmfVec[i] = KEY_P;
					break;
				case 't':
					bpmfVec[i] = KEY_T;
					break;
				case 'u':
					bpmfVec[i] = KEY_U;
					break;
				case 'w':
					bpmfVec[i] = KEY_W;
					break;
				case 'x':
					bpmfVec[i] = KEY_X;
					break;
				case 'y':
					bpmfVec[i] = KEY_Y;
					break;
				case 'z':
					bpmfVec[i] = KEY_Z;
					break;
				default:
					return false;
			}
		}
	} //end of for()


	if (bpmfVec.size() > 1 && bpmfVec[0].compare(HSU_G_34) == 0 &&
		(bpmfVec[1].compare(HSU_E_35) == 0 ||
		bpmfVec[1].compare(HSU_U_36) == 0))
          bpmfVec[0] = HSU_J_37;

	bpmfStr.clear();
	for(size_t idx = 0; idx < bpmfVec.size(); idx++)
		bpmfStr = bpmfStr.append(bpmfVec[idx]);

	return true;
}

/*
                     è¨±æ??µç›¤?ç½®??
   
 ?Œâ??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€??
 ??   ?? ? â?  ?§â?  ?–â?  ?Šâ?  ?šâ?  ?©â?  ?žâ?  ?¡â?  ?†â?
 ?‚Q   ?‚W   ?‚E ?â?R   ?‚T   ?‚Y   ?‚U   ?‚I   ?‚O   ?‚P   ??
 ?”â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€??
   ?Œâ??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€??
   ?? ?˜â?  ?™â?  ?‰â?  ?ˆâ?  ?â?  ?â???â?  ?Žâ??¦ã???
   ?‚A ?Ÿâ?S ??‚D ??‚F ??‚G ?œâ?H ?›â?J ?“â?K ?¤â?L ?¥â?
   ?”â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€?? 
     ?Œâ??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?¬â??€?? 
     ?? ?—â?  ?¨â?  ?’â?  ?‘â?  ?…â?  ?‹â?  ?‡â?  
     ?‚Z   ?‚X   ?‚C ?•â?V ?”â?B   ?‚N ???M ?¢â?  
     ?”â??€?´â??€?´â??€?´â??€?´â??€?´â??€?´â??€??           
     ?Œâ??€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€??           
     ??             ä¸€    ??                 ??           
     ?”â??€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€??

?Œâ??¬â??€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€??
?‚å???    ?…ã??‡ã??‰ã??‹ã??ã??ã??’ã??—ã??ã??Ÿã„¡??„§     ??
?‚éŸ³??    ï¼¢ï¼°ï¼­ï¼¦ï¼¤ï¼´ï¼®ï¼¬ï¼§ï¼«ï¼¨ï¼ªï¼?¼²ï¼ºï¼³ï¼¥ï¼©ï¼¡ï¼¯ï¼®ï¼¥     ??
?œâ??¼â??€?€?€?€?€?€?€?€?¬â??¬â??€?€?€?¬â??¬â??€?€?€?€?€?¬â???
?‚å??‚ã??šã??¢ã„¤?¥ã„¦?¨ã„©?‚å??‚ã??”ã??˜â??‹â?????œã???‚æ???
?‚å½¢?‚ï¼¶ï¼¹ï¼·ï¼­ï¼«ï¼¬ï¼¬ï¼¸ï¼µ?‚ç¨±?‚ï¼ªï¼¶ï¼£ï¼¡â??†â?ï¼³ï¼¤ï¼¦ï¼§ï¼¨ï¼ª?‚é???
?”â??´â??€?€?€?€?€?€?€?€?´â??´â??€?€?€?´â??´â??€?€?€?€?€?´â???

*/