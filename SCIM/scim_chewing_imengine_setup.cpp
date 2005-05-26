/** @file scim_chewing_imengine_setup.cpp
 * implementation of Setup Module of chewing imengine module.
 */

/*
 * SCIM-chewing -
 *	Intelligent Chinese Phonetic IM Engine for SCIM.
 *
 * Copyright (c) 2005
 *	SCIM-chewing Developers. See ChangeLog for details.
 *
 * See the file "COPYING" for information on usage and redistribution
 * of this file.
 */

#define Uses_SCIM_CONFIG_BASE

#include <gtk/gtk.h>

#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#ifdef HAVE_GETTEXT
  #include <libintl.h>
  #define _(String) dgettext(GETTEXT_PACKAGE,String)
  #define N_(String) (String)
#else
  #define _(String) (String)
  #define N_(String) (String)
  #define bindtextdomain(Package,Directory)
  #define textdomain(domain)
  #define bind_textdomain_codeset(domain,codeset)
#endif

#include <scim.h>
#include <gtk/scimkeyselection.h>
#include "scim_chewing_config_entry.h"

using namespace scim;

#define scim_module_init chewing_imengine_setup_LTX_scim_module_init
#define scim_module_exit chewing_imengine_setup_LTX_scim_module_exit

#define scim_setup_module_create_ui       chewing_imengine_setup_LTX_scim_setup_module_create_ui
#define scim_setup_module_get_category    chewing_imengine_setup_LTX_scim_setup_module_get_category
#define scim_setup_module_get_name        chewing_imengine_setup_LTX_scim_setup_module_get_name
#define scim_setup_module_get_description chewing_imengine_setup_LTX_scim_setup_module_get_description
#define scim_setup_module_load_config     chewing_imengine_setup_LTX_scim_setup_module_load_config
#define scim_setup_module_save_config     chewing_imengine_setup_LTX_scim_setup_module_save_config
#define scim_setup_module_query_changed   chewing_imengine_setup_LTX_scim_setup_module_query_changed

static GtkWidget * create_setup_window();
static void load_config( const ConfigPointer &config );
static void save_config( const ConfigPointer &config );
static bool query_changed();

// Module Interface.
extern "C" {

	void scim_module_init()
	{
		//bindtextdomain( GETTEXT_PACKAGE, SCIM_CHEWING_LOCALEDIR );
		bind_textdomain_codeset( GETTEXT_PACKAGE, "UTF-8" );
	}

	void scim_module_exit()
	{
	}

	GtkWidget * scim_setup_module_create_ui()
	{
		return create_setup_window();
	}

	String scim_setup_module_get_category()
	{
		return String( "IMEngine" );
	}

	String scim_setup_module_get_name()
	{
		return String( _( "Chewing" ) );
	}

	String scim_setup_module_get_description()
	{
		return String( _( "A Intelligent Chinese Phonetic IMEngine Module." ) );
	}

	void scim_setup_module_load_config( const ConfigPointer &config )
	{
		load_config( config );
	}

	void scim_setup_module_save_config( const ConfigPointer &config )
	{
		save_config( config );
	}

	bool scim_setup_module_query_changed()
	{
		return query_changed();
	}
} // extern "C"

// Internal data structure
struct KeyboardConfigData {
	const char *key;
	const char *label;
	const char *title;
	const char *tooltip;
	GtkWidget  *entry;
	GtkWidget  *button;
	String      data;
};

// Internal data declaration.
// static bool __config_use_capslock          = true;
static bool __config_add_phrase_forward = false;
// static bool __config_show_candidate_comment= true;
static String __config_kb_type_data;
static String __config_kb_type_data_translated;

static bool __have_changed                 = false;

// static GtkWidget    * __widget_use_capslock          = 0;
static GtkWidget    * __widget_add_phrase_forward = 0;
static GtkWidget    * __widget_kb_type = 0;
static GList *kb_type_list = 0;
// static GtkWidget    * __widget_show_candidate_comment= 0;
static GtkTooltips  * __widget_tooltips              = 0;

static KeyboardConfigData __config_keyboards[] =
{
    {
        // key
        SCIM_CONFIG_IMENGINE_CHEWING_TRIGGER_KEY,
        // label
        _( "Trigger keys:" ),
        // title
        _( "Select trigger keys" ),
        // tooltip
        _( "The key events to switch Chewing input mode. "
            "Click on the button on the right to edit it." ),
        // entry
        NULL,
        // button
        NULL,
        // data
        "Ctrl+space"
    },
    {
        // key
        SCIM_CONFIG_IMENGINE_CHEWING_CHI_ENG_KEY,
        // label
        _("Chewing CHI/ENG keys:"),
        // title
        _("Select CHI/ENG keys"),
        // tooltip
        _("The key events to switch English and Chinese mode. "
           "Click on the button on the right to edit it."),
        // entry
        NULL,
        // button
        NULL,
        // data
        "Shift+Shift_L+KeyRelease"
    },
    {
        // key
        NULL,
        // label
        NULL,
        // title
        NULL,
        // tooltip
        NULL,
        // entry
        NULL,
        // button
        NULL,
        // data
        ""
    },
};

// Declaration of internal functions.
static void on_default_editable_changed(
		GtkEditable *editable,
		gpointer user_data );

static void on_default_toggle_button_toggled(
		GtkToggleButton *togglebutton,
		gpointer user_data );

static void on_default_key_selection_clicked(
		GtkButton *button,
		gpointer user_data );

static void setup_widget_value();

static GtkWidget *create_options_page();

static GtkWidget *create_keyboard_page();

// Function implementations.
static GtkWidget *create_options_page()
{
	GtkWidget *vbox;


	vbox = gtk_vbox_new (FALSE, 0);
	gtk_widget_show (vbox);

//	__widget_use_capslock = gtk_check_button_new_with_mnemonic (_("Use _CapsLock"));
//	gtk_widget_show (__widget_use_capslock);
//	gtk_box_pack_start (GTK_BOX (vbox), __widget_use_capslock, FALSE, FALSE, 4);
//	gtk_container_set_border_width (GTK_CONTAINER (__widget_use_capslock), 4);

	__widget_add_phrase_forward = gtk_check_button_new_with_mnemonic (_("Add _Phrase forward"));
	gtk_widget_show (__widget_add_phrase_forward);
	gtk_box_pack_start (GTK_BOX (vbox), __widget_add_phrase_forward, FALSE, FALSE, 4);
	gtk_container_set_border_width (GTK_CONTAINER (__widget_add_phrase_forward), 4);

//	__widget_show_candidate_comment = gtk_check_button_new_with_mnemonic (_("_Show candidate comment"));
//	gtk_widget_show (__widget_show_candidate_comment);
//	gtk_box_pack_start (GTK_BOX (vbox), __widget_show_candidate_comment, FALSE, FALSE, 4);
//	gtk_container_set_border_width (GTK_CONTAINER (__widget_show_candidate_comment), 4);

	// Connect all signals.
//	g_signal_connect ((gpointer) __widget_use_capslock, "toggled",
//			G_CALLBACK (on_default_toggle_button_toggled),
//			&__config_use_capslock);
	g_signal_connect ((gpointer) __widget_add_phrase_forward, "toggled",
			G_CALLBACK (on_default_toggle_button_toggled),
			&__config_add_phrase_forward);
//	g_signal_connect ((gpointer) __widget_show_candidate_comment, "toggled",
//			G_CALLBACK (on_default_toggle_button_toggled),
//			&__config_show_candidate_comment);

	// Set all tooltips.
//	gtk_tooltips_set_tip (__widget_tooltips, __widget_use_capslock,
//			_("Whether to use Caps Lock key for changing chewing output mode to Jamo or not."), NULL);

	gtk_tooltips_set_tip (__widget_tooltips, __widget_add_phrase_forward,
			_("Whether to add Phrase forward or not."), NULL);

//	gtk_tooltips_set_tip (__widget_tooltips, __widget_show_candidate_comment,
//			_("Whether to show the comment of candidates or not."), NULL);

	return vbox;
}

struct _builtin_keymap {
	char *entry;
	String translated_name;
} builtin_keymaps[] = {
		{ 
			"KB_DEFAULT",
			String( _( "Default Keyboard" ) ) },
		{       
			"KB_HSU",
			String( _( "Hsu's Keyboard" ) ) },
		{       
			"KB_GIN_YEIH",
			String( _( "Gin-Yieh Keyboard" ) ) },
		{
			"KB_ET",
			String( _( "ETen Keyboard" ) ) },
		{
			"KB_ET26",
			String( _( "ETen 26-key Keyboard" ) ) },
		{
			"KB_DVORAK",
			String( _( "Dvorak Keyboard" ) ) },
		{
			"KB_DVORAK_HSU",
			String( _( "Dvorak Keyboard with Hsu's support" ) ) },
		{
			"KB_HANYU_PINYING",
			String( _( "Han-Yu PinYin Keyboard" ) ) }
};

static GtkWidget *create_keyboard_page()
{
	GtkWidget *table;
	GtkWidget *label;

	int i;

	table = gtk_table_new (3, 3, FALSE);
	gtk_widget_show (table);

	// Create keyboard setting.
	for (i = 0; __config_keyboards [i].key; ++ i) {
		label = gtk_label_new (NULL);
		gtk_label_set_text_with_mnemonic (GTK_LABEL (label), _(__config_keyboards[i].label));
		gtk_widget_show (label);
		gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
		gtk_misc_set_padding (GTK_MISC (label), 4, 0);
		gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
				(GtkAttachOptions) (GTK_FILL),
				(GtkAttachOptions) (GTK_FILL), 4, 4);

		__config_keyboards [i].entry = gtk_entry_new ();
		gtk_widget_show (__config_keyboards [i].entry);
		gtk_table_attach (GTK_TABLE (table), __config_keyboards [i].entry, 1, 2, i, i+1,
				(GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
				(GtkAttachOptions) (GTK_FILL), 4, 4);
		gtk_entry_set_editable (GTK_ENTRY (__config_keyboards[i].entry), FALSE);

		__config_keyboards[i].button = gtk_button_new_with_label ("...");
		gtk_widget_show (__config_keyboards[i].button);
		gtk_table_attach (GTK_TABLE (table), __config_keyboards[i].button, 2, 3, i, i+1,
				(GtkAttachOptions) (GTK_FILL),
				(GtkAttachOptions) (GTK_FILL), 4, 4);
		gtk_label_set_mnemonic_widget (GTK_LABEL (label), __config_keyboards[i].button);
	}

	// Setup KB_TYPE combo box
	__widget_kb_type = gtk_combo_new();
	gtk_widget_show (__widget_kb_type);

	for (
		int i = 0; 
		i < (sizeof(builtin_keymaps) / sizeof(_builtin_keymap)); 
		i++) {
		kb_type_list = g_list_append(
				kb_type_list,
				(void *) builtin_keymaps[ i ].translated_name.c_str() );
	}
	
	gtk_combo_set_popdown_strings (GTK_COMBO (__widget_kb_type), kb_type_list);
	g_list_free(kb_type_list);
	gtk_combo_set_use_arrows (GTK_COMBO (__widget_kb_type), TRUE);
	gtk_editable_set_editable (GTK_EDITABLE (GTK_ENTRY (GTK_COMBO (__widget_kb_type)->entry)), FALSE);
	label = gtk_label_new (_("Use keyboard type:"));
	gtk_widget_show (label);
	gtk_misc_set_alignment (GTK_MISC (label), 1.0, 0.5);
	gtk_misc_set_padding (GTK_MISC (label), 4, 0);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, i, i+1,
			(GtkAttachOptions) (GTK_FILL),
			(GtkAttachOptions) (GTK_FILL), 4, 4);
	gtk_table_attach (GTK_TABLE (table), __widget_kb_type, 1, 2, i, i+1,
			(GtkAttachOptions) (GTK_FILL|GTK_EXPAND),
			(GtkAttachOptions) (GTK_FILL), 4, 4);
	gtk_tooltips_set_tip (__widget_tooltips, GTK_COMBO (__widget_kb_type)->entry,
			_("Change the default keyboard layout type"), NULL);
	g_signal_connect(
		(gpointer) GTK_ENTRY(GTK_COMBO(__widget_kb_type)->entry), 
		"changed",
		G_CALLBACK (on_default_editable_changed),
		&(__config_kb_type_data_translated));
	i++;

	for (i = 0; __config_keyboards [i].key; ++ i) {
		g_signal_connect ((gpointer) __config_keyboards [i].button, "clicked",
				G_CALLBACK (on_default_key_selection_clicked),
				&(__config_keyboards [i]));
		g_signal_connect ((gpointer) __config_keyboards [i].entry, "changed",
				G_CALLBACK (on_default_editable_changed),
				&(__config_keyboards [i].data));
	}

	for (i = 0; __config_keyboards [i].key; ++ i) {
		gtk_tooltips_set_tip (__widget_tooltips, __config_keyboards [i].entry,
				_(__config_keyboards [i].tooltip), NULL);
	}

	return table;
}

static GtkWidget *create_setup_window()
{
	static GtkWidget *window = 0;

	if (!window) {
		GtkWidget *notebook;
		GtkWidget *label;
		GtkWidget *page;

		__widget_tooltips = gtk_tooltips_new ();

		// Create the Notebook.
		notebook = gtk_notebook_new ();
		gtk_widget_show (notebook);

		// Create the first page.
		page = create_options_page ();
		gtk_container_add (GTK_CONTAINER (notebook), page);

		// Create the label for this note page.
		label = gtk_label_new (_("Options"));
		gtk_widget_show (label);
		gtk_notebook_set_tab_label(
			GTK_NOTEBOOK( notebook ),
			gtk_notebook_get_nth_page( GTK_NOTEBOOK( notebook ), 0 ), 
			label );

		// Create the second page.
		page = create_keyboard_page ();

		// Create the label for this note page.
		label = gtk_label_new (_("Keyboard"));
		gtk_widget_show (label);

		// Append this page.
		gtk_notebook_append_page (GTK_NOTEBOOK (notebook), page, label);

		window = notebook;

		setup_widget_value ();
	}

	return window;
}

void setup_widget_value()
{
//	if (__widget_use_capslock) {
//		gtk_toggle_button_set_active (
//				GTK_TOGGLE_BUTTON (__widget_use_capslock),
//				__config_use_capslock);
//	}

	if (__widget_add_phrase_forward) {
		gtk_toggle_button_set_active (
				GTK_TOGGLE_BUTTON (__widget_add_phrase_forward),
				__config_add_phrase_forward);
	}

//	if (__widget_show_candidate_comment) {
//		gtk_toggle_button_set_active (
//				GTK_TOGGLE_BUTTON (__widget_show_candidate_comment),
//				__config_show_candidate_comment);
//	}

	for (int i = 0; __config_keyboards [i].key; ++ i) {
		if (__config_keyboards [i].entry) {
			gtk_entry_set_text (
					GTK_ENTRY (__config_keyboards [i].entry),
					__config_keyboards [i].data.c_str ());
		}
	}

	int index_keymap = (sizeof(builtin_keymaps) / sizeof(_builtin_keymap)) - 1;
	for ( ; index_keymap >= 0;  index_keymap--) {
		if ( __config_kb_type_data == builtin_keymaps[index_keymap].entry ) {
			break;
		}
	}
	if (index_keymap < 0)
		index_keymap = 0;
	
	gtk_entry_set_text (
			GTK_ENTRY(GTK_COMBO(__widget_kb_type)->entry),
			builtin_keymaps[index_keymap].translated_name.c_str()
	);
}

void load_config( const ConfigPointer &config )
{
	if (!config.null ()) {
//		__config_use_capslock =
//			config->read (String (SCIM_CONFIG_IMENGINE_CHEWING_USE_CAPSLOCK),
//					__config_use_capslock);
		__config_add_phrase_forward =
			config->read (String (SCIM_CONFIG_IMENGINE_CHEWING_ADD_PHRASE_FORWARD),
					__config_add_phrase_forward);
		__config_kb_type_data = 
			config->read (String (SCIM_CONFIG_IMENGINE_CHEWING_USER_KB_TYPE),
					__config_kb_type_data);
//		__config_show_candidate_comment =
//			config->read (String (SCIM_CONFIG_IMENGINE_CHEWING_SHOW_CANDIDATE_COMMENT),
//					__config_show_candidate_comment);

		for (int i = 0; __config_keyboards [i].key; ++ i) {
			__config_keyboards [i].data =
				config->read (String (__config_keyboards [i].key),
						__config_keyboards [i].data);
		}

		setup_widget_value ();

		__have_changed = false;
	}
}

void save_config( const ConfigPointer &config )
{
	if (!config.null ()) {
//		config->write (String (SCIM_CONFIG_IMENGINE_CHEWING_USE_CAPSLOCK),
//				__config_use_capslock);
		config->write (String (SCIM_CONFIG_IMENGINE_CHEWING_ADD_PHRASE_FORWARD),
				__config_add_phrase_forward);
	        
		int index_keymap = 
			(sizeof(builtin_keymaps) / sizeof(_builtin_keymap)) - 1;
		for ( ; index_keymap >= 0;  index_keymap--) {
			if (__config_kb_type_data_translated == 
				builtin_keymaps[index_keymap].translated_name ) {
				break;
			}
		}
		if (index_keymap < 0)
			index_keymap = 0;
		__config_kb_type_data = builtin_keymaps[index_keymap].entry;

		config->write (String (SCIM_CONFIG_IMENGINE_CHEWING_USER_KB_TYPE),
				__config_kb_type_data);
//		config->write (String (SCIM_CONFIG_IMENGINE_CHEWING_SHOW_CANDIDATE_COMMENT),
//				__config_show_candidate_comment);

		for (int i = 0; __config_keyboards [i].key; ++ i) {
			config->write (String (__config_keyboards [i].key),
					__config_keyboards [i].data);
		}

		__have_changed = false;
	}
}

bool query_changed()
{
	return __have_changed;
}

static void on_default_editable_changed(
		GtkEditable *editable,
		gpointer user_data )
{
	String *str = static_cast< String * >( user_data );

	if ( str ) {
		*str = String( gtk_entry_get_text( GTK_ENTRY( editable ) ) );
		__have_changed = true;
	}
}

static void on_default_toggle_button_toggled(
		GtkToggleButton *togglebutton,
		gpointer user_data )
{
	bool *toggle = static_cast< bool * >( user_data );

	if ( toggle ) {
		*toggle = gtk_toggle_button_get_active( togglebutton );
		__have_changed = true;
	}
}

static void on_default_key_selection_clicked(
		GtkButton *button,
		gpointer user_data )
{
	KeyboardConfigData *data = static_cast< KeyboardConfigData * >( user_data );

	if ( data ) {
		GtkWidget *dialog = scim_key_selection_dialog_new (_(data->title));
		gint result;

		scim_key_selection_dialog_set_keys (
				SCIM_KEY_SELECTION_DIALOG( dialog ),
				gtk_entry_get_text( GTK_ENTRY( data->entry ) ) );

		result = gtk_dialog_run( GTK_DIALOG( dialog ) );

		if ( result == GTK_RESPONSE_OK ) {
			const gchar *keys = scim_key_selection_dialog_get_keys (
				SCIM_KEY_SELECTION_DIALOG( dialog ) );

			if ( ! keys )
				keys = "";

			if ( strcmp( keys, gtk_entry_get_text( GTK_ENTRY( data->entry ) ) ) != 0 )
				gtk_entry_set_text( GTK_ENTRY( data->entry ), keys );
		}

		gtk_widget_destroy( dialog );
	}
}
