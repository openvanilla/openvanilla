#include "CLucene/StdHeader.h"
#ifndef _lucene_search_MultiTermQuery_
#define _lucene_search_MultiTermQuery_

#include "CLucene/util/StringBuffer.h"
#include "CLucene/index/IndexReader.h"
#include "CLucene/index/Term.h"
#include "CLucene/index/Terms.h"
#include "FilteredTermEnum.h"
#include "SearchHeader.h"
#include "BooleanQuery.h"
#include "TermQuery.h"

using namespace lucene::index;
using namespace lucene::util;
namespace lucene{ namespace search{
    /**
     * A {@link Query} that matches documents containing a subset of terms provided
     * by a {@link FilteredTermEnum} enumeration.
     * <P>
     * <code>MultiTermQuery</code> is not designed to be used by itself.
     * <BR>
     * The reason being that it is not intialized with a {@link FilteredTermEnum}
     * enumeration. A {@link FilteredTermEnum} enumeration needs to be provided.
     * <P>
     * For example, {@link WildcardQuery} and {@link FuzzyQuery} extend
     * <code>MultiTermQuery</code> to provide {@link WildcardTermEnum} and
     * {@link FuzzyTermEnum}, respectively.
     */
    class MultiTermQuery: public Query {
    private:
        const Term* term;
        FilteredTermEnum* _enum;
        //const IndexReader& reader;
        BooleanQuery* query;

    protected:

        /** Enable or disable lucene style toString(field) format */
        bool LUCENE_STYLE_TOSTRING;

        /** Set the TermEnum to be used */
        void setEnum(FilteredTermEnum* _enum) ;
    public:
        /** Constructs a query for terms matching <code>term</code>. */
        MultiTermQuery(Term* t);
        /* DSR:CL_BUG_LEAK: This destructor needed to be virtual so it would
        ** get called for instances of derived classes. */
        virtual ~MultiTermQuery();


        float_t sumOfSquaredWeights(Searcher& searcher);

        void normalize(const float_t norm) ;

        Scorer* scorer(IndexReader& reader) ;

        //marked public by search highlighter
        BooleanQuery* getQuery() ;

        /** Prints a user-readable version of this query. */
        const char_t* toString(const char_t* field);
    };
}}
#endif
