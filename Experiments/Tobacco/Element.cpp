#include "Sequence.h"

#include <stdexcept>

void Element::add(Sequence*, vector<Sequence*>::iterator)
{ throw domain_error("Element does not support this operation."); }

void Element::remove(vector<Sequence*>::iterator)
{ throw domain_error("Element does not support this operation."); }

vector<Sequence*>::iterator Element::getIterator(void)
{ throw domain_error("Element does not support this operation."); }

void Element::accept(SequenceVisitor& visitor)
{ visitor.visit(this); }

string& Element::getView(void)	
{ return m_view; }

void Element::setView(string& view)
{ m_view = view; }
