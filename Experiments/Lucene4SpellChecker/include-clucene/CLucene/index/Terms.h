#include "CLucene/StdHeader.h"
#ifndef _lucene_index_Terms_
#define _lucene_index_Terms_

#include "Term.h"
namespace lucene{ namespace index {

	class TermDocs {
	public:
		virtual ~TermDocs(){ };

		// Sets this to the data for a term.
		// The enumeration is reset to the start of the data for this term.
		virtual void seek(Term* term)=0;

		// Returns the current document number.  <p> This is invalid until {@link
		//	#next()} is called for the first time.
		virtual int_t Doc() const=0;

		// Returns the frequency of the term within the current document.  <p> This
		//	is invalid until {@link #next()} is called for the first time.
		virtual int_t Freq() const=0;

		// Moves to the next pair in the enumeration.  <p> Returns true iff there is
		//	such a next pair in the enumeration.
		virtual bool next() =0;

		// Attempts to read multiple entries from the enumeration, up to length of
		// <i>docs</i>.  Document numbers are stored in <i>docs</i>, and term
		// frequencies are stored in <i>freqs</i>.  The <i>freqs</i> array must be as
		// long_t as the <i>docs</i> array.
		//
		// <p>Returns the number of entries read.  Zero is only returned when the
		// stream has been exhausted.
		virtual int_t read(int_t docs[], int_t freqs[])=0;

		// Skips entries to the first beyond the current whose document number is
		// greater than or equal to <i>target</i>. <p>Returns true iff there is such
		// an entry.  <p>Behaves as if written: <pre>
		//   bool skipTo(int_t target) {
		//     do {
		//       if (!next())
		// 	     return false;
		//     } while (target > doc());
		//     return true;
		//   }
		// </pre>
		// Some implementations are considerably more efficient than that.
		virtual bool skipTo(const int_t target)=0;

		// Frees associated resources.
		virtual void close() = 0;
	};


	// Abstract class for enumerating terms.
	//
	//<p>Term enumerations are always ordered by Term.compareTo().  Each term in
	//the enumeration is greater than all that precede it.
	class TermEnum {
	public:
		// Increments the enumeration to the next element.  True if one exists.
		virtual bool next()=0;

		// Returns the current Term in the enumeration.
		virtual Term* getTerm(const bool pointer=true)=0;

		// Returns the docFreq of the current Term in the enumeration.
		virtual int_t DocFreq() const=0;

		// Closes the enumeration to further activity, freeing resources.
		virtual void close() =0;

		virtual ~TermEnum(){ }

	};



	// TermPositions provides an interface for enumerating the &lt;document,
	// frequency, &lt;position&gt;* &gt; tuples for a term.  <p> The document and
	// frequency are as for a TermDocs.  The positions portion lists the ordinal
	// positions of each occurence of a term in a document.
	// @see IndexReader#termPositions
	class TermPositions: public virtual TermDocs {
	public:
		// Returns next position in the current document.  It is an error to call
		//	this more than {@link #freq()} times
		//	without calling {@link #next()}<p> This is
		//	invalid until {@link #next()} is called for
		//	the first time.
		virtual int_t nextPosition() = 0;

		virtual ~TermPositions(){ }
	};
}}
#endif
