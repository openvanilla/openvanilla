#include "AVConfig.h"
#include <fstream>

/* used */
AVConfig::AVConfig(const string &filename)
{
	_filename = filename;
	_dictionary = new AVDictionary();
	//bzero(&_timestamp, sizeof(_timestamp));
}

AVConfig::~AVConfig() 
{

}

/* used */
void AVConfig::setFilename(const string &filename)
{
	_filename = filename;
}

/* used */
bool AVConfig::load()
{
	/*
	PlistParser parser;
	ifstream fs;

	fs.open(_filename.c_str(), ios_base::binary);
	if (!fs.is_open()) {
		// We fill in a default dictionary
		_parent = parser.parse(defaultDictionary().c_str());
		return false;
	}

	fs.seekg(0, ios_base::end);
	streampos length = fs.tellg();
	fs.seekg(0);

	char *buf=(char*)calloc(1, (size_t)length);
	fs.read(buf, (streamsize)length);
	fs.close();

	_parent = parser.parse(buf);
	free(buf);

	if (!isValid()) {
		// We fill in a default dictionary
		_parent = parser.parse(defaultDictionary().c_str());        
		return false;
	}

	_timestamp = getFileTimeStamp(_filename.c_str());
	*/
	return true;
}

/* used */
bool AVConfig::write()
{
	/*
	ofstream fs;
	fs.open(_filename.c_str(), ios::out | ios::binary);
	if (!fs.is_open()) return false;
	fs << _parent.str(0, xmlhead());
	fs.close();
	*/
	return true;
}

/* used */
AVDictionary AVConfig::dictionary()
{
	/*
	if (!isValid()) return AVDictionary();

	// child: #parent #plist> #dict
	return AVDictionary(_parent.childAtIndex(0)->childAtIndex(0), false);
	*/
	return *_dictionary;
}

/* used */
bool AVConfig::needSync()
{
	/*
	time_t fts = getFileTimeStamp(_filename);
	return !isTimeStampEqual(&_timestamp, &fts);
	*/
	return false;
}

bool AVConfig::isValid()
{
	/*
	// first node: <plist>
	PlistNode *plist = _parent.childAtIndex(0);
	if (!plist) return false;

	// child: <dict>
	PlistNode *dict = plist->childAtIndex(0);
	if (!dict) return false;    
	*/

	return true;
}

/*
string AVConfig::xmlhead()
{
	const char *data="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";

	return string(data);
}
*/

/*
string AVConfig::defaultDictionary()
{
	const char *data="<plist version=\"1.0\"><dict></dict></plist>\n";
	string def = xmlhead();
	def += data;
	return def;
}
*/

/*
time_t AVConfig::getFileTimeStamp(const string &filename)
{
	const char *f=filename.c_str();
	struct stat s;
	time_t t;
	bzero(&t, sizeof(t));
	if (f && !stat(f, &s))
	{
#ifdef __APPLE__
		t=s.st_mtimespec;
#else
		t=s.st_mtime;
#endif
	}
	return t;
}
*/

/*
bool AVConfig::isTimeStampEqual(const time_t *s1, const time_t *s2)
{
	return !memcmp(s1, s2, sizeof(time_t));
}
*/
