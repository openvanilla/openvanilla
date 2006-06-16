#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <string>

using namespace std;

class SequenceVisitor;

class Sequence
{
public:
	virtual bool add(Sequence*, vector<Sequence*>::iterator) = 0;
	virtual bool remove(vector<Sequence*>::iterator) = 0;
	virtual vector<Sequence*>::iterator getIterator(void) = 0;
	virtual void accept(SequenceVisitor& visitor) = 0;
	
	virtual string& getView(void) = 0;
	virtual void setView(string& str) = 0;
	virtual size_t getLength(void) = 0;
	
	virtual bool isElement(void) = 0;
};

class Element : public Sequence
{
public:
	virtual bool add(Sequence*, vector<Sequence*>::iterator) = 0;
	virtual bool remove(vector<Sequence*>::iterator) = 0;
	virtual vector<Sequence*>::iterator getIterator(void) = 0;
	virtual void accept(SequenceVisitor& visitor) = 0;
	
	virtual string& getView(void) = 0;
	virtual void setView(string& str) = 0;
	virtual size_t getLength(void) = 0;
	
	virtual bool isElement(void) = 0;

protected:

private:
	string m_view;
};

class Chunk : public Sequence
{
public:
	~Chunk(void);

	virtual bool add(Sequence*, vector<Sequence*>::iterator) = 0;
	virtual bool remove(vector<Sequence*>::iterator) = 0;
	virtual vector<Sequence*>::iterator getIterator(void) = 0;
	virtual void accept(SequenceVisitor& visitor) = 0;
	
	virtual string& getView(void) = 0;
	virtual void setView(string& str) = 0;
	virtual size_t getLength(void) = 0;
	
	virtual bool isElement(void) = 0;

private:
	string m_view;
	vector<Sequence*> m_seqVector;
};

class SequenceVisitor
{
public:
	virtual void visit(Sequence*) = 0;
	virtual void visit(Element*) = 0;
	virtual void visit(Chunk*) = 0;

	virtual string& getElementViews(void) = 0;
	virtual string& getChunkViews(void) = 0;

private:
	string m_elementViews;
	string m_chunkViews;
};

#endif
