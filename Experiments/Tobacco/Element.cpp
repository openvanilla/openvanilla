#include "Sequence.h"

bool Element::add(Sequence*, vector<Sequence*>::iterator)	{ return false; }
bool Element::remove(vector<Sequence*>::iterator)			{ return false; }
vector<Sequence*>::iterator Element::getIterator(void)		{ return NULL; }
void Element::accept(SequenceVisitor& visitor)				{ visitor.visit(this); }

string& Element::getView(void)							{ return m_view; }
void Element::setView(string& view)						{ m_view = view; }
size_t Element::getLength(void)							{ return 1; }
	
bool Element::isElement(void)								{ return true; }
