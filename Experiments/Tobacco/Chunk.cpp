#include "Sequence.h"
#include <algorithm>

struct DeleteObject
{
	template<typename T>
	void operator() (const T* ptr) const { delete ptr; }
};

Chunk::~Chunk(void)
{
	for_each(m_seqVector.begin(), m_seqVector.end(), DeleteObject());
}

bool Chunk::add(Sequence* seq, vector<Sequence*>::iterator pos)
{
	m_seqVector.insert(pos, 1, seq);
	return true;
}

bool Chunk::remove(vector<Sequence*>::iterator pos)
{
	m_seqVector.erase(pos);
	return true;
}

vector<Sequence*>::iterator Chunk::getIterator(void)
{ return m_seqVector.begin(); }

void Chunk::accept(SequenceVisitor& visitor)
{
	for(vector<Sequence*>::iterator iter = m_seqVector.begin();
		iter != m_seqVector.end();
		++iter)
		(*iter)->accept(visitor);
	visitor.visit(this);
}

string& Chunk::getView(void)
{ return m_view; }

void Chunk::setView(string& view)
{ m_view = view; }

bool Chunk::isElement(void)	
{ return false; }
