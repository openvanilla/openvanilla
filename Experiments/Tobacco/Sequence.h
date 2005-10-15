#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <string>

using namespace std;

struct DeleteObject
{
	template<typename T>
	void operator()(const T* ptr) const { delete ptr; }
};

class Sequence
{
public:
	virtual bool add(Sequence*) = 0;
	virtual bool remove(int) = 0;
	virtual Sequence* getChild(int) = 0; 

	string* get(void) { return &sequence_; }
	void set(string& str) { sequence_ = str; }
private:
        string sequence_;
};                    

class Element : public Sequence
{
public:
	virtual bool add(Sequence*) { return false; }
	virtual bool remove(int) { return false; }
	virtual Sequence* getChild(int) { return NULL; }
};

class Chunk : public Sequence
{
public:
	~Chunk(void)
	{
		for_each(chunk_.begin(), chunk_.end(), DeleteObject());
	}

	bool add(Sequence* seq)
	{
		chunk_.push_back(seq);
		return true;
	}

	bool remove(int index)
	{
		chunk_.erase(chunk_.begin() + index);
		return true;
	}

	Sequence* getChild(int index)
	{
		return chunk_[index];
	}

private:
	vector<Sequence*> chunk_;
};

#endif
