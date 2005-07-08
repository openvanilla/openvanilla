#ifndef MultiFieldQueryParser_H
#define MultiFieldQueryParser_H

#include "CLucene/StdHeader.h"
#include "CLucene/analysis/AnalysisHeader.h"
#include "CLucene/search/SearchHeader.h"
#include "QueryParser.h"

using namespace lucene::index;
using namespace lucene::util;
using namespace lucene::search;
using namespace lucene::analysis;

namespace lucene{ namespace queryParser{
    /**
     * A QueryParser which constructs queries to search multiple fields.
     *
     * @author <a href="mailto:kelvin@relevanz.com">Kelvin Tan</a>
     * @version $Revision: 1.2 $
     */
    class MultiFieldQueryParser: public QueryParser
    {
    public:
        static const l_byte_t NORMAL_FIELD     = 0;
        static const l_byte_t REQUIRED_FIELD   = 1;
        static const l_byte_t PROHIBITED_FIELD = 2;
    
        /*MultiFieldQueryParser(QueryParserTokenManager tm)
        {
            super(tm);
        }
    
        MultiFieldQueryParser(Reader* stream):
            QueryParser(stream)
        {
        }*/
    
       MultiFieldQueryParser(char_t* f, Analyzer& a):
        QueryParser(f,a)
        {
        }
    
        /**
         * <p>
         * Parses a query which searches on the fields specified.
         * <p>
         * If x fields are specified, this effectively constructs:
         * <pre>
         * <code>
         * (field1:query) (field2:query) (field3:query)...(fieldx:query)
         * </code>
         * </pre>
         *
         * @param query Query string to parse
         * @param fields Fields to search on
         * @param analyzer Analyzer to use
         * @throws ParserException if query parsing fails
         * @throws TokenMgrError if query parsing fails
         */
        static Query& Parse(const char_t* query, const char_t** fields, const int_t fieldsLen, Analyzer& analyzer);
    
        /**
         * <p>
         * Parses a query, searching on the fields specified.
         * Use this if you need to specify certain fields as required,
         * and others as prohibited.
         * <p><pre>
         * Usage:
         * <code>
         * String[] fields = {"filename", "contents", "description"};
         * int_t[] flags = {MultiFieldQueryParser.NORMAL FIELD,
         *                MultiFieldQueryParser.REQUIRED FIELD,
         *                MultiFieldQueryParser.PROHIBITED FIELD,};
         * parse(query, fields, flags, analyzer);
         * </code>
         * </pre>
         *<p>
         * The code above would construct a query:
         * <pre>
         * <code>
         * (filename:query) +(contents:query) -(description:query)
         * </code>
         * </pre>
         *
         * @param query Query string to parse
         * @param fields Fields to search on
         * @param flags Flags describing the fields
         * @param analyzer Analyzer to use
         * @throws ParserException if query parsing fails
         * @throws TokenMgrError if query parsing fails
         */
        static Query& Parse(const char_t* query, const char_t** fields, const int_t fieldsLen, const l_byte_t* flags, Analyzer& analyzer);
    };
}}
#endif
