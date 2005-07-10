#include "CLucene/StdHeader.h"

#include "SearchHeader.h"
#include "CLucene/document/Document.h"
#include "CLucene/index/Term.h"
#include "CLucene/util/PriorityQueue.h"
#include "HitQueue.h"

using namespace lucene::index;
using namespace lucene::util;
using namespace lucene::document;
namespace lucene{ namespace search{
    
    class MultiHitCollector: public HitCollector{
    private:
      HitCollector* results;
      int_t start;
    public: 
      MultiHitCollector(HitCollector* _results, int_t _start);
      void collect(const int_t doc, const float_t score) ;
    };
    
    
    /** Implements search over a set of <code>Searchers</code>. */
    class MultiSearcher: public Searcher {
    private:
        Searcher** searchers;
        int_t searchersLen;
        int_t* starts;
        int_t _maxDoc;
    
    public:
      /** Creates a searcher which searches <i>searchers</i>. */
      MultiSearcher(Searcher** _searchers, int_t _searchersLen);
      ~MultiSearcher();
    
      /** Frees resources associated with this <code>Searcher</code>. */
      void close() ;
    
      int_t docFreq(const Term& term) const ;
    
      /** For use by {@link HitCollector} implementations. */
      Document& doc(const int_t n) ;
    
      /** For use by {@link HitCollector} implementations to identify the
       * index of the sub-searcher that a particular hit came from. */
      int_t searcherIndex(int_t n) ;
    
      int_t maxDoc() const;
    
      TopDocs& Search(Query& query, const Filter* filter, const int_t nDocs) ;
    
    
      /** Lower-level search API.
       *
       * <p>{@link HitCollector#collect(int_t,float_t)} is called for every non-zero
       * scoring document.
       *
       * <p>Applications should only use this if they need <i>all</i> of the
       * matching documents.  The high-level search API ({@link
       * Searcher#search(Query)}) is usually more efficient, as it skips
       * non-high-scoring hits.
       *
       * @param query to match documents
       * @param filter if non-null, a bitset used to eliminate some documents
       * @param results to receive hits
       */
      void Search(Query& query, const Filter* filter, HitCollector& results);
    };

}}
