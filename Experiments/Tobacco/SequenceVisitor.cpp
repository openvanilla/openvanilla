#include "Sequence.h"

void SequenceVisitor::visit(Element* element)
{
	m_elementViews+= element->getView();
}

void SequenceVisitor::visit(Chunk* chunk)
{
	m_chunkViews+= chunk->getView();
}

string& SequenceVisitor::getElementViews(void)
{
	return m_elementViews;
}

string& SequenceVisitor::getChunkViews(void)
{
	return m_chunkViews;
}