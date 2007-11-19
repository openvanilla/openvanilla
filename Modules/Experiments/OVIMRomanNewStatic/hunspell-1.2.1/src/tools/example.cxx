#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "hunspell.hxx"

extern char * mystrdup(const char * s);

using namespace std;

int 
main(int argc, char** argv)
{

    char * af;
    char * df;
    char * wtc;
    FILE* wtclst;

  /* first parse the command line options */
  /* arg1 - affix file, arg2 dictionary file, arg3 - file of words to check */

  if (argv[1]) {
       af = mystrdup(argv[1]);
  } else {
    fprintf(stderr,"correct syntax is:\n"); 
    fprintf(stderr,"example affix_file dictionary_file file_of_words_to_check\n");
    exit(1);
  }
  if (argv[2]) {
       df = mystrdup(argv[2]);
  } else {
    fprintf(stderr,"correct syntax is:\n"); 
    fprintf(stderr,"example affix_file dictionary_file file_of_words_to_check\n");
    exit(1);
  }
  if (argv[3]) {
       wtc = mystrdup(argv[3]);
  } else {
    fprintf(stderr,"correct syntax is:\n"); 
    fprintf(stderr,"example affix_file dictionary_file file_of_words_to_check\n");
    exit(1);
  }

  
  /* open the words to check list */
  wtclst = fopen(wtc,"r");
  if (!wtclst) {
    fprintf(stderr,"Error - could not open file of words to check\n");
    exit(1);
  }

   
    Hunspell * pMS= new Hunspell(af,df);
    
    int k;
    int dp;
    char buf[101];

    while(fgets(buf,100,wtclst)) {
      k = strlen(buf);
      *(buf + k - 1) = '\0';
       dp = pMS->spell(buf);
       if (dp) {
          fprintf(stdout,"\"%s\" is okay\n",buf);
          fprintf(stdout,"\n");
       } else {
          fprintf(stdout,"\"%s\" is incorrect!\n",buf);
          fprintf(stdout,"   suggestions:\n");
          char ** wlst;
          int ns = pMS->suggest(&wlst,buf);
          for (int i=0; i < ns; i++) {
            fprintf(stdout,"    ...\"%s\"\n",wlst[i]);
            free(wlst[i]);
          }
          fprintf(stdout,"\n");
          free(wlst);
       }
    }

    delete pMS;
    fclose(wtclst);
    free(wtc);
    free(df);
    free(af);

    return 0;
}

