#include "CLucene/StdHeader.h"
#ifndef _lucene_search_BooleanClause_
#define _lucene_search_BooleanClause_

#include "SearchHeader.h"
namespace lucene{ namespace search {
	// A clause in a BooleanQuery. 
	class BooleanClause {
	public:
		bool deleteQuery;

		// The query whose matching documents are combined by the boolean query. 
		Query& query;
	      
		// If true, documents documents which <i>do not</i>
		//	match this sub-query will <i>not</i> match the boolean query. 
		bool required;
	      
		// If true, documents documents which <i>do</i>
		//	match this sub-query will <i>not</i> match the boolean query. 
		bool prohibited;
	      
		// Constructs a BooleanClause with query <code>q</code>, required
		//	<code>r</code> and prohibited <code>p</code>.  
		BooleanClause(Query& q, const bool DeleteQuery,const bool r, const bool p):
			query(q),
			required(r),
			prohibited(p),
			deleteQuery(DeleteQuery)
		{
		}
		~BooleanClause(){
			if ( deleteQuery )
				delete &query;
		}
	};
}}
#endif

