#include "CLucene/StdHeader.h"
#ifndef _lucene_search_WildcardTermEnum_
#define _lucene_search_WildcardTermEnum_
#ifndef NO_WILDCARD_QUERY

#include "CLucene/index/IndexReader.h"
#include "CLucene/index/Term.h"
#include "CLucene/index/Terms.h"
#include "FilteredTermEnum.h"

using namespace lucene::index;
namespace lucene { namespace search {
    /**
     * Subclass of FilteredTermEnum for enumerating all terms that match the
     * specified wildcard filter term->
     * <p>
     * Term enumerations are always ordered by term->compareTo().  Each term in
     * the enumeration is greater than all that precede it.
     */
    class WildcardTermEnum: public FilteredTermEnum {
    private:
        Term* searchTerm;
        const char_t* field;
        char_t* _text;
		char_t* text; //a pointer to _text, which is modified
        char_t* pre;
        int_t preLen;
        bool fieldMatch;
        bool endEnum;

        /********************************************
        * const char_t* equality with support for wildcards
        ********************************************/

        protected:
        bool termCompare(Term* term) ;

        public:
        const static char_t WILDCARD_STRING = '*';
        const static char_t WILDCARD_CHAR = '?';

        /** Creates new WildcardTermEnum */
        WildcardTermEnum(IndexReader& reader, Term* term);
    ~WildcardTermEnum();

        float_t difference() ;

        bool EndEnum() ;

        /**
         * Determines if a word matches a wildcard pattern.
         * <small>Work released by Granta Design Ltd after originally being done on
         * company time.</small>
         */
        static bool wildcardEquals(const char_t* pattern, int_t patternIdx, const char_t* str, int_t stringIdx);

        void close();
    };
}}
#endif
#endif
