#include "CLucene/StdHeader.h"
#ifndef _lucene_search_HitCollector_
#define _lucene_search_HitCollector_


namespace lucene{ namespace search {
    // Lower-level search API.
    // @see Searcher#search(Query,HitCollector)
    class HitCollector {
    public:
        // Called once for every non-zero scoring document, with the document number
        //and its score.
        //
        //<P>If, for example, an application wished to collect all of the hits for a
        //query in a BitSet, then it might:<pre>
        //  Searcher = new IndexSearcher(indexReader);
        //  final BitSet bits = new BitSet(indexReader.maxDoc());
        //  searcher.search(query, new HitCollector() {
        //      public void collect(int_t doc, float_t score) {
        //        bits.set(doc);
        //      }
        //    });
        //</pre>
        //
        //<p>Note: This is called in an inner search loop.  For good search
        //performance, implementations of this method should not call {@link
        //Searcher#doc(int_t)} or {@link
        //org.apache.lucene.index.IndexReader#document(int_t)} on every document
        //number encountered.  Doing so can slow searches by an order of magnitude
        //or more. 
        virtual void collect(const int_t doc, const float_t score) = 0;
		virtual ~HitCollector(){}
    };
}}
#endif
