#include "CLucene.h"

#include "OVFileHandler.h"
#include "OVStringToolKit.h"
#include <vector>

using namespace std;

using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::document;

int main()
{
	lucene::analysis::standard::StandardAnalyzer analyzer;
	const char* indexDirPath = "./indexDir";
	IndexWriter* writer = new IndexWriter(indexDirPath, analyzer, true);

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
