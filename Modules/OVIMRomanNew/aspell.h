/* Automatically generated file.  Do not edit directly. */

/* This file is part of The New Aspell
 * Copyright (C) 2001-2002 by Kevin Atkinson under the GNU LGPL
 * license version 2.0 or 2.1.  You should have received a copy of the
 * LGPL license along with this library if you did not you can find it
 * at http://www.gnu.org/.                                              */

#ifndef ASPELL_ASPELL__H
#define ASPELL_ASPELL__H

#ifdef WIN32PORT
#ifdef ASPELL_EXPORTS
#define ASPELL_API __declspec(dllexport)
#else
#define ASPELL_API __declspec(dllimport)
#endif
#else
#define ASPELL_API
#endif
#ifdef __cplusplus
extern "C" {
#endif

/******************************* type id *******************************/


union AspellTypeId {

  unsigned int num;

  char str[4];

};


typedef union AspellTypeId AspellTypeId;


/************************** mutable container **************************/


typedef struct AspellMutableContainer AspellMutableContainer;


ASPELL_API int aspell_mutable_container_add(struct AspellMutableContainer * ths, const char * to_add);

ASPELL_API int aspell_mutable_container_remove(struct AspellMutableContainer * ths, const char * to_rem);

ASPELL_API void aspell_mutable_container_clear(struct AspellMutableContainer * ths);

ASPELL_API struct AspellMutableContainer * aspell_mutable_container_to_mutable_container(struct AspellMutableContainer * ths);



/******************************* key info *******************************/



enum AspellKeyInfoType {AspellKeyInfoString, AspellKeyInfoInt, AspellKeyInfoBool, AspellKeyInfoList};
typedef enum AspellKeyInfoType AspellKeyInfoType;


struct AspellKeyInfo {

  /* the name of the key */
  const char * name;

  /* the key type */
  enum AspellKeyInfoType type;

  /* the default value of the key */
  const char * def;

  /* a brief description of the key or null if internal value */
  const char * desc;

  /* other data used by config implementations
   * should be set to 0 if not used */
  char otherdata[16];

};


typedef struct AspellKeyInfo AspellKeyInfo;


/******************************** config ********************************/


typedef struct AspellKeyInfoEnumeration AspellKeyInfoEnumeration;


ASPELL_API int aspell_key_info_enumeration_at_end(const struct AspellKeyInfoEnumeration * ths);

ASPELL_API const struct AspellKeyInfo * aspell_key_info_enumeration_next(struct AspellKeyInfoEnumeration * ths);

ASPELL_API void delete_aspell_key_info_enumeration(struct AspellKeyInfoEnumeration * ths);

ASPELL_API struct AspellKeyInfoEnumeration * aspell_key_info_enumeration_clone(const struct AspellKeyInfoEnumeration * ths);

ASPELL_API void aspell_key_info_enumeration_assign(struct AspellKeyInfoEnumeration * ths, const struct AspellKeyInfoEnumeration * other);



typedef struct AspellConfig AspellConfig;


ASPELL_API struct AspellConfig * new_aspell_config();

ASPELL_API void delete_aspell_config(struct AspellConfig * ths);

ASPELL_API struct AspellConfig * aspell_config_clone(const struct AspellConfig * ths);

ASPELL_API void aspell_config_assign(struct AspellConfig * ths, const struct AspellConfig * other);

ASPELL_API unsigned int aspell_config_error_number(const struct AspellConfig * ths);

ASPELL_API const char * aspell_config_error_message(const struct AspellConfig * ths);

ASPELL_API const struct AspellError * aspell_config_error(const struct AspellConfig * ths);

/* sets extra keys which this config class should accept
 * begin and end are expected to point to the begging
 * and end of an array of Aspell Key Info */
ASPELL_API void aspell_config_set_extra(struct AspellConfig * ths, const struct AspellKeyInfo * begin, const struct AspellKeyInfo * end);

/* returns the KeyInfo object for the
 * corresponding key or returns null and sets
 * error_num to PERROR_UNKNOWN_KEY if the key is
 * not valid. The pointer returned is valid for
 * the lifetime of the object. */
ASPELL_API const struct AspellKeyInfo * aspell_config_keyinfo(struct AspellConfig * ths, const char * key);

/* returns a newly allocated enumeration of all the
 * possible objects this config class uses */
ASPELL_API struct AspellKeyInfoEnumeration * aspell_config_possible_elements(struct AspellConfig * ths, int include_extra);

/* returns the default value for given key which
 * way involve substating variables, thus it is
 * not the same as keyinfo(key)->def returns null
 * and sets error_num to PERROR_UNKNOWN_KEY if
 * the key is not valid. Uses the temporary
 * string. */
ASPELL_API const char * aspell_config_get_default(struct AspellConfig * ths, const char * key);

/* returns a newly alloacted enumeration of all the
 * key/value pairs. This DOES not include ones
 * which are set to their default values */
ASPELL_API struct AspellStringPairEnumeration * aspell_config_elements(struct AspellConfig * ths);

/* inserts an item, if the item already exists it
 * will be replaced. returns true if it succesed
 * or false on error. If the key in not valid it
 * sets error_num to PERROR_UNKNOWN_KEY, if the
 * value is not valid it will sets error_num to
 * PERROR_BAD_VALUE, if the value can not be
 * changed it sets error_num to
 * PERROR_CANT_CHANGE_VALUE, and if the value is
 * a list and you are trying to set it directory
 * it sets error_num to PERROR_LIST_SET */
ASPELL_API int aspell_config_replace(struct AspellConfig * ths, const char * key, const char * value);

/* remove a key and returns true if it exists
 * otherise return false. This effictly sets the
 * key to its default value. Calling replace with
 * a value of "<default>" will also call
 * remove. If the key does not exists sets
 * error_num to 0 or PERROR_NOT, if the key in
 * not valid sets error_num to
 * PERROR_UNKNOWN_KEY, if the value can not be
 * changed sets error_num to
 * PERROR_CANT_CHANGE_VALUE */
ASPELL_API int aspell_config_remove(struct AspellConfig * ths, const char * key);

ASPELL_API int aspell_config_have(const struct AspellConfig * ths, const char * key);

/* returns null on error */
ASPELL_API const char * aspell_config_retrieve(struct AspellConfig * ths, const char * key);

ASPELL_API int aspell_config_retrieve_list(struct AspellConfig * ths, const char * key, struct AspellMutableContainer * lst);

/* return -1 on error, 0 if false, 1 if true */
ASPELL_API int aspell_config_retrieve_bool(struct AspellConfig * ths, const char * key);

/* return -1 on error */
ASPELL_API int aspell_config_retrieve_int(struct AspellConfig * ths, const char * key);



/******************************** error ********************************/


struct AspellError {

  const char * mesg;

  const struct AspellErrorInfo * err;

};


typedef struct AspellError AspellError;

ASPELL_API int aspell_error_is_a(const struct AspellError * ths, const struct AspellErrorInfo * e);


struct AspellErrorInfo {

  const struct AspellErrorInfo * isa;

  const char * mesg;

  unsigned int num_parms;

  const char * parms[3];

};


typedef struct AspellErrorInfo AspellErrorInfo;


/**************************** can have error ****************************/


typedef struct AspellCanHaveError AspellCanHaveError;


ASPELL_API unsigned int aspell_error_number(const struct AspellCanHaveError * ths);

ASPELL_API const char * aspell_error_message(const struct AspellCanHaveError * ths);

ASPELL_API const struct AspellError * aspell_error(const struct AspellCanHaveError * ths);

ASPELL_API void delete_aspell_can_have_error(struct AspellCanHaveError * ths);



/******************************** errors ********************************/


extern const struct AspellErrorInfo * const aerror_other;
extern const struct AspellErrorInfo * const aerror_operation_not_supported;
extern const struct AspellErrorInfo * const   aerror_cant_copy;
extern const struct AspellErrorInfo * const aerror_file;
extern const struct AspellErrorInfo * const   aerror_cant_open_file;
extern const struct AspellErrorInfo * const     aerror_cant_read_file;
extern const struct AspellErrorInfo * const     aerror_cant_write_file;
extern const struct AspellErrorInfo * const   aerror_invalid_name;
extern const struct AspellErrorInfo * const   aerror_bad_file_format;
extern const struct AspellErrorInfo * const aerror_dir;
extern const struct AspellErrorInfo * const   aerror_cant_read_dir;
extern const struct AspellErrorInfo * const aerror_config;
extern const struct AspellErrorInfo * const   aerror_unknown_key;
extern const struct AspellErrorInfo * const   aerror_cant_change_value;
extern const struct AspellErrorInfo * const   aerror_bad_key;
extern const struct AspellErrorInfo * const   aerror_bad_value;
extern const struct AspellErrorInfo * const   aerror_duplicate;
extern const struct AspellErrorInfo * const aerror_language_related;
extern const struct AspellErrorInfo * const   aerror_unknown_language;
extern const struct AspellErrorInfo * const   aerror_unknown_soundslike;
extern const struct AspellErrorInfo * const   aerror_language_not_supported;
extern const struct AspellErrorInfo * const   aerror_no_wordlist_for_lang;
extern const struct AspellErrorInfo * const   aerror_mismatched_language;
extern const struct AspellErrorInfo * const aerror_encoding;
extern const struct AspellErrorInfo * const   aerror_unknown_encoding;
extern const struct AspellErrorInfo * const   aerror_encoding_not_supported;
extern const struct AspellErrorInfo * const   aerror_conversion_not_supported;
extern const struct AspellErrorInfo * const aerror_pipe;
extern const struct AspellErrorInfo * const   aerror_cant_create_pipe;
extern const struct AspellErrorInfo * const   aerror_process_died;
extern const struct AspellErrorInfo * const aerror_bad_input;
extern const struct AspellErrorInfo * const   aerror_invalid_word;
extern const struct AspellErrorInfo * const   aerror_word_list_flags;
extern const struct AspellErrorInfo * const     aerror_invalid_flag;
extern const struct AspellErrorInfo * const     aerror_conflicting_flags;


/******************************* speller *******************************/


typedef struct AspellSpeller AspellSpeller;


ASPELL_API struct AspellCanHaveError * new_aspell_speller(struct AspellConfig * config);

ASPELL_API struct AspellSpeller * to_aspell_speller(struct AspellCanHaveError * obj);

ASPELL_API void delete_aspell_speller(struct AspellSpeller * ths);

ASPELL_API unsigned int aspell_speller_error_number(const struct AspellSpeller * ths);

ASPELL_API const char * aspell_speller_error_message(const struct AspellSpeller * ths);

ASPELL_API const struct AspellError * aspell_speller_error(const struct AspellSpeller * ths);

ASPELL_API struct AspellConfig * aspell_speller_config(struct AspellSpeller * ths);

/* returns  0 if it is not in the dictionary,
 * 1 if it is, or -1 on error. */
ASPELL_API int aspell_speller_check(struct AspellSpeller * ths, const char * word, int word_size);

ASPELL_API int aspell_speller_add_to_personal(struct AspellSpeller * ths, const char * word, int word_size);

ASPELL_API int aspell_speller_add_to_session(struct AspellSpeller * ths, const char * word, int word_size);

ASPELL_API const struct AspellWordList * aspell_speller_personal_word_list(struct AspellSpeller * ths);

ASPELL_API const struct AspellWordList * aspell_speller_session_word_list(struct AspellSpeller * ths);

ASPELL_API const struct AspellWordList * aspell_speller_main_word_list(struct AspellSpeller * ths);

ASPELL_API int aspell_speller_save_all_word_lists(struct AspellSpeller * ths);

ASPELL_API int aspell_speller_clear_session(struct AspellSpeller * ths);

/* Return null on error.
 * the word list returned by suggest is only valid until the next
 * call to suggest */
ASPELL_API const struct AspellWordList * aspell_speller_suggest(struct AspellSpeller * ths, const char * word, int word_size);

ASPELL_API int aspell_speller_store_replacement(struct AspellSpeller * ths, const char * mis, int mis_size, const char * cor, int cor_size);



/******************************** filter ********************************/


typedef struct AspellFilter AspellFilter;


ASPELL_API void delete_aspell_filter(struct AspellFilter * ths);

ASPELL_API unsigned int aspell_filter_error_number(const struct AspellFilter * ths);

ASPELL_API const char * aspell_filter_error_message(const struct AspellFilter * ths);

ASPELL_API const struct AspellError * aspell_filter_error(const struct AspellFilter * ths);

ASPELL_API struct AspellFilter * to_aspell_filter(struct AspellCanHaveError * obj);



/*************************** document checker ***************************/


struct AspellToken {

  unsigned int offset;

  unsigned int len;

};


typedef struct AspellToken AspellToken;


typedef struct AspellDocumentChecker AspellDocumentChecker;


ASPELL_API void delete_aspell_document_checker(struct AspellDocumentChecker * ths);

ASPELL_API unsigned int aspell_document_checker_error_number(const struct AspellDocumentChecker * ths);

ASPELL_API const char * aspell_document_checker_error_message(const struct AspellDocumentChecker * ths);

ASPELL_API const struct AspellError * aspell_document_checker_error(const struct AspellDocumentChecker * ths);

/* Creates a new document checker.
 * The speller class is expect to last until this
 * class is destroyed.
 * If config is given it will be used to overwide
 * any relevent options set by this speller class.
 * The config class is not once this function is done.
 * If filter is given then it will take ownership of
 * the filter class and use it to do the filtering.
 * You are expected to free the checker when done. */
ASPELL_API struct AspellCanHaveError * new_aspell_document_checker(struct AspellSpeller * speller);

ASPELL_API struct AspellDocumentChecker * to_aspell_document_checker(struct AspellCanHaveError * obj);

/* reset the internal state of the filter.
 * should be called whenever a new document is being filtered */
ASPELL_API void aspell_document_checker_reset(struct AspellDocumentChecker * ths);

/* process a string
 * The string passed in should only be split on white space
 * characters.  Furthermore, between calles to reset, each string
 * should be passed in exactly once and in the order they appeared
 * in the document.  Passing in stings out of order, skipping
 * strings or passing them in more than once may lead to undefined
 * results. */
ASPELL_API void aspell_document_checker_process(struct AspellDocumentChecker * ths, const char * str, int size);

/* returns the next misspelled word in the processed string
 * if there are no more misspelled word than token.word
 * will be null and token.size will be 0 */
ASPELL_API struct AspellToken aspell_document_checker_next_misspelling(struct AspellDocumentChecker * ths);

/* returns the underlying filter class */
ASPELL_API struct AspellFilter * aspell_document_checker_filter(struct AspellDocumentChecker * ths);



/****************************** word list ******************************/


typedef struct AspellWordList AspellWordList;


ASPELL_API int aspell_word_list_empty(const struct AspellWordList * ths);

ASPELL_API unsigned int aspell_word_list_size(const struct AspellWordList * ths);

ASPELL_API struct AspellStringEnumeration * aspell_word_list_elements(const struct AspellWordList * ths);



/************************** string enumeration **************************/


typedef struct AspellStringEnumeration AspellStringEnumeration;


ASPELL_API void delete_aspell_string_enumeration(struct AspellStringEnumeration * ths);

ASPELL_API struct AspellStringEnumeration * aspell_string_enumeration_clone(const struct AspellStringEnumeration * ths);

ASPELL_API void aspell_string_enumeration_assign(struct AspellStringEnumeration * ths, const struct AspellStringEnumeration * other);

ASPELL_API int aspell_string_enumeration_at_end(const struct AspellStringEnumeration * ths);

ASPELL_API const char * aspell_string_enumeration_next(struct AspellStringEnumeration * ths);



/********************************* info *********************************/


struct AspellModuleInfo {

  const char * name;

  double order_num;

  const char * lib_dir;

  struct AspellStringList * dict_dirs;

  struct AspellStringList * dict_exts;

};


typedef struct AspellModuleInfo AspellModuleInfo;


struct AspellDictInfo {

  /* name to identify the dictionary by */
  const char * name;

  const char * code;

  const char * jargon;

  int size;

  const char * size_str;

  struct AspellModuleInfo * module;

};


typedef struct AspellDictInfo AspellDictInfo;


typedef struct AspellModuleInfoList AspellModuleInfoList;


ASPELL_API struct AspellModuleInfoList * get_aspell_module_info_list(struct AspellConfig * config);

ASPELL_API int aspell_module_info_list_empty(const struct AspellModuleInfoList * ths);

ASPELL_API unsigned int aspell_module_info_list_size(const struct AspellModuleInfoList * ths);

ASPELL_API struct AspellModuleInfoEnumeration * aspell_module_info_list_elements(const struct AspellModuleInfoList * ths);



typedef struct AspellDictInfoList AspellDictInfoList;


ASPELL_API struct AspellDictInfoList * get_aspell_dict_info_list(struct AspellConfig * config);

ASPELL_API int aspell_dict_info_list_empty(const struct AspellDictInfoList * ths);

ASPELL_API unsigned int aspell_dict_info_list_size(const struct AspellDictInfoList * ths);

ASPELL_API struct AspellDictInfoEnumeration * aspell_dict_info_list_elements(const struct AspellDictInfoList * ths);



typedef struct AspellModuleInfoEnumeration AspellModuleInfoEnumeration;


ASPELL_API int aspell_module_info_enumeration_at_end(const struct AspellModuleInfoEnumeration * ths);

ASPELL_API const struct AspellModuleInfo * aspell_module_info_enumeration_next(struct AspellModuleInfoEnumeration * ths);

ASPELL_API void delete_aspell_module_info_enumeration(struct AspellModuleInfoEnumeration * ths);

ASPELL_API struct AspellModuleInfoEnumeration * aspell_module_info_enumeration_clone(const struct AspellModuleInfoEnumeration * ths);

ASPELL_API void aspell_module_info_enumeration_assign(struct AspellModuleInfoEnumeration * ths, const struct AspellModuleInfoEnumeration * other);



typedef struct AspellDictInfoEnumeration AspellDictInfoEnumeration;


ASPELL_API int aspell_dict_info_enumeration_at_end(const struct AspellDictInfoEnumeration * ths);

ASPELL_API const struct AspellDictInfo * aspell_dict_info_enumeration_next(struct AspellDictInfoEnumeration * ths);

ASPELL_API void delete_aspell_dict_info_enumeration(struct AspellDictInfoEnumeration * ths);

ASPELL_API struct AspellDictInfoEnumeration * aspell_dict_info_enumeration_clone(const struct AspellDictInfoEnumeration * ths);

ASPELL_API void aspell_dict_info_enumeration_assign(struct AspellDictInfoEnumeration * ths, const struct AspellDictInfoEnumeration * other);



/***************************** string list *****************************/


typedef struct AspellStringList AspellStringList;


ASPELL_API struct AspellStringList * new_aspell_string_list();

ASPELL_API int aspell_string_list_empty(const struct AspellStringList * ths);

ASPELL_API unsigned int aspell_string_list_size(const struct AspellStringList * ths);

ASPELL_API struct AspellStringEnumeration * aspell_string_list_elements(const struct AspellStringList * ths);

ASPELL_API int aspell_string_list_add(struct AspellStringList * ths, const char * to_add);

ASPELL_API int aspell_string_list_remove(struct AspellStringList * ths, const char * to_rem);

ASPELL_API void aspell_string_list_clear(struct AspellStringList * ths);

ASPELL_API struct AspellMutableContainer * aspell_string_list_to_mutable_container(struct AspellStringList * ths);

ASPELL_API void delete_aspell_string_list(struct AspellStringList * ths);

ASPELL_API struct AspellStringList * aspell_string_list_clone(const struct AspellStringList * ths);

ASPELL_API void aspell_string_list_assign(struct AspellStringList * ths, const struct AspellStringList * other);



/****************************** string map ******************************/


typedef struct AspellStringMap AspellStringMap;


ASPELL_API struct AspellStringMap * new_aspell_string_map();

ASPELL_API int aspell_string_map_add(struct AspellStringMap * ths, const char * to_add);

ASPELL_API int aspell_string_map_remove(struct AspellStringMap * ths, const char * to_rem);

ASPELL_API void aspell_string_map_clear(struct AspellStringMap * ths);

ASPELL_API struct AspellMutableContainer * aspell_string_map_to_mutable_container(struct AspellStringMap * ths);

ASPELL_API void delete_aspell_string_map(struct AspellStringMap * ths);

ASPELL_API struct AspellStringMap * aspell_string_map_clone(const struct AspellStringMap * ths);

ASPELL_API void aspell_string_map_assign(struct AspellStringMap * ths, const struct AspellStringMap * other);

ASPELL_API int aspell_string_map_empty(const struct AspellStringMap * ths);

ASPELL_API unsigned int aspell_string_map_size(const struct AspellStringMap * ths);

ASPELL_API struct AspellStringPairEnumeration * aspell_string_map_elements(const struct AspellStringMap * ths);

/* Insert a new element.
 * Will NOT overright an existing entry.
 * Returns false if the element already exists. */
ASPELL_API int aspell_string_map_insert(struct AspellStringMap * ths, const char * key, const char * value);

/* Insert a new element.
 * Will overright an existing entry.
 * Always returns true. */
ASPELL_API int aspell_string_map_replace(struct AspellStringMap * ths, const char * key, const char * value);

/* Looks up an element.
 * Returns null if the element did not exist.
 * Returns an empty string if the element exists but has a null value.
 * Otherwises returns the value */
ASPELL_API const char * aspell_string_map_lookup(const struct AspellStringMap * ths, const char * key);



/***************************** string pair *****************************/


struct AspellStringPair {

  const char * first;

  const char * second;

};


typedef struct AspellStringPair AspellStringPair;


/*********************** string pair enumeration ***********************/


typedef struct AspellStringPairEnumeration AspellStringPairEnumeration;


ASPELL_API int aspell_string_pair_enumeration_at_end(const struct AspellStringPairEnumeration * ths);

ASPELL_API struct AspellStringPair aspell_string_pair_enumeration_next(struct AspellStringPairEnumeration * ths);

ASPELL_API void delete_aspell_string_pair_enumeration(struct AspellStringPairEnumeration * ths);

ASPELL_API struct AspellStringPairEnumeration * aspell_string_pair_enumeration_clone(const struct AspellStringPairEnumeration * ths);

ASPELL_API void aspell_string_pair_enumeration_assign(struct AspellStringPairEnumeration * ths, const struct AspellStringPairEnumeration * other);



#ifdef __cplusplus
}
#endif
#endif /* ASPELL_ASPELL__H */
