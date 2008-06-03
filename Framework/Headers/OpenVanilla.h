// OpenVanilla.h: The Framework
//
// Copyright (c) 2004-2007 The OpenVanilla Project (http://openvanilla.org)
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of OpenVanilla nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/*!
	@header OpenVanilla
	@abstract The OpenVanilla Framework
	@copyright OpenVanilla.org
	@charset utf-8
*/

#ifndef __OpenVanilla_h
#define __OpenVanilla_h

/*!
	@define	OV_VERSION
	@abstract The version of the OpenVanilla framework, for instance,
	0x00080000 represents version 0.8.0 (00_08_00_00).
	@discussion All Input Method modules and Filter moudles are based
	on the OpenVanilla framework, and if the loader and the modules are
	not based on the same version of the OpenVanilla framework, thet
	might be incompatible. That's why we need an identifier to tell which
	version of the OpenVanilla framework that the loader or a module is
	based on, and ask the loader to load the modules only based on
	the same version of the OpenVanilla framework.
*/
#define OV_VERSION   (0x00080000)      // version 0.8.0 (00_08_00_00)


/*! 
	@class OVBase
	@abstract A pure base class that defines the virtual destructor
*/

class OVBase {
public:
/*!
	@method OVBase()
	@abstract The constructor of the OVBase class.
*/	
    virtual ~OVBase() {}
};

/*! 
	@class OVKeyCode
	@abstract An abstract interface that accesses key code and key
	states (e.g. if SHIFT is hit at the time the key code is sent)
	@discussion What an Input Method software does is to receive the
	informations about which keys that users typed, and then send 
	representive strings or characters to the current application.
	The OVKeyCode object represents the information of keyboard
	events, you can know which key is pressed down by the code()
	function and which function keys such as Shift and Control by
	other functions.
	
*/
class OVKeyCode : public OVBase  {
public:
/*!
	@method code
	@abstract The keycode of the pressed key sent to your input
	method software.
	@return	The keycode.
*/	
    virtual int code() = 0;
/*!
	@method isShift
	@abstract If the Shift key is pressed down.
	@result If the Shift key is pressred down, it returns 1, else 
	returns 0.
*/
    virtual int isShift() = 0;
/*!
	@method isCapslock
	@abstract If the Capslock is on.
	@result If the Capslock is on, it returns 1, else returns 0.
*/
    virtual int isCapslock() = 0;

/*!
	@method isCtrl
	@abstract If the Control key is pressed down.
	@result If the Control key is pressed down, it returns 1, else 
	returns 0.
*/
    virtual int isCtrl() = 0;
/*!
	@method isAlt
	@abstract If the Alt key is pressed down.
	@result If the Alt key is pressed down, it returns 1, else 
	returns 0.
*/
    virtual int isAlt() = 0;
/*!
	@method isOpt
	@abstract If the Option key is pressed down.
	@discussion Only Mac OS supprts the Option key, so if you need to
	implement your input method to other platforms such as Windows
	and *nix, use isAlt() instead. 
	@result If the Option key is pressed down, it returns 1, else 
	returns 0.	
*/
    virtual int isOpt() { return isAlt(); }
/*!
	@method isCommand
	@abstract If the Command key is pressed down.
	@discussion Only Mac OS supprts the Command key, so if you need to
	implement your input method to other platforms such as Windows
	and *nix, use isAlt() or isCtrl() instead. 
	@result If the Option key is pressed down, it returns 1, else 
	returns 0.	
*/
    virtual int isCommand() { return isAlt(); }
/*!
	@method isNum
	@abstract If the Number Lock is on.
	@result If the Number Lock is on, it returns 1, else 
	returns 0.
*/
    virtual int isNum() = 0;
/*!
	@method isFunctionKey
	@abstract If any function key is pressed, the function keys
	include the Control key, the Alt key, the Option key, and the
	Number lock.
	@discussion In most cases, the Input Method software only handle the
	keyboard events when these function keys are not pressed, because the
	combination of a function key and any other key often represents an
	application hotkey or system hotkey, and you should let the applications
	to take over the control of the keyboard events.
	@result If any function key is pressed, it returns 1, else 
	returns 0.
*/
    virtual int isFunctionKey() {
        return isCtrl() || isAlt() || isOpt() || isCommand() || isNum();
    }

};

/*! 
	@class OVBuffer
	@abstract An abstract interface for the pre-edit and composing buffer.
*/
class OVBuffer : public OVBase {
public:
/*! 
	@method clear
	@abstract Clears the composing buffer.
*/	
    virtual OVBuffer* clear() = 0;
/*! 
	@method append
	@abstract Appends characters to the end of the characters of the current
	composing buffer.
	@param s The characters to append in UTF-8 encoding.
	@discussion You should use the append() function to set the content of 
	the buffer. If you want to set the content of the buffer with a new 
	array of characters, use clear() first and then use the append() function.
	Note, since the buffer text is changed, you need to call the update()
	function to update the user interface and then users are able to see
	the changes.
*/	
    virtual OVBuffer* append(const char *s) = 0;
/*! 
	@method send
	@abstract Commits the characters into the current application.
	@discussion The composing buffer is only a pre-edit area of the Input 
	Method software, and you must stop the pre-edit progress and then send
	the characters to current application, and the task of inputting
	characters is then finally done.
	If you want to finish th pre-edit progress, you have two choices: 
	if the key-combinitions that users typed are wrong, you may
	call clear() to reset the input buffer text, otherwise, you shall
	call send() function to commit the characters and clear or change the 
	composing buffer in the same time.
*/
    virtual OVBuffer* send() = 0;
/*! 
	@method update
	@abstract Updates the display of the composing buffer.
	@discussion After changing the content of the composing buffer, you
	need to update the user interface to let the users be able to see the
	changes.
*/
    virtual OVBuffer* update() = 0;
/*! 
	@method update
	@abstract Updates the display of the composing buffer with extra 
	informations.
	@discussion
	@param cursorPos The position of the cursor(caret), it should be minimal
	0 and maximal as the length of composing text.
	@param markFrom The begin of the mark.
	@param markTo The end of the mark.
*/
    virtual OVBuffer* update(int cursorPos, int markFrom = -1, int markTo = -1) = 0;
/*! 
	@method isEmpty
	@abstract If the the composing text is empty.
	@result	If the composing text is empty, it returns 1, else returns 0.
*/
    virtual int isEmpty() = 0 ;
};

/*!
	@class OVCandidate
	@abstract An abstract interface for the message bar (called Candidate
	in OpenVanilla).
	@discussion By default it is a simple horizontal strip, but it is 
	possible, for example, to send marked-up texts through 
	OVCandidate::append(). The implementation is up to the IM environment
	(Loader) that provides this interface to the IM.
*/
class OVCandidate : public OVBase  {
public:
/*! 
	@method clear
	@abstract Clears the content of the Candidates list.
*/	
    virtual OVCandidate* clear() = 0;
/*! 
	@method append
	@abstract Appends characters to the end of the characters of the current
	content of the Candidates list window.
	@param s The characters to append in UTF-8 encoding.
	@discussion You should use the append() function to set the content of 
	the Candidates list . If you want to set the content with a new 
	array of characters, use clear() first and then use the append() function.
	Note, since the buffer text is changed, you need to call the update()
	function to update the user interface and then users are able to see
	the changes, and do not forget to call show() to show the window.
*/
    virtual OVCandidate* append(const char *s) = 0;
/*! 
	@method hide
	@abstract Asks the Candidate list window to hide.
	@seealso show	
*/
    virtual OVCandidate* hide() = 0;
/*! 
	@method show
	@abstract Asks the Candidate list window to show.
	@seealso hide	
*/
    virtual OVCandidate* show() = 0;
/*! 
	@method update
	@abstract Updates the display of the Candidates list
	@discussion After changing the content of the Candidates list, you
	need to update the user interface to let the users be able to see the
	changes.
*/
    virtual OVCandidate* update() = 0;
/*! 
	@method onScreen
	@abstract If the Candidates list window is shown.
	@discussion You can use hide() or show() to hide and show the
	Candidates list window.
	@result If the Candidates list window is shown, it returns 1, else
	it returns 0.
*/
    virtual int onScreen()=0;
};

/*!
	@class OVDictionary
	@abstract An abstract interface for a simple dictionary. 
	@discussion It is recommended that this dictionary be implemented as
	a type-free dictionary, i.e. you can replace the key with any value 
	of any type, and type conversion between integer and string is done
	transparently, like what is done in e.g. sqlite3.
*/
class OVDictionary : public OVBase {
public:
/*! 
	@method keyExist
	@abstract If a configuration key exists in the dictionary.
	@param key The configuration key.
	@result If the configuration key exists. it returns 1, else it 
	returns 0.
*/	
    virtual int keyExist(const char *key) = 0;
/*! 
	@method getInteger
	@abstract Retrieve the integer value of a configuration key from
	the dictionary.
	@param key The configuration key.
	@result The integer value of the configuration key.
*/
    virtual int getInteger(const char *key) = 0;
/*! 
	@method setInteger
	@abstract Set the integer value of a configuration key to the
	dictionary.
	@param key The configuration key.
	@param value The integer value to set.
*/
    virtual int setInteger(const char *key, int value) = 0;
/*! 
	@method getString
	@abstract Retrieve the string value of a configuration key from
	the dictionary.
	@param key The configuration key.
	@result The string value of the configuration key.
*/
    virtual const char* getString(const char *key) = 0;
/*! 
	@method setString
	@abstract Set the string value of a configuration key to the
	dictionary.
	@param key The configuration key.
	@param value The string value to set.
	@seealso setString
	@seealso getStringWithDefault
*/
    virtual const char* setString(const char *key, const char *value) = 0;
/*! 
	@method getIntegerWithDefault
	@abstract Try to retrieve the integer value of a configuration key
	from the dictionary, if the configuration key does not exist, use the
	default value.
	@param key The configuration key.
	@param value The default integer value.
	@result The integer value of the configuration key.	
*/
    virtual int getIntegerWithDefault(const char *key, int value) {
        if (!keyExist(key)) setInteger(key, value); return getInteger(key);
    }
/*! 
	@method getStringWithDefault
	@abstract Try to retrieve the string value of a configuration key
	from the dictionary, if the configuration key does not exist, use the
	default value.
	@param key The configuration key.
	@param value The default string value.
	@result The string value of the configuration key.
	@seealso getString
	@seealso setString
*/
    virtual const char* getStringWithDefault(const char *key, const char *value) 
    {
        if (!keyExist(key)) setString(key, value); return getString(key);
    }
};

/*!
	@class OVService
	@abstract The basical service provided by the OpenVanilla loader.
	@discussion OVService is a helper class. It provides some functions
	to help the modules to interact with user interface such as making
	alert sound or showing notify messages. It provides text conversion
	functions, too.
*/
class OVService : public OVBase {
public:
/*! 
	@method beep
	@abstract Asks the loader to make an alert sound.
*/	
    virtual void beep() = 0;
/*! 
	@method notify
	@abstract Asks the loader to show a notify window with a notify message.
	@param msg The message to show.
*/
    virtual void notify(const char *msg) = 0;
/*! 
	@method locale
	@abstract  The locale of current user's environment. 
	@discussion It might be "en" if the user is now under the English
	locale. The Traditional Chinese locale might be "zh_TW" or "zh-Hant",
	and the Simplified Chinese locale might be "zh_CN" or "zh-Hans".
	@return The identifier of the current locale.
*/
    virtual const char *locale() = 0 ;
/*!
	@method userSpacePath
	@abstract The full-path of the user space for an OpenVanilla module.
	@param modid The identifier of a OpenVanilla module.
	@return The full-path.
*/
    virtual const char *userSpacePath(const char *modid) = 0;
/*! 
	@method pathSeparator
	@abstract The path separator character.
	@discussion On Mac OS X, Unix or any other Unix-like operating system,
	the path separator is a slash(/) as the path separato is a backslash (\)
	on Microsoft Windows. 
	Since OpenVanilla is a cross-platform framework, we need to use
	different path separator character separately on each platform. 
	@return The path separator character.
*/
    virtual const char *pathSeparator() = 0;

    // limited support for encoding conversion
/*! 
	@method toUTF8
	@abstract Converts a text from other encodings to UTF-8 encoding.
	@param encoding Currently supported char encoding = "big5" 
	(short for big5-hkscs)
	@param src The source text.
	@return The converted text.
*/
    virtual const char *toUTF8(const char *encoding, const char *src) = 0;
/*! 
	@method fromUTF8
	@abstract Converts a text from UTF-8 encoding to other encodings.
	@param encoding Currently supported char encoding = "big5" 
	(short for big5-hkscs)
	@param src The source text.
	@return The converted text.
*/
    virtual const char *fromUTF8(const char *encoding, const char *src) = 0;
/*! 
	@method UTF16ToUTF8
	@abstract Converts a text from UTF-16 to UTF-8 encodings.
	@discussion the endian order of these two functions is always that 
	of machine order (ie. on a big endian machine it's UTF16BE, 
	otherwise UTF16LE) no byte order mark (0xfffe or 0xfeff) is ever
	used here)  also note no boundry check mechanism exists, the Loader
	takes care  about it
	@param src The source text.
	@param len The length of the text.
	@return The converted text.
*/
    virtual const char *UTF16ToUTF8(unsigned short *src, int len) = 0;
/*! 
	@method UTF8ToUTF16
	@abstract Converts a text from UTF-8 to UTF-16 encodings.
	@discussion the endian order of these two functions is always that 
	of machine order (ie. on a big endian machine it's UTF16BE, 
	otherwise UTF16LE) no byte order mark (0xfffe or 0xfeff) is ever
	used here)  also note no boundry check mechanism exists, the Loader
	takes care  about it
	@param src The source text.
	@return The converted text.
*/
    virtual int UTF8ToUTF16(const char *src, unsigned short **rcvr) = 0;
};

/*!
	@class OVInputMethodContext
	@abstract The input method context. 
	@discussion The context is always created by the IM module
	(defined below) and is used exclusively by the IM environment (Loader).
	This is where your implementation for an input method goes.
	Note that the derived OVInputMethodContext::keyEvent() method MUST RETURN
	either 0 or non-zero. It should return non-zero if the event is "processed"
	by the IM context, 0 if the event is "rejected" or "not processed."
*/
class OVInputMethodContext : public OVBase  {
public:
/*!
	@method start
	@abstract Handles the event of starting a session.
*/	
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {}
/*!
	@method clear
	@abstract Handles the event of clearing a session.
*/	
    virtual void clear() {}
/*!
	@method end
	@abstract Handles the event of ending a session.
	@discussion It is not rare that you need to hide the Candidate list
	window or reset the composing buffer when an input session is end.
	You may want to write some data to files or databases in this time, too.
*/
    virtual void end() {}
/*!
	@method keyEvent
	@abstract Handles the keyboard events.
	@discussion The keyEvent function is the core of an Input Method module.
	Tt is called every time when a key is pressed down, and you should decide
	what to do to react with this key by the implementation of the keyEvent
	function. You can use the OVBuffer object instance to control the 
	composing buffer or commit characters, use the OVCandudate object to
	decide the content of the Candidate list window and the window should be
	shown or not. You can also use the OVSerive to do text conversion, or
	make alert sound or show notify messages.
	@return It should return non-zero if the event is "processed"
	by the IM context, 0 if the event is "rejected" or "not processed."
*/
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*) = 0;
};

/*!
	@class OVModule
	@abstract The common OpenVanilla module interface.
	@discussion Currently OpenVanilla has two kinds of modules, 
	InputMethod (that creates IM context and handles IM configuration 
	issues) and OutputFilter (that does any magical transformation/conversion
	of output buffer, for example, conversion between Traditional and 
	Simplified Chinese, or convert all kanas into romanized forms, etc.)
*/
class OVModule : public OVBase {
public:
/*!
	@method moduleType
	@abstract The type of a OpenVanilla module.
	@discussion There are some varied typed of OpenVanilla modules. So we
	need an identifier to tell if a module is an Input Method or an Output
	Filter. If you were working with the OVInputMethod subclass, the 
	moduleType would be "OVInputMethod", while it would be "OVOutputFilter"
	while you were working with the OVOutputFilter subclass.
	@return The type of the module.
*/
    virtual const char *moduleType() = 0;
/*!
	@method identifier
	@abstract The inner identifier of a OpenVanilla module.
	@discussion Each OpenVanilla module should have an identical identifier.
	The loader will use this identifier to load the settings from the 
	configuration file and any other required data for the specified module.
	You must implement this function in your module.
	@return The identifier. 
*/
    virtual const char *identifier() = 0;
/*!
	@method localizedName
	@abstract The localized name of a OpenVanilla module.
	@param locale The locale of current user's environment. It might be "en"
	if the user is now under the English locale. The Traditional Chinese
	locale might be "zh_TW" or "zh-Hant", and the Simplified Chinese locale
	might be "zh_CN" or "zh-Hans". If you did not implement this function
	in your module, the looader will use the identifier instead.
	@return The localized name.
*/
    virtual const char *localizedName(const char* locale) { return identifier(); }
/*!
	@method initialize
	@abstract To initialize the module.
	@discussion For example, to load settings from the configurations
	dictionary, to load extra required data and so on.
	@param moduleCfg The configuration dictionary.
	@param src The shared service.
	@param modulePath The full-path where the binary of the module is
	located.
*/
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) { return 1; }
/*!
	@method update
	@abstract To update the configuration dictionary and write it into
	the disk drive.
	@param moduleCfg The configuration dictionary.
	@param src The shared service.
*/
    virtual void update(OVDictionary *moduleCfg, OVService *srv) {}
};

/*!
	@class OVInputMethod
	@abstract The InputMethod module interface
*/
class OVInputMethod : public OVModule
{
public:
/*!
	@method newContext
	@abstract The input method context.
	@return The input method context.
*/	
    virtual OVInputMethodContext* newContext() = 0;
/*!
	@method moduleType
	@abstract The type of OVInputMethod
	@discussion There are some varied typed of OpenVanilla modules. So we
	need an identifier to tell if a module is an Input Method module.
	@return The "OVInputMethod" characters.
*/
    virtual const char* moduleType() { return "OVInputMethod"; }
};

/*!
	@class OVOutputFilter
	@abstract The OutputFilter module interface. 
	@discussion  Note we don't care in which encoding the source is given.
	It is recommended that the source be in UTF-8 and that the ouput be the
	same. Also note we don't care about the length of the source, nor do we
	care about the length of the output. See the "lazy design" principle in
	the beginning of this header file. One more thing: it should be possible
	for the OutputFilter to return the source as output.
*/
class OVOutputFilter : public OVModule
{
public:
/*!
	@method process
	@abstract To proccess the output filtering.
	@discussion Implementation of the function is required in your
	Output Filter module.
	@param source The source text to filter.
	@param srv The shared OpenVanilla service.	
	@return The processed text.
*/	
    virtual const char* process(const char *source, OVService *srv) = 0;
/*!
	@method moduleType
	@abstract The type of OVOutputFilter
	@dicsussion There are some varied typed of OpenVanilla modules. So we
	need an identifier to tell if a module is an Output Filter module.	
	@return The "OVOutputFilter" characters.
*/
    virtual const char* moduleType() { return "OVOutputFilter"; }
};

/*!
	@enum Common key codes. 
	@discussion Note these are FIXED DEFINITIONS in OpenVanilla.h; an
	IM environment should map different key codes into these when offering
	OVKeyCode-derived objects to an IM context

	@constant ovkEsc The keycode of the Escape key.
	@constant ovkSpace	The keycode of the Space key.
	@constant ovkReturn	The keycode of the Return key.
	@constant ovkDelete The keycode of the Delete key.
	@constant ovkBackspace The keycode of the BackSpace key.
	@constant ovkUp The keycode of the Up arrow key.
	@constant ovkDown The keycode of the Down arrow key.
	@constant ovkLeft The keycode of the Left arrow key.
	@constant ovkRight The keycode of the Right arrow key.
	@constant ovkHome The keycode of the Home key.
	@constant ovkEnd The keycode of the End key.
	@constant ovkPageUp The keycode of the Page Up key.
	@constant ovkPageDown The keycode of the Page Down key.
	@constant ovkTab The keycode of the Tab key.	
*/ 
enum 
{
    ovkEsc=27, ovkSpace=32, ovkReturn=13, 
    ovkDelete=127, ovkBackspace=8,
    ovkUp=30, ovkDown=31, ovkLeft=28, ovkRight=29,
    ovkHome=1, ovkEnd=4, ovkPageUp=11, ovkPageDown=12,
    ovkTab=9
};

#endif
