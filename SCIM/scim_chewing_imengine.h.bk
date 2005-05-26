/*
 * SCIM-chewing -
 *	Intelligent Chinese Phonetic IM Engine for SCIM.
 *
 * Copyright (c) 2004, 2005
 *	SCIM-chewing Developers. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

#ifndef SCIM_CHEWING_H
#define SCIM_CHEWING_H

using namespace scim;

class ChewingLookupTable : public LookupTable
{
	public:
		ChewingLookupTable();
		virtual ~ChewingLookupTable();
		virtual WideString get_candidate( int index ) const;
		virtual AttributeList get_attributes ( int index ) const;
		virtual unsigned int number_of_candidates() const;
		virtual void clear();
		void init();
		void update( ChoiceInfo *ci );
	private:
		ChoiceInfo *pci;
		int current_page;
		IConvert m_iconv;
};

class ChewingIMEngineFactory : public IMEngineFactoryBase
{
public:
	ChewingIMEngineFactory( const ConfigPointer& config );
	virtual ~ChewingIMEngineFactory();

	virtual WideString get_name() const;
	virtual String get_uuid() const;
	virtual String get_icon_file() const;
	virtual WideString get_authors() const;
	virtual WideString get_credits() const;
	virtual WideString get_help() const;

	virtual bool validate_encoding( const String& encoding ) const;
	virtual bool validate_locale( const String& locale ) const;

	virtual IMEngineInstancePointer create_instance(
		const String& encoding, int id = -1 );
	bool valid() const { return m_valid; }
	ConfigPointer m_config;

private:
	bool init();
	bool m_valid;
	void reload_config( const ConfigPointer &scim_config );
	Connection m_reload_signal_connection;

	KeyEventList m_chi_eng_keys;
	String m_default_KeyboardType;
	bool m_add_phrase_forward;

	friend class ChewingIMEngineInstance;
};

class ChewingIMEngineInstance : public IMEngineInstanceBase
{
public:
	ChewingIMEngineInstance(
		ChewingIMEngineFactory *factory, 
		const String& encoding, int id = -1 );

	virtual ~ChewingIMEngineInstance();

	virtual bool process_key_event( const KeyEvent& key );
	virtual void move_preedit_caret( unsigned int pos );
	virtual void select_candidate( unsigned int index );
	virtual void update_lookup_table_page_size( unsigned int page_size );
	virtual void lookup_table_page_up();
	virtual void lookup_table_page_down();
	virtual void reset();
	virtual void focus_in();
	virtual void focus_out();
	virtual void trigger_property( const String& property );
private:
	bool commit( ChewingOutput *pgo );
	void reload_config( const ConfigPointer &scim_config );
	bool match_key_event( const KeyEventList &keylist, const KeyEvent &key );

	void initialize_all_properties ();
	void refresh_all_properties ();
	void refresh_status_property ();

	Connection m_reload_signal_connection;
	WideString m_converted_string;
	WideString m_preedit_string;
	WideString m_commit_string;
	WideString m_aux_string;
	KeyEvent m_prev_key;
	ChewingIMEngineFactory *m_factory;
	ChewingLookupTable m_lookup_table;
	IConvert m_iconv;
	ChewingConf cf;
	ChewingData da;
	ConfigData config;
	ChewingOutput gOut;
};

#endif

