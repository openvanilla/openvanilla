#ifndef _lucene_util_VoidMap_
#define _lucene_util_VoidMap_
#include "CLucene/StdHeader.h"

#include <functional>

using namespace std;
namespace lucene{ namespace util{

	class charCompare:public binary_function<const char_t*,const char_t*,bool>
	{
	public:
		bool operator()( const char_t* val1, const char_t* val2 ) const{
			//bool ret = (stringCompare( (char_t*)val1,(char_t*)val2 ) < 0);
			bool ret = (stringCompare( val1,val2 ) < 0);
			return ret;
		}
	};

	template<typename _kt, typename _vt, typename _Compare=charCompare > class VoidMap{
	private:
		map<_kt,_vt,_Compare> keys;
		typedef pair <_kt, _vt> pr;
		bool dk;
		bool dv;
		int dkDelType;
		int dvDelType;
	public:

		void setDoDelete(const bool onKey, const int deleteType){
			if ( onKey ){
				dk = true;
				dkDelType = deleteType;
			}else{
				dv = true;
				dvDelType = deleteType;
			}
		}
		VoidMap ():
			dk(false),
			dv(false),
			dkDelType(DELETE_TYPE_NONE),
			dvDelType(DELETE_TYPE_NONE)
		{
		}
		~VoidMap (){
			clear();
		}
		VoidMap ( const bool deleteKey, const int keyDelType, const bool deleteValue, const int valueDelType ):
			dk(deleteKey),
			dv(deleteValue),
			dkDelType(keyDelType),
			dvDelType(valueDelType)
		{
		}

		void put(_kt k,_vt v){
			keys[k] = v;
		}
		int_t size() const{
			return (int_t)keys.size();
		}
		bool exists(_kt k)const{
			typename map<_kt,_vt,_Compare>::const_iterator itr = keys.find(k);
			bool ret = itr != keys.end();
			return ret;
		}
		const _vt get(_kt k) const{
			typename map<_kt,_vt,_Compare>::const_iterator itr = keys.find(k);
			if ( itr == keys.end() )
				return NULL;
			else
				return itr->second;
		}
		_vt get(_kt k){
			typename map<_kt,_vt,_Compare>::iterator itr = keys.find(k);
			if ( itr == keys.end() )
				return NULL;
			else
				return itr->second;
		}
		const _kt getKey(_kt k) const{
			typename map<_kt,_vt,_Compare>::const_iterator itr = keys.find(k);
			if ( itr == keys.end() )
				return NULL;
			else
				return itr->first;
		}
		_kt getKey(_kt k){
			typename map<_kt,_vt,_Compare>::iterator itr = keys.find(k);
			if ( itr == keys.end() )
				return NULL;
			else
				return itr->first;
		}

		typedef typename map<_kt,_vt,_Compare>::iterator iterator;
		typedef typename map<_kt,_vt,_Compare>::const_iterator const_iterator;

		typename map<_kt,_vt,_Compare>::iterator begin(){
			return keys.begin();
		}
		typename map<_kt,_vt,_Compare>::iterator end(){
			return keys.end();
		}
		void remove(_kt key, const bool dontDeleteKey = false, const bool dontDeleteValue = false){
			typename map<_kt,_vt,_Compare>::iterator itr = keys.find(key);
			if ( itr != keys.end() ){
				//delete key first. This prevents potential loops (deleting object removes itself)
				if ( dk && !dontDeleteKey ){
					if ( dkDelType == DELETE_TYPE_DELETE )
						delete itr->first;
					else if ( dkDelType == DELETE_TYPE_DELETE_ARRAY )
						delete[] itr->first;
				}

				if ( dv && !dontDeleteValue ){
					if ( dvDelType == DELETE_TYPE_DELETE )
						delete itr->second;
					else if ( dvDelType == DELETE_TYPE_DELETE_ARRAY )
						delete[] itr->second;
				}
				keys.erase(itr);
			}
		}
		void clear(){
			if ( dk || dv ){
				typename map<_kt,_vt,_Compare>::iterator itr = keys.begin();
				while ( itr != keys.end() ){
                    if ( dk ){
						if ( dkDelType == DELETE_TYPE_DELETE )
							delete itr->first;
						else if ( dkDelType == DELETE_TYPE_DELETE_ARRAY )
							delete[] itr->first;
					}

					if ( dv ){
						if ( dvDelType == DELETE_TYPE_DELETE )
							delete itr->second;
						else if ( dvDelType == DELETE_TYPE_DELETE_ARRAY )
							delete[] itr->second;
					}

                    itr++;
				}
			}
			keys.clear();
		}
	};
}}
#endif

