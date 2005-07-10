#include "CLucene/StdHeader.h"
#ifndef _lucene_search_SearchHeader_
#define _lucene_search_SearchHeader_

#include "CLucene/index/IndexReader.h"
#include "Scorer.h"
#include "CLucene/document/Document.h"
#include "CLucene/index/Term.h"
#include "Filter.h"
#include "CLucene/document/Document.h"
#include "TopDocs.h"
#include "CLucene/util/VoidList.h"

using namespace lucene::index;
namespace lucene{ namespace search{

	//predefine classes
	class Searcher;
	class Query;
	class Hits;

    class HitDoc {
      public:
        float_t score;
        int_t id;
        lucene::document::Document* doc;
		
        HitDoc* next;					  // in doubly-linked cache
        HitDoc* prev;					  // in doubly-linked cache
		
        HitDoc(const float_t s, const int_t i);
        ~HitDoc();
    };



    // A ranked list of documents, used to hold search results. 
    class Hits {
    private:
		Query& query;
		Searcher& searcher;
		const Filter* filter;
	    
		int_t length;				  // the total number of hits
		lucene::util::VoidList<HitDoc*> hitDocs;	  // cache of hits retrieved
	    
		HitDoc* first;				  // head of LRU cache
		HitDoc* last;				  // tail of LRU cache
		int_t numDocs;			  // number cached
		int_t maxDocs;			  // max to cache

    public:
		Hits(Searcher& s, Query& q, const Filter* f);
		~Hits();
	      
		// Returns the total number of hits available in this set. 
		int_t Length() const;
	    
		// Returns the nth document in this set.
		//	<p>Documents are cached, so that repeated requests for the same element may
		//	return the same Document object.  
		lucene::document::Document& doc(const int_t n);
	      
		int_t id (const int_t n);
	    
		// Returns the score for the nth document in this set.  
		float_t score(const int_t n);
	      
	private:
		// Tries to add new documents to hitDocs.
		// Ensures that the hit numbered <code>min</code> has been retrieved.
		void getMoreDocs(const int_t Min);
	    
		HitDoc& getHitDoc(const int_t n);
	    
		void addToFront(HitDoc& hitDoc);
	    
		void remove(HitDoc& hitDoc);
    };



	// The abstract base class for search implementations.
	// <p>Subclasses implement search over a single index, over multiple indices,
	// and over indices on remote servers.
	class Searcher {
	public:
		virtual int_t docFreq(const Term& term)const = 0;
		virtual int_t maxDoc()const = 0;
		virtual TopDocs& Search(Query& query, const Filter* filter, const int_t n) = 0;

		virtual ~Searcher(){ }
		// Returns the documents matching <code>query</code>.
		Hits& search(Query& query) {
			return search(query, NULL);
		}

		// Returns the documents matching <code>query</code> and
		//	<code>filter</code>. 
		Hits& search(Query& query, const Filter* filter) {
			return *new Hits(*this, query, filter);
		}

		// Lower-level search API.
		//
		// <p>{@link HitCollector#collect(int_t,float_t)} is called for every non-zero
		// scoring document.
		//
		// <p>Applications should only use this if they need <i>all</i> of the
		// matching documents.  The high-level search API ({@link
		// Searcher#search(Query)}) is usually more efficient, as it skips
		// non-high-scoring hits. 
		void search(Query& query, HitCollector& results) {
			Search(query, NULL, results);
		}


		// Lower-level search API.
		//
		// <p>{@link HitCollector#collect(int_t,float_t)} is called for every non-zero
		// scoring document.
		//
		// <p>Applications should only use this if they need <i>all</i> of the
		// matching documents.  The high-level search API ({@link
		// Searcher#search(Query)}) is usually more efficient, as it skips
		// non-high-scoring hits.
		//
		// @param query to match documents
		// @param filter if non-null, a bitset used to eliminate some documents
		// @param results to receive hits
		virtual void Search(Query& query, const Filter* filter, HitCollector& results) = 0;


		// For use by {@link HitCollector} implementations. 
		virtual lucene::document::Document& doc(const int_t i) = 0;

		// Frees resources associated with this Searcher. 
		virtual void close() = 0;
	};

	
	//TODO: make this class serializable
	class Query {	
	public:
		Query():boost(1.0f){
			//constructor
		}
		virtual ~Query(){
		}

		// query boost factor
		float_t boost;

		// query weighting
		virtual float_t sumOfSquaredWeights(Searcher& searcher) = 0;
		virtual void normalize(const float_t norm) = 0;

		// query evaluation
		virtual Scorer* scorer(IndexReader& reader) = 0;

		virtual void prepare(IndexReader& reader) = 0;

	    
		virtual const char_t* getQueryName() const = 0;
		
		bool instanceOf(const char_t* other){
			if ( this != NULL && stringCompare( other, getQueryName() ) ==0 )
				return true;
			else
				return false;
		}
		static Scorer* scorer( Query& query, Searcher& searcher, IndexReader& reader){
			query.prepare(reader);
			float_t sum = query.sumOfSquaredWeights(searcher);
			float_t norm = 1.0f / (float_t)sqrt(sum);
			query.normalize(norm);
			return query.scorer(reader);
		}

		// Sets the boost for this term to <code>b</code>.  Documents containing
		//	this term will (in addition to the normal weightings) have their score
		//	multiplied by <code>b</code>. 
		void setBoost(float_t b) { boost = b; }

		// Gets the boost for this term.  Documents containing
		//	this term will (in addition to the normal weightings) have their score
		//	multiplied by <code>b</code>.   The boost is 1.0 by default.  
		float_t getBoost() { return boost; }

		// Prints a query to a string, with <code>field</code> as the default field
		//	for terms.
		//	<p>The representation used is one that is readable by
		//	<a href="doc/lucene.queryParser.QueryParser.html">QueryParser</a>
		//	(although, if the query was created by the parser, the printed
		//	representation may not be exactly what was parsed). 
		virtual const char_t* toString(const char_t* field) = 0;
	};





}}
#endif
