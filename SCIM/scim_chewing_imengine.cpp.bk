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

#define Uses_SCIM_UTILITY
#define Uses_SCIM_IMENGINE
#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_CONFIG_PATH
#define Uses_SCIM_LOOKUP_TABLE
#define Uses_SCIM_IMENGINE_MODULE
#define Uses_SCIM_ICONV
#define Uses_SCIM_DEBUG
#define Uses_SCIM_C_STRING

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_GETTEXT
  #include <libintl.h>
  #define _(String) dgettext(PACKAGE,String)
  #define N_(String) (String)
#else
  #define _(String) (String)
  #define N_(String) (String)
  #define bindtextdomain(Package,Directory)
  #define textdomain(domain)
  #define bind_textdomain_codeset(domain,codeset)
#endif

#define SCIM_PROP_STATUS                                                     "/IMEngine/Chinese/Chewing/ChiEngMode"

#include <scim.h>
#include <chewing/chewing.h>

#include "scim_chewing_imengine.h"
#include "scim_chewing_config_entry.h"

/**
 * LIBCHEWING_ENCODING is a new macro introduced in libchewing 0.3.
 * In stable release (0.2.x) doesn't provide this macro, and still
 * uses Big5 as its internal encoding.
 */
#ifndef LIBCHEWING_ENCODING
#define LIBCHEWING_ENCODING "BIG5"
#endif

using namespace scim;

static IMEngineFactoryPointer _scim_chewing_factory( 0 );
static ConfigPointer _scim_config( 0 );

static Property _status_property (SCIM_PROP_STATUS, "");
//static Property _letter_property (SCIM_PROP_LETTER, _("Full/Half Letter"));
//static Property _punct_property  (SCIM_PROP_PUNCT, _("Full/Half Punct"));

extern "C" {
	void scim_module_init()
	{
	}

	void scim_module_exit()
	{
		_scim_config.reset();
	}

	unsigned int scim_imengine_module_init( const ConfigPointer& config )
	{
		_status_property.set_tip (_("The status of the current input method. Click to change it."));
		_status_property.set_label (_("Eng"));
		_scim_config = config;
		return 1;
	}

	IMEngineFactoryPointer scim_imengine_module_create_factory(
			uint32 engine )
	{
		if ( engine != 0 )
			return IMEngineFactoryPointer( 0 );

		if ( _scim_chewing_factory.null() ) {
			ChewingIMEngineFactory *factory =
				new ChewingIMEngineFactory( _scim_config );
			if ( factory && factory->valid() )
				_scim_chewing_factory = factory;
			else
				delete factory;
		}
		return _scim_chewing_factory;
	}
}

/**
 * Implementation of ChewingIMEngineFactory.
 */

ChewingIMEngineFactory::ChewingIMEngineFactory( const ConfigPointer& config )
	: m_config( config ),
	  m_valid( false )
{
	reload_config( config );
	set_languages( "zh_TW,zh_HK,zh_SG" );
	m_valid = init();
	m_reload_signal_connection = m_config->signal_connect_reload (
			slot (this, &ChewingIMEngineFactory::reload_config));
}

bool ChewingIMEngineFactory::init()
{
	char prefix[] = CHEWING_DATADIR;
	char hash_postfix[] = "/.chewing/";

	/*
	 * XXX: awkful initialization routines performed on libchewing.
	 * The routines will be cleaned up in libchewing-0.3.x.
	 */
	ReadTree( prefix );
	if ( InitChar( prefix ) == 0 ) {
		SCIM_DEBUG_IMENGINE( 1 ) << 
			"Dictionary file corrupted!\n";
		return false;
	}
	InitDict( prefix );
	// NOTE: we can use scim_get_home_dir() for getting $HOME
	if ( ReadHash( (char *)(scim_get_home_dir() + hash_postfix).c_str() ) == 0 ) {
		SCIM_DEBUG_IMENGINE( 1 ) << 
			"User Phrase Library load failed!\n";
		return false;
	}
	return true;
}

void ChewingIMEngineFactory::reload_config( const ConfigPointer &scim_config )
{
	String str;
	// Load Chi/Eng mode keys
	str = m_config->read(
			String (SCIM_CONFIG_IMENGINE_CHEWING_CHI_ENG_KEY),
			String ("Shift+Shift_L+KeyRelease") + 
			String ("Shift+Shift_R+KeyRelease"));
	scim_string_to_key_list ( m_chi_eng_keys, str );

	// Load keyboard type
	m_default_KeyboardType = m_config->read (
			String( SCIM_CONFIG_IMENGINE_CHEWING_USER_KB_TYPE ),
			String( "KB_DEFAULT" ));

	// SCIM_CONFIG_IMENGINE_CHEWING_ADD_PHRASE_FORWARD
	m_add_phrase_forward = m_config->read(
			String (SCIM_CONFIG_IMENGINE_CHEWING_ADD_PHRASE_FORWARD),
			false);
}

ChewingIMEngineFactory::~ChewingIMEngineFactory()
{
	m_reload_signal_connection.disconnect ();
}

WideString ChewingIMEngineFactory::get_name() const
{
	return utf8_mbstowcs( _( "Chewing" ) );
}

String ChewingIMEngineFactory::get_uuid() const
{
	return String( "fcff66b6-4d3e-4cf2-833c-01ef66ac6025" );
}

String ChewingIMEngineFactory::get_icon_file() const
{
	return String( SCIM_CHEWING_ICON_FILE );
}

WideString ChewingIMEngineFactory::get_authors() const
{
	return utf8_mbstowcs( _( "Chewing core team <http://chewing.csie.net>" ) );
}

WideString ChewingIMEngineFactory::get_credits() const
{
	return WideString();
}

WideString ChewingIMEngineFactory::get_help() const
{
	String help;
	String chi_eng_mode_switch;

	scim_key_list_to_string( chi_eng_mode_switch, m_chi_eng_keys );


	help =	String( _( "Hot Keys:" ) ) +
		String( "\n\n  " ) + chi_eng_mode_switch + String( ":\n" ) +
		String( _( "    Switch between English/Chinese mode." ) ) +
		String( _( "\n\n  Space:\n"
			   "    Use space key to select candidate phrases."
			   "\n\n  Tab:\n"
			   "    Use tab key to dispart or connect a phrase."
			   "\n\n  Ctrl + [number]:\n"
			   "    Use ctrl + number key to add a user-defined phrase. (Here number stands for the length of the user-defined phrase.)"
			   "\n\n  Ctrl + 0:\n"
			   "    Use Ctrl + 0 to specify symbolic input."
		) );

	return utf8_mbstowcs( help );
}

IMEngineInstancePointer ChewingIMEngineFactory::create_instance(
		const String& encoding, int id )
{
	return new ChewingIMEngineInstance( this, encoding, id );
}

bool ChewingIMEngineFactory::validate_encoding( const String& encoding ) const
{
	return IMEngineFactoryBase::validate_encoding( encoding );
}

bool ChewingIMEngineFactory::validate_locale( const String& locale ) const
{
	return IMEngineFactoryBase::validate_locale( locale );
}

/**
 * Implementation of ChewingIMEngineInstance
 */

ChewingIMEngineInstance::ChewingIMEngineInstance(
		ChewingIMEngineFactory *factory, 
		const String& encoding, int id )
	: IMEngineInstanceBase( factory, encoding, id ),
	  m_factory( factory )
{
	reload_config( m_factory->m_config );
	m_iconv.set_encoding( LIBCHEWING_ENCODING );
	m_lookup_table.init();

	m_reload_signal_connection =
		m_factory->m_config->signal_connect_reload(
			slot( this, &ChewingIMEngineInstance::reload_config ) );
}

void ChewingIMEngineInstance::reload_config( const ConfigPointer& scim_config )
{
	char default_selectionKeys[] = "1234567890";

	// Reset all data.
	reset();

	config.selectAreaLen = SCIM_CHEWING_SELECTION_KEYS_NUM * 5 + 5;
	config.maxChiSymbolLen = 16;

	/* Configure selection keys definition */
	// XXX: need to support configure 
	//m_factory->m_config->read(
	//		String( SCIM_CONFIG_IMENGINE_CHEWING_USER_SELECTION_KEYS ),
	//		default_selectionKeys );
	default_selectionKeys[ SCIM_CHEWING_SELECTION_KEYS_NUM ] = '\0';
	for ( int i = 0; i < SCIM_CHEWING_SELECTION_KEYS_NUM; i++ ) {
		config.selKey[ i ] = default_selectionKeys[ i ];
	}

	// SCIM_CONFIG_IMENGINE_CHEWING_ADD_PHRASE_FORWARD
	config.bAddPhraseForward = m_factory->m_add_phrase_forward ? 0 : 1;

	SetConfig( &da, &config );
}


ChewingIMEngineInstance::~ChewingIMEngineInstance()
{
	m_reload_signal_connection.disconnect();
}

bool ChewingIMEngineInstance::process_key_event( const KeyEvent& key )
{

	if ( match_key_event( m_factory->m_chi_eng_keys, key ) ) {
		m_prev_key = key;
		OnKeyCapslock( &da, &gOut );
		refresh_status_property();
		return commit( &gOut );
	}
	m_prev_key = key;

	/* 
	 * This is a workaround against the known issue in OpenOffice with 
	 * GTK+ im module hanlding key pressed/released events.
	 */
	if ( key.is_key_release() )
		return true;

	if (
		key.mask == SCIM_KEY_NullMask && 
		key.mask != SCIM_KEY_ShiftMask &&
		key.mask != SCIM_KEY_ControlMask ) {
		switch ( key.code ) {
			case SCIM_KEY_Left:
				OnKeyLeft( &da, &gOut );
				break;
			case SCIM_KEY_Right:
				OnKeyRight( &da, &gOut );
				break;
			case SCIM_KEY_Up:
				OnKeyUp( &da, &gOut );
				break;
			case SCIM_KEY_Down:
				OnKeyDown( &da, &gOut );
				break;
			case SCIM_KEY_space:
				OnKeySpace( &da, &gOut );
				break;
			case SCIM_KEY_Return:
				OnKeyEnter( &da, &gOut );
				break;
			case SCIM_KEY_BackSpace:
				OnKeyBackspace( &da, &gOut );
				break;
			case SCIM_KEY_Escape:
				OnKeyEsc( &da, &gOut );
				break;
			case SCIM_KEY_Delete:
				OnKeyDel( &da, &gOut );
				break;
			case SCIM_KEY_Home:
				OnKeyHome( &da, &gOut );
				break;
			case SCIM_KEY_End:
				OnKeyEnd( &da, &gOut );
				break;
			case SCIM_KEY_Tab:
				OnKeyTab( &da, &gOut );
				break;
			default:
				OnKeyDefault(
					&da, 
					key.get_ascii_code(), 
					&gOut );
				break;
		}
	}
	else if ( key.mask == SCIM_KEY_ShiftMask ) {
		switch ( key.code ) {
			case SCIM_KEY_Left:
				OnKeyShiftLeft( &da, &gOut );
				break;
			case SCIM_KEY_Right:
				OnKeyShiftRight( &da, &gOut );
				break;
			default:
				OnKeyDefault(
					&da,
					key.get_ascii_code(),
					&gOut );
				break;
		}
	}
	else if ( key.mask == SCIM_KEY_ControlMask ) {
		if ( 
			key.get_ascii_code() <= '9' && 
			key.get_ascii_code() >= '0' ) {
			OnKeyCtrlNum( &da, key.get_ascii_code(), &gOut );
		}
	}
	return commit( &gOut );
}

void ChewingIMEngineInstance::move_preedit_caret( unsigned int pos )
{
}

void ChewingIMEngineInstance::select_candidate ( unsigned int index )
{
	OnKeyDefault(
		&da, 
		'1' + index, 
		&gOut );
	commit( &gOut );
}

void ChewingIMEngineInstance::update_lookup_table_page_size(
		unsigned int page_size )
{
	da.config.selectAreaLen = page_size * 5 + 5;
}

void ChewingIMEngineInstance::lookup_table_page_up()
{
	OnKeySpace( &da, &gOut );
	commit( &gOut );
}

void ChewingIMEngineInstance::lookup_table_page_down()
{
	OnKeySpace( &da, &gOut );
	commit( &gOut );
}

void ChewingIMEngineInstance::reset()
{
	/* Configure Keyboard Type */
	cf.kb_type = KBStr2Num(
			(char *) m_factory->m_default_KeyboardType.c_str() );

	/*
	 * XXX: The legacy code doesn't make sense, and will be removed in
	 * 0.3.x version of libchewing.
	 */
	cf.inp_cname = (char *) strdup( "Chewing" );
	cf.inp_ename = (char *) strdup( "Chewing" );

	InitChewing( &da, &cf );
}

void ChewingIMEngineInstance::focus_in()
{
	initialize_all_properties ();
}

void ChewingIMEngineInstance::focus_out()
{
	OnKeyEnter( &da, &gOut );
	commit( &gOut );
}

void ChewingIMEngineInstance::trigger_property( const String& property )
{
}

bool ChewingIMEngineInstance::commit( ChewingOutput *pgo )
{
	AttributeList attr;
	// commit string
	m_commit_string = WideString();
	if ( pgo->keystrokeRtn & KEYSTROKE_COMMIT ) {
		for ( int i = 0; i < pgo->nCommitStr; i++ ) {
			m_iconv.convert(
					m_converted_string, 
					(char *) pgo->commitStr[ i ].s,
					strlen((char *)pgo->commitStr[ i ].s) );
			m_commit_string += m_converted_string;
		}
		commit_string( m_commit_string );
	}
	m_preedit_string = WideString();
	// preedit string
	// XXX show Interval
	for ( int i = 0; i < pgo->chiSymbolCursor; i++ ) {
		m_iconv.convert( 
			m_converted_string, 
			(char *) pgo->chiSymbolBuf[ i ].s,
			strlen((char *)pgo->chiSymbolBuf[ i ].s) );
		m_preedit_string += m_converted_string;
	}
	// zuin string
	for ( int i = 0; i < ZUIN_SIZE; i++ ) {
		if ( pgo->zuinBuf[ i ].s[ 0 ] != '\0' ) {
			 m_iconv.convert(
				m_converted_string, 
				(char *) pgo->zuinBuf[ i ].s,
				strlen((char *)pgo->zuinBuf[ i ].s) );
			 m_preedit_string += m_converted_string;
		}
	}
	for ( int i = pgo->chiSymbolCursor; i < pgo->chiSymbolBufLen; i++ ) {
		m_iconv.convert( 
			m_converted_string, 
			(char *) pgo->chiSymbolBuf[ i ].s,
			strlen((char *)pgo->chiSymbolBuf[ i ].s) );
		m_preedit_string += m_converted_string;
	}

	for ( int i = 0; i < pgo->nDispInterval; i++ ) {
		if ( pgo->dispInterval[ i ].to - pgo->dispInterval[ i ].from > 1 ) {
			attr.push_back(Attribute(
						pgo->dispInterval[ i ].from,
						pgo->dispInterval[ i ].to - pgo->dispInterval[ i ].from,
						SCIM_ATTR_DECORATE,
						SCIM_ATTR_DECORATE_UNDERLINE));
			attr.push_back(Attribute(
						pgo->dispInterval[ i ].from,
						pgo->dispInterval[ i ].to - pgo->dispInterval[ i ].from,
						SCIM_ATTR_FOREGROUND,
						i % 2 ? SCIM_RGB_COLOR_BLUE(128): SCIM_RGB_COLOR_BLUE(255) ));
		}
	}
	// cursor decoration
	attr.push_back(Attribute(pgo->chiSymbolCursor, 1, SCIM_ATTR_DECORATE, SCIM_ATTR_DECORATE_REVERSE));

	// update display
	update_preedit_string( m_preedit_string,attr );
	update_preedit_caret( pgo->chiSymbolCursor );

	// show preedit string
	if ( m_preedit_string.empty() ) {
		hide_preedit_string();
	} else {
		show_preedit_string();
	}
	
	// show lookup table
	if ( pgo->pci->nPage != 0 ) {
		m_lookup_table.update( pgo->pci );
		update_lookup_table( m_lookup_table );
		show_lookup_table();
	} else {
		hide_lookup_table();
	}
	
	// show aux string
	m_aux_string = WideString();
	if ( pgo->bShowMsg ) {
		for ( int i = 0; i < pgo->showMsgLen; i++ ) {
			m_iconv.convert(
				m_converted_string,
				(char *) pgo->showMsg[ i ].s,
				strlen((char *)pgo->showMsg[ i ].s) );
			m_aux_string += m_converted_string;
		}
		update_aux_string( m_aux_string );
		show_aux_string();
		pgo->showMsgLen = 0;
	} else {
		hide_aux_string();
	}
	if ( pgo->keystrokeRtn & KEYSTROKE_ABSORB )
		return true;
	if ( pgo->keystrokeRtn & KEYSTROKE_IGNORE )
		return false;
	return true;
}

bool ChewingIMEngineInstance::match_key_event(
		const KeyEventList &keylist,
		const KeyEvent &key )
{
	KeyEventList::const_iterator kit;
	for( kit = keylist.begin(); kit != keylist.end(); ++kit ) {
		if (key.code == kit->code && key.mask == kit->mask)
			if (key.is_key_press() || m_prev_key.code == key.code)
				return true;
	}
	return false;
}

void ChewingIMEngineInstance::initialize_all_properties ()
{
	PropertyList proplist;
	proplist.push_back (_status_property);

	register_properties (proplist);
	refresh_all_properties ();
}

void ChewingIMEngineInstance::refresh_all_properties ()
{
	refresh_status_property ();
}

void ChewingIMEngineInstance::refresh_status_property ()
{
	if ( GetChiEngMode( &da ) != CHINESE_MODE )
		_status_property.set_label (_("Eng"));
	else
		_status_property.set_label (_("Chi"));

	update_property (_status_property);
}

ChewingLookupTable::ChewingLookupTable()
	: LookupTable( SCIM_CHEWING_SELECTION_KEYS_NUM )
{
}

ChewingLookupTable::~ChewingLookupTable()
{
}

WideString ChewingLookupTable::get_candidate( int index ) const
{
	WideString m_converted_string;
	int no = pci->pageNo * pci->nChoicePerPage;
	m_iconv.convert(
		m_converted_string, 
		(char *) pci->totalChoiceStr[ no + index ], 
		(int) strlen( (char *) pci->totalChoiceStr[ no + index ] ) );
	return m_converted_string;
}

AttributeList ChewingLookupTable::get_attributes( int index ) const
{
	return AttributeList();
}

unsigned int ChewingLookupTable::number_of_candidates() const
{
	return pci->nTotalChoice;
}

void ChewingLookupTable::clear()
{
}

void ChewingLookupTable::init()
{
	std::vector< WideString > labels;
	m_iconv.set_encoding( LIBCHEWING_ENCODING );
	char buf[ 2 ] = { 0, 0 };
	for ( int i = 0; i < (SCIM_CHEWING_SELECTION_KEYS_NUM); ++i ) {
		buf[ 0 ] = '1' + i;
		labels.push_back( utf8_mbstowcs( buf ) );
	}
	buf[ 0 ] = '0';
	labels.push_back( utf8_mbstowcs( buf ) );
	set_candidate_labels( labels );
}

void ChewingLookupTable::update( ChoiceInfo *ci )
{
	pci = ci;
}

