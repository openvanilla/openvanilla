#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <string>

using namespace std;

class SequenceVisitor;

class Sequence
{
public:
	virtual void add(Sequence*, vector<Sequence*>::iterator) = 0;
	virtual void remove(vector<Sequence*>::iterator) = 0;
	virtual vector<Sequence*>::iterator getIterator(void) = 0;
	virtual void accept(SequenceVisitor& visitor) = 0;
	
	virtual string& getView(void) = 0;
	virtual void setView(string& str) = 0;
};

class Element : public Sequence
{
public:
	void add(Sequence*, vector<Sequence*>::iterator);
	void remove(vector<Sequence*>::iterator);
	vector<Sequence*>::iterator getIterator(void);
	void accept(SequenceVisitor& visitor);
	
	string& getView(void);
	void setView(string& str);

protected:

private:
	string m_view;
};

class Chunk : public Sequence
{
public:
	~Chunk(void);

	void add(Sequence*, vector<Sequence*>::iterator);
	void remove(vector<Sequence*>::iterator);
	vector<Sequence*>::iterator getIterator(void);
	void accept(SequenceVisitor& visitor);
	
	string& getView(void);
	void setView(string& str);

private:
	string m_view;
	vector<Sequence*> m_seqVector;
};

class SequenceVisitor
{
public:
	void visit(Sequence*);
	void visit(Element*);
	void visit(Chunk*);

	string& getElementViews(void);
	string& getChunkViews(void);

private:
	string m_elementViews;
	string m_chunkViews;
};

#endif
