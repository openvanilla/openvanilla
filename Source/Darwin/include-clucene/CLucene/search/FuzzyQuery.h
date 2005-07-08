#include "CLucene/StdHeader.h"
#ifndef _lucene_search_FuzzyQuery_
#define _lucene_search_FuzzyQuery_

#include "CLucene/index/IndexReader.h"
#include "CLucene/index/Term.h"
#include "MultiTermQuery.h"

#ifndef NO_FUZZY_QUERY


namespace lucene{ namespace search{
  //FuzzyTermEnum is a subclass of FilteredTermEnum for enumerating all 
  //terms that are similiar to the specified filter term.
  //
  //Term enumerations are always ordered by Term.compareTo().  Each term in
  //the enumeration is greater than all that precede it.

  class FuzzyTermEnum: public FilteredTermEnum {
    public:
	  //Constructor
      FuzzyTermEnum(IndexReader& reader, Term* term);
	  //Destructor
      ~FuzzyTermEnum();
      //Returns the fact if the current term in the enumeration has reached the end
      bool EndEnum();
      //Close the enumeration
      void close();
      //Sets the fuzzy threshold and calculates the scale factor
      void setFuzzyThreshold(float_t value);

    protected:
      //Compares term with the searchTerm using the Levenshtein distance.
      bool termCompare(Term* term) ;

	  //Returns the difference between the distance and the fuzzy threshold
      //multiplied by the scale factor
      float_t difference();

    private:
      double distance;
      bool fieldMatch;
      bool endEnum;

      Term* searchTerm; 

      float_t FUZZY_THRESHOLD;
      float_t SCALE_FACTOR;

      //Methods for computing Levenshtein distance
    
      //Finds and returns the smallest of three integers
      static int_t Min(const int_t a, const int_t b, const int_t c);

      //This array saves us from the time required to create a new array
      //everytime editDistance is called.
      int_t* e;
      int_t eWidth;
      int_t eHeight;

      //Calculates the Levenshtein distance also known as edit distance is a measure of similiarity
      //between two strings where the distance is measured as the number of character
      //deletions, insertions or substitutions required to transform one string to
      //the other string.
      int_t editDistance(const char_t* s, const char_t* t, const int_t n, const int_t m) ;
  };

  // class FuzzyQuery implements the fuzzy search query
  class FuzzyQuery: public MultiTermQuery {
    private:
      Term* fuzzyTerm;

   public:
     //Constructor
     FuzzyQuery(Term* term);
	 //Destructor
     ~FuzzyQuery();

	 //Prepares a fuzzy query
     void prepare(IndexReader& reader);

	 //Prepares a fuzzy query
     const char_t* toString(const char_t* field);

	 //Returns the query string
     const char_t* getQueryName() const;
  };
}}
#endif
#endif
