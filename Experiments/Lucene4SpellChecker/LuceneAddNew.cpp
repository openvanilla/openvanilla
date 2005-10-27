#include "CLucene.h"

#include "OVFileHandler.h"
#include "OVStringToolKit.h"
#include <vector>

#include <windows.h>

using namespace std;

using namespace lucene::index;
using namespace lucene::analysis;
using namespace lucene::document;

int main()
{
	lucene::analysis::standard::StandardAnalyzer analyzer;
	const char* indexDirPath = "./indexDir";
	IndexWriter* writer = new IndexWriter(indexDirPath, &analyzer, true);

	OVFileHandler* fileHandler = new OVFileHandler("./dict.txt");
	vector<string> lineVector;
	vector<string> outStringVector;
	vector<string> delimiterVector;
	delimiterVector.push_back("\t");
	delimiterVector.push_back(" ");
	int amount = fileHandler->getLines(lineVector);
	for(int i = 0; i < amount; i++)
	{
		OVStringToolKit::splitString(lineVector[i],
			outStringVector,
			delimiterVector,
			false);

		if(outStringVector.size() > 0)
		{
			TCHAR wordT[100];
			_tcsset(wordT, _T('\0'));
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, outStringVector[0].c_str(), outStringVector[0].length(), wordT, 100);
			Document* doc = new Document();
			doc->add( *Field::Keyword(_T("word"), wordT) );
			writer->addDocument(doc);
			delete &doc;
		}
		outStringVector.clear();
	}

	writer->optimize();
	writer->close();

	return amount;
}
