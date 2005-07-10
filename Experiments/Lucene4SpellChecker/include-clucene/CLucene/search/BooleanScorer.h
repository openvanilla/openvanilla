#include "CLucene/StdHeader.h"
#ifndef _lucene_search_BooleanScorer_
#define _lucene_search_BooleanScorer_

#include "HitCollector.h"
#include "Scorer.h"

namespace lucene{ namespace search{
	
	//predefine for use in BooleanScorer class LUCENE_EXPORT.
	class BucketTable;

	class Bucket {
	  public:
		int_t	doc;				  // tells if bucket is valid
		float_t	score;				  // incremental score
		int_t	bits;					  // used for bool constraints
		int_t	coord;					  // count of terms in score
		Bucket*	next;				  // next valid bucket

		Bucket();
		~Bucket();
	};

	class SubScorer {
	  public:
		Scorer* scorer;
		bool required;
		bool prohibited;
		HitCollector* collector;
		SubScorer* next;
		SubScorer(Scorer* scr, const bool r, const bool p, HitCollector* c, SubScorer* nxt);
		~SubScorer();
	};

	class BooleanScorer: public Scorer {
	private:
		int_t currentDoc;

		SubScorer* scorers;
		BucketTable* bucketTable;

		int_t maxCoord;
		int_t nextMask;

	public:
		int_t requiredMask;
		int_t prohibitedMask;
		float_t* coordFactors;


		BooleanScorer();
		~BooleanScorer();
		void add(Scorer& scorer, const bool required, const bool prohibited);

	private:
		void computeCoordFactors();
		void score(HitCollector& results, const int_t maxDoc);
	};





	
	class BucketTable {		
	private:
		BooleanScorer& scorer;
	public:
		const static int_t SIZE = 1 << 10;
		const static int_t MASK = SIZE - 1;
		Bucket buckets[SIZE];

		BucketTable(BooleanScorer& scr);
		void collectHits(HitCollector& results);
		int_t size() const;
		HitCollector* newCollector(const int_t mask);

		Bucket* first;			  // head of valid list

		void clear();
		~BucketTable(){
			clear();
		}
	};



	class Collector: public virtual HitCollector {
	private:
		BucketTable& bucketTable;
		int_t mask;
	public:
		Collector(const int_t mask, BucketTable& bucketTable);
		
		void collect(const int_t doc, const float_t score);
	};


}}
#endif
