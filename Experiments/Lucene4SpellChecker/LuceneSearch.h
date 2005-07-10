#ifndef _LUCENESEARCH_H_
#define _LUCENESEARCH_H_

#include "CLucene.h"
#include "CLucene/util/Reader.h"

#include <vector>

using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::util;
using namespace lucene::store;
using namespace lucene::document;
using namespace lucene::search;
using namespace lucene::queryParser;

static void run(const char* query, vector<const char*>& outputVectorRef);

#endif
