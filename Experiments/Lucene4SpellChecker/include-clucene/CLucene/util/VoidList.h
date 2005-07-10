#ifndef _lucene_util_VoidList_
#define _lucene_util_VoidList_

#include "CLucene/StdHeader.h"
using namespace std;

namespace lucene{ namespace util{

	
	template<typename _kt=void*> class VoidList{
	private:
		vector<_kt> keys;
		bool dv;
		int dvDelType;
	public:

		void setDoDelete(const int deleteType){
			dv = true;
			dvDelType = deleteType;
		}
		VoidList():
			dv(false),
			dvDelType(DELETE_TYPE_DELETE)
		{
		}
		virtual ~VoidList(){
			clear();
		}
		VoidList ( const bool deleteValue, const int valueDelType ):
			dv(deleteValue),
			dvDelType(valueDelType)
		{
		}

		void put( _kt k ){		
			keys.push_back(k);
		}
		uint_t size() const{
			return (uint_t)keys.size();
		}
		_kt at(int_t i){
			return keys[i];
		}
		_kt at(int_t i) const{
			return keys[i];
		}
		void set(int_t i, _kt val) {
			if ( dv ){
				if ( dvDelType == DELETE_TYPE_DELETE )
					delete keys[i];
				else if ( dvDelType == DELETE_TYPE_DELETE_ARRAY )
					delete[] keys[i];
			}
			keys[i] = val;
		}

		_kt operator[](int_t i){
			return keys[i];
		}
		_kt operator[](int_t i) const{
			return keys[i];
		}
		typedef typename vector<_kt>::const_iterator const_iterator;

		typename vector<_kt>::const_iterator begin() const{
			return keys.begin();
		}

		typename vector<_kt>::const_iterator end() const{
			return keys.end();
		}
		typedef typename vector<_kt>::iterator iterator;

		typename vector<_kt>::iterator begin(){
			return keys.begin();
		}
		typename vector<_kt>::iterator end(){
			return keys.end();
		}

		void pop_back(){
			if ( dv ){
				if ( dvDelType == DELETE_TYPE_DELETE )
					delete keys[keys.size()-1];
				else if ( dvDelType == DELETE_TYPE_DELETE_ARRAY )
					delete[] keys[keys.size()-1];
			}
			keys.pop_back();
		}
		void push_back( _kt k ){
			keys.push_back( k );
		}
		void push_front(_kt k){
			keys.insert(keys.begin(),k);
		}

		void clear(){
			if ( dv ){
				for ( uint_t i=0;i<keys.size();i++ ){
					if ( dvDelType == DELETE_TYPE_DELETE )
						delete keys[i];
					else if ( dvDelType == DELETE_TYPE_DELETE_ARRAY )
						delete[] keys[i];
				}

			}
			keys.clear();
		}

		void remove(int_t i){
			if ( dv ){
				if ( dvDelType == DELETE_TYPE_DELETE )
					delete keys[i];
				else if ( dvDelType == DELETE_TYPE_DELETE_ARRAY )
					delete[] keys[i];
			}
			keys.erase( keys.begin()+i );
		}

	};
}}
#endif
