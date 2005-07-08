#include "CLucene.h"
#include "CLucene/util/Reader.h"

#include "OVFileHandler.h"
#include "OVStringToolKit.h"
#include <vector>

using namespace std;

using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::util;
using namespace lucene::store;
using namespace lucene::document;

int main()
{
	lucene::analysis::standard::StandardAnalyzer analyzer;
	IndexWriter* writer = new IndexWriter("./index", analyzer, false);

	OVFileHandler* fileHandler = new OVFileHandler("./dict.txt");
	vector<string> lineVector;
	int amount = fileHandler->getLines(lineVector);
	for(int i = 0; i < amount; i++)
	{
		char* name = "word";
		const char* word = lineVector[i].c_str();
		Document& doc = *new Document();
		doc.add( Field::Keyword(name, word) );
		writer->addDocument(doc);
		delete &doc;
	}

	writer->optimize();
	writer->close();

	return amount;
}
