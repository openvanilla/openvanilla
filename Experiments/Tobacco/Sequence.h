#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <string>

using namespace std;

class Sequence
{
public:
	virtual bool add(Sequence&) { return false; } 
	virtual bool remove(int) { return false; }
	virtual bool getChild(int, Sequence&) { return false; }
};

class Element : public Sequence
{
public:
	bool set(string& str)
	{
		element_ = str;
		return true;
	}

private:
	string element_;
};

class Clause : public Sequence
{
public:
	bool add(Sequence& seq)
	{
		clause_.push_back(seq);
		return true;
	}

	bool remove(int index)
	{
		clause_.erase(clause_.begin() + index);
		return true;
	}

	bool getChild(int index, Sequence& seq)
	{
		seq = clause_[index];
		return true;
	}

private:
	vector<Sequence> clause_;
};

#endif
