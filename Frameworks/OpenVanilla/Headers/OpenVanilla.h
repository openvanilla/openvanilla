// OpenVanilla.h: The Framework
//
// Copyright (c) 2004-2009 The OpenVanilla Project (http://openvanilla.org)
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
	@file OpenVanilla.h
	@framework OpenVanilla Framework
	@copyright OpenVanilla.org
	@abstract OpenVanilla is a collection of popular Chinese, Taiwanese,
	Japanese and symbol input methods and language tools, and there are already 
	OpenVanilla installation package for Mac OS X, Windows and Linux platform.
	
	However, in the point of view of developers, OpenVanilla is not only an
	Inpur Method software, but a cross-platform Input Method/Text Proccessing
	framework.
	
	The OpenVanilla framework composites two main parts. One is the core
	loader, while another part is the collection of modules (or so-called 
	plugins). The core loader's work is to communicate with the system Input
	Method APIs, such as the IME (Input Method Editor) architecture on Microsoft
	Windows platform, TSM (Text Service Manager) and IMK (Input Method Kit)
	architectures on Mac OS X, and so on. Another work of the core loader
	is to load the module, the modules are dynamic loadable binary files.
	
	In other words, OpenVanilla provides an abstract interface between the
	Input Method engines and the system Input Method APIs. By the implementation
	of an OpenVnailla Inpue Method module, you need not to rewrite your codes
	for varied platforms just because the system APis are varied.
	
	The core of the OpenVanilla framework is just a concept, a header file,
	or just several simple C++ classes which define the basic but flexible 
	properties that an Input Method should have, the events it should handle
	and the tools it could use.
	
	To work with your own Input Method module, just implemet the OVInputMethod 
	class by providing an unique identifier and localized name, and creating
	an Input Method context (the OVInputMethodContext class) to handle keyboard
	events. The keyEvent method is the function to proccess/translate the
	keyboard events to corresponding text to output to the current application.
	The keyboard events are defined in the OVKeyCode class.
	 
	There are two main tools that an Input Method module could use while
	proccessing the keyboard events, they are the pre-edit buffer (OVBuffer)
	and the candidate list window (OVCandidate). If your Input Method needs to
	use several keys as components to compose a character, like most Chinese
	and Japanese Input Methods and so on, you can use the pre-edit buffer to
	display the components by calling the append() method to append a component
	to current content of the buffer, and call the send() method to send the
	content of the buffer to the current application.
	
	If there are two or more results matching to the combination of the
	components, you may ask users to choose one of these results. In this case,
	you should use the candidate list window. You can decide the content of
	the window, and ask it to be shown and hidden.
	
	If your module offers extra settings which allow users to customize the
	behaviours, your can read the settings from the configuration file
	in the initialize() method in your module. The settings of your module
	is restored in the OVDictionary object which contains varied string and
	integer values. You can use the update() method to write the settings.
	
	You can also obtain the file path of your module in the initialize()
	method. That means, if your Input Method need to load extra resources
	such as a database file and so on, you can specify the path of your database
	file when the loader is calling the initialize() method of modules.
	
	The OVService class also supports some simple tools that your module
	can use, for example, if you need to make an alert sound when the users input
	wrong keys, you can call the beep() method. And if you want to show some
	messages to notify the users, just call the notify() method.
	
	The OpenVanilla Framework also has a kind of module which is called Output
	Filter. Unlike an Input Method module which handles the keyboard events,
	an Output Filter module haneles the output text of an Input Method module.
	For example, when a user typed a character in Traditional Chinese,
	he or she can use an Output Filter module to convert the character to
	Simplified Chinese. So he or she can use his or her favorite Traditional
	Chinese Input Method to type Simplified Chinese without learning any
	Simplified Chinese Input Methods.
	
	Most methods in an Ouput Filter module are the same as the methods in an
	Input Method module. The difference between an Output Filter and an Input
	Method modules is, an Output Filter has no Input Method context, and you 
	need to implement the process() method. You can obtain the source text
	to convert in this method, and just return the converted result.
*/

#ifndef __OpenVanilla_h
#define __OpenVanilla_h

/*!
	@define	OV_VERSION
	@abstract The version of the OpenVanilla framework, for instance,
	0x00080000 represents version 0.8.0 (00_08_00_00).
	@discussion 
	All Input Method modules and Filter moudles are based
	on the OpenVanilla framework, and if the loader and the modules are
	not based on the same version of the OpenVanilla framework, thet
	might be incompatible. That's why we need an identifier to tell which
	version of the OpenVanilla framework that the loader or a module is
	based on, and ask the loader to load the modules only based on
	the same version of the OpenVanilla framework.
	
	If the version is updated, you need to re-compile all modules.
*/
#define OV_VERSION   (0x00080000)      // version 0.8.0 (00_08_00_00)


/*! 
	@class OVBase
	@abstract A pure base class that defines the virtual destructor
	@discussion 
	All classes in the OpenVanilla framework are based
	on OVBase class. It provides nothing but a simple destructor
	to make it easy to destruct an object.
*/

class OVBase {
public:
/*!
	@function ~OVBase
	@abstract The destructor of an OVBase object.
*/	
    virtual ~OVBase() {}
};

/*! 
	@class OVKeyCode
	@abstract An abstract interface that accesses key code and key
	states (e.g. if SHIFT is hit at the time the key code is sent)
	@discussion 
	What an Input Method software does is to receive the
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
	@function code
	@abstract The keycode of the pressed key sent to your input
	method software. For example, if it returns 'c', it means the user
	pressed on the 'c' key on his/her keyboard.
	@return	The keycode.
*/	
    virtual int code() = 0;
/*!
	@function isShift
	@abstract If the shift key is pressed down.
	@result If the shift key is pressred down, it returns true, else 
	returns false.
*/
    virtual bool isShift() = 0;
/*!
	@function isCapslock
	@abstract If the capslock is on.
	@result If the capslock is on, it returns true, else returns false.
*/
    virtual bool isCapslock() = 0;

/*!
	@function isCtrl
	@abstract If the control key is pressed down.
	@result If the control key is pressed down, it returns true, else 
	returns false.
*/
    virtual bool isCtrl() = 0;
/*!
	@function isAlt
	@abstract If the alt key is pressed down.
	@result If the alt key is pressed down, it returns true, else 
	returns false.
*/
    virtual bool isAlt() = 0;
/*!
	@function isOpt
	@abstract If the option key is pressed down.
	@discussion 
	Only Mac OS supprts the option key, so if you need to
	implement your input method to other platforms such as Windows
	and *nix, use isAlt() instead. 
	@result If the option key is pressed down, it returns true, else 
	returns false.
	@seealso isAlt isAlt	
*/
    virtual bool isOpt() { return isAlt(); }
/*!
	@function isCommand
	@abstract If the command key is pressed down.
	@discussion 
	Only Mac OS supprts the command key, so if you need to
	implement your input method to other platforms such as Windows
	and *nix, use isAlt() or isCtrl() instead.
	@result If the Command key is pressed down, it returns true, else 
	returns false.
	@seealso isAlt isAlt
*/
    virtual bool isCommand() { return isAlt(); }
/*!
	@function isNum
	@abstract If the number lock is on.
	@result If the number lock is on, it returns true, else 
	returns false.
*/
    virtual bool isNum() = 0;
/*!
	@function isFunctionKey
	@abstract If any function key is pressed, the function keys
	include the Control key, the Alt key, the Option key, and the
	Number lock.
	@discussion 
	In most cases, the Input Method software only handle the
	keyboard events when these function keys are not pressed, because the
	combination of a function key and any other key often represents an
	application hotkey or system hotkey, and you should let the applications
	to take over the control of the keyboard events.
	@result If any function key is pressed, it returns true, else 
	returns false.
	@seealso isCtrl isCtrl
	@seealso isAlt isAlt
	@seealso isOpt isOpt
	@seealso isCommand isCommand
	@seealso isNum isNum
*/
    virtual bool isFunctionKey() {
        return isCtrl() || isAlt() || isOpt() || isCommand() || isNum();
    }

};

/*! 
	@class OVBuffer
	@abstract An abstract interface for the pre-edit and composing buffer.
	@discusson
	Most Input Methods require a pre-edit and composing area. That means,
	If your Input Method needs to use several keys as components to compose
	a character, like most Chinese and Japanese Input Methods and so on, you
	can use the pre-edit buffer to display the components by calling the
	append() method to append a component to current content of the buffer,
	and call the send() method to send the content of the buffer to the
	current application.	
*/
class OVBuffer : public OVBase {
public:
/*! 
	@function clear
	@abstract Clears the composing buffer.
	@return The OVBuffer object.
*/	
    virtual OVBuffer* clear() = 0;
/*! 
	@function append
	@abstract Appends characters to the end of the characters of the current
	composing buffer.
	@param s The characters to append in UTF-8 encoding.
	@discussion 
	You should use the append() function to set the content of 
	the buffer. If you want to set the content of the buffer with a new 
	array of characters, use clear() first and then use the append() function.
	Note, since the buffer text is changed, you need to call the update()
	function to update the user interface and then users are able to see
	the changes.
	@return The OVBuffer object.	
*/	
    virtual OVBuffer* append(const char *s) = 0;
/*! 
	@function send
	@abstract Commits the characters into the current application.
	@discussion 
	The composing buffer is only a pre-edit area of the Input 
	Method software, and you must stop the pre-edit progress and then send
	the characters to current application, and the task of inputting
	characters is then finally done.
	
	If you want to finish th pre-edit progress, you have two choices: 
	if the key-combinitions that users typed are wrong, you may
	call clear() to reset the input buffer text, otherwise, you shall
	call send() function to commit the characters and clear or change the 
	composing buffer in the same time.
	@return The OVBuffer object.	
*/
    virtual OVBuffer* send() = 0;
/*! 
	@function update
	@abstract Updates the display of the composing buffer.
	@discussion 
	After changing the content of the composing buffer, you
	need to update the user interface to let the users be able to see the
	changes.
	@return The OVBuffer object.
*/
    virtual OVBuffer* update() = 0;
/*! 
	@function update
	@abstract Updates the display of the composing buffer with extra 
	informations.
	@discussion
	Sometimes you may need to update not only the content of the composing
	buffer but also the position of the cusor(caret) within the buffer, you
	may need to add an extra underline under some characters. For example, 
	you are working with a smart Pingyin or Phonetic Chinese Input Method.
	Instead of simply calling a update() function, you can update the
	composing buffer in this way.
	@param cursorPos The position of the cursor(caret). It should be minimal
	0 and maximal as the length of composing text.
	@param markFrom The begin of the mark.  It should be minimal
	0 and maximal as the length of composing text.
	@param markTo The end of the mark.  It should be minimal
	as the value of markFrom and maximal as the length of composing text.
	@return The OVBuffer object.
*/
    virtual OVBuffer* update(int cursorPos, int markFrom = -1, int markTo = -1) = 0;
/*! 
	@function isEmpty
	@abstract If the the composing text is empty.
	@result	If the composing text is empty, it returns true, else 
	returns false.
*/
    virtual bool isEmpty() = 0;
};

/*!
	@class OVCandidate
	@abstract An abstract interface for the message bar (called Candidate
	in OpenVanilla).
	@discussion 
	By default it is a simple horizontal strip, but it is 
	possible, for example, to send marked-up texts through 
	OVCandidate::append(). The implementation is up to the IM environment
	(Loader) that provides this interface to the IM.
*/
class OVCandidate : public OVBase  {
public:
/*! 
	@function clear
	@abstract Clears the content of the Candidates list.
	@return The OVCandidate object.
*/	
    virtual OVCandidate* clear() = 0;
/*! 
	@function append
	@abstract Appends characters to the end of the characters of the current
	content of the Candidates list window.
	@param s The characters to append in UTF-8 encoding.
	@discussion 
	You should use the append() function to set the content of 
	the Candidates list . If you want to set the content with a new 
	array of characters, use clear() first and then use the append() function.
	Note, since the buffer text is changed, you need to call the update()
	function to update the user interface and then users are able to see
	the changes, and do not forget to call show() to show the window.
	@return The OVCandidate object.
*/
    virtual OVCandidate* append(const char *s) = 0;
/*! 
	@function hide
	@abstract Asks the Candidate list window to hide.
	@return The OVCandidate object.	
	@seealso show show	
*/
    virtual OVCandidate* hide() = 0;
/*! 
	@function show
	@abstract Asks the Candidate list window to show.
	@return The OVCandidate object.
	@seealso hide hide
*/
    virtual OVCandidate* show() = 0;
/*! 
	@function update
	@abstract Updates the display of the Candidates list
	@discussion 
	After changing the content of the Candidates list, you
	need to update the user interface to let the users be able to see the
	changes.
	@return The OVCandidate object.	
*/
    virtual OVCandidate* update() = 0;
/*! 
	@function onScreen
	@abstract If the Candidates list window is shown.
	@discussion 
	You can use hide() or show() to hide and show the
	Candidates list window.
	@result If the Candidates list window is shown, it returns true, else
	it returns false.
*/
    virtual bool onScreen() = 0;
};

/*!
	@class OVDictionary
	@abstract An abstract interface for a simple dictionary. 
	@discussion 
	It is recommended that this dictionary be implemented as
	a type-free dictionary, i.e. you can replace the key with any value 
	of any type, and type conversion between integer and string is done
	transparently, like what is done in e.g. sqlite3.
*/
class OVDictionary : public OVBase {
public:
/*! 
	@function keyExist
	@abstract If a configuration key exists in the dictionary.
	@param key The configuration key.
	@result If the configuration key exists. it returns true, else it 
	returns false.
*/	
    virtual bool keyExist(const char *key) = 0;
/*! 
	@function getInteger
	@abstract Retrieve the integer value of a configuration key from
	the dictionary.
	@param key The configuration key.
	@result The integer value of the configuration key.
*/
    virtual int getInteger(const char *key) = 0;
/*! 
	@function setInteger
	@abstract Set the integer value of a configuration key to the
	dictionary.
	@param key The configuration key.
	@param value The integer value to set.
*/
    virtual int setInteger(const char *key, int value) = 0;
/*! 
	@function getString
	@abstract Retrieve the string value of a configuration key from
	the dictionary.
	@param key The configuration key.
	@result The string value of the configuration key.
*/
    virtual const char *getString(const char *key) = 0;
/*! 
	@function setString
	@abstract Set the string value of a configuration key to the
	dictionary.
	@param key The configuration key.
	@param value The string value to set.
	@seealso setString
	@seealso getStringWithDefault
*/
    virtual const char *setString(const char *key, const char *value) = 0;
/*! 
	@function getIntegerWithDefault
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
	@function getStringWithDefault
	@abstract Try to retrieve the string value of a configuration key
	from the dictionary, if the configuration key does not exist, use the
	default value.
	@param key The configuration key.
	@param value The default string value.
	@result The string value of the configuration key.
	@seealso getString
	@seealso setString
*/
    virtual const char *getStringWithDefault(const char *key, const char *value) 
    {
        if (!keyExist(key)) setString(key, value); return getString(key);
    }
};

/*!
	@class OVService
	@abstract The basical service provided by the OpenVanilla loader.
	@discussion 
	OVService is a helper class. It provides some functions
	to help the modules to interact with user interface such as making
	alert sound or showing notify messages. It provides text conversion
	functions, too.
*/
class OVService : public OVBase {
public:
/*! 
	@function beep
	@abstract Asks the loader to make an alert sound.
*/	
    virtual void beep() = 0;
/*! 
	@function notify
	@abstract Asks the loader to show a notify window with a notify message.
	@param msg The message to show.
*/
    virtual void notify(const char *msg) = 0;
/*! 
	@function locale
	@abstract  The locale of current user's environment. 
	@discussion 
	It might be "en" if the user is now under the English
	locale. The Traditional Chinese locale might be "zh_TW" or "zh-Hant",
	and the Simplified Chinese locale might be "zh_CN" or "zh-Hans".
	@return The identifier of the current locale.
*/
    virtual const char *locale() = 0 ;
/*!
	@function userSpacePath
	@abstract The full-path of the user space for an OpenVanilla module.
	@param modid The identifier of a OpenVanilla module.
	@return The full-path.
*/
    virtual const char *userSpacePath(const char *modid) = 0;
/*! 
	@function pathSeparator
	@abstract The path separator character.
	@discussion 
	On Mac OS X, Unix or any other Unix-like operating system,
	the path separator is a slash(/) as the path separato is a backslash (\)
	on Microsoft Windows. 
	Since OpenVanilla is a cross-platform framework, we need to use
	different path separator character separately on each platform. 
	@return The path separator character.
*/
    virtual const char *pathSeparator() = 0;

    // limited support for encoding conversion
/*! 
	@function toUTF8
	@abstract Converts a text from other encodings to UTF-8 encoding.
	@param encoding Currently supported char encoding = "big5" 
	(short for big5-hkscs)
	@param src The source text.
	@return The converted text.
*/
    virtual const char *toUTF8(const char *encoding, const char *src) = 0;
/*! 
	@function fromUTF8
	@abstract Converts a text from UTF-8 encoding to other encodings.
	@param encoding Currently supported char encoding = "big5" 
	(short for big5-hkscs)
	@param src The source text.
	@return The converted text.
*/
    virtual const char *fromUTF8(const char *encoding, const char *src) = 0;
/*! 
	@function UTF16ToUTF8
	@abstract Converts a text from UTF-16 to UTF-8 encodings.
	@discussion 
	the endian order of these two functions is always that 
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
	@function UTF8ToUTF16
	@abstract Converts a text from UTF-8 to UTF-16 encodings.
	@discussion 
	the endian order of these two functions is always that 
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
	@discussion 
	The context is always created by the IM module
	(defined below) and is used exclusively by the IM environment (Loader).
	This is where your implementation for an input method goes.
	Note that the derived OVInputMethodContext::keyEvent() method MUST RETURN
	either 0 or non-zero. It should return non-zero if the event is "processed"
	by the IM context, 0 if the event is "rejected" or "not processed."
*/
class OVInputMethodContext : public OVBase  {
public:
/*!
	@function start
	@abstract Handles the event of starting a session.
	@param buffer The shared composing buffer object.
	@param candidate The shared Candidate list object.
	@param service The shared OpenVanilla service object.	
*/	
    virtual void start(OVBuffer*, OVCandidate*, OVService*) {}
/*!
	@function clear
	@abstract Handles the event of clearing a session.
*/	
    virtual void clear() {}
/*!
	@function end
	@abstract Handles the event of ending a session.
	@discussion 
	It is not rare that you need to hide the Candidate list
	window or reset the composing buffer when an input session is end.
	You may want to write some data to files or databases in this time, too.
*/
    virtual void end() {}
/*!
	@function keyEvent
	@abstract Handles the keyboard events.
	@discussion 
	The keyEvent function is the core of an Input Method module.
	Tt is called every time when a key is pressed down, and you should decide
	what to do to react with this key by the implementation of the keyEvent
	function. You can use the OVBuffer object instance to control the 
	composing buffer or commit characters, use the OVCandudate object to
	decide the content of the Candidate list window and the window should be
	shown or not. You can also use the OVSerive to do text conversion, or
	make alert sound or show notify messages.
	@param keyCode The recieved key code.
	@param buffer The shared composing buffer object.
	@param candidate The shared Candidate list object.
	@param service The shared OpenVanilla service object.
	@return It should return non-zero if the event is "processed"
	by the IM context, 0 if the event is "rejected" or "not processed."
*/
    virtual int keyEvent(OVKeyCode*, OVBuffer*, OVCandidate*, OVService*) = 0;
};

/*!
	@class OVModule
	@abstract The common OpenVanilla module interface.
	@discussion 
	Currently OpenVanilla has two kinds of modules, 
	InputMethod (that creates IM context and handles IM configuration 
	issues) and OutputFilter (that does any magical transformation/conversion
	of output buffer, for example, conversion between Traditional and 
	Simplified Chinese, or convert all kanas into romanized forms, etc.)
*/
class OVModule : public OVBase {
public:
/*!
	@function moduleType
	@abstract The type of a OpenVanilla module.
	@discussion 
	There are some varied typed of OpenVanilla modules. So we
	need an identifier to tell if a module is an Input Method or an Output
	Filter. If you were working with the OVInputMethod subclass, the 
	moduleType would be "OVInputMethod", while it would be "OVOutputFilter"
	while you were working with the OVOutputFilter subclass.
	@return The type of the module.
*/
    virtual const char *moduleType() = 0;
/*!
	@function identifier
	@abstract The inner unique identifier of a OpenVanilla module.
	@discussion 
	Each OpenVanilla module should have an identical identifier.
	The loader will use this identifier to load the settings from the 
	configuration file and any other required data for the specified module.
	You must implement this function in your module.
	@return The identifier. 
*/
    virtual const char *identifier() = 0;
/*!
	@function localizedName
	@abstract The localized name of a OpenVanilla module.
	@param locale The locale of current user's environment. It might be "en"
	if the user is now under the English locale. The Traditional Chinese
	locale might be "zh_TW" or "zh-Hant", and the Simplified Chinese locale
	might be "zh_CN" or "zh-Hans". If you did not implement this function
	in your module, the looader will use the identifier instead.
	@return The localized name.
*/
    virtual const char *localizedName(const char *locale) { return identifier(); }
/*!
	@function initialize
	@abstract To initialize the module.
	@discussion 
	For example, to load settings from the configurations
	dictionary, to load extra required data and so on.
	@param moduleCfg The configuration dictionary.
	@param srv The shared service.
	@param modulePath The full-path where the binary of the module is
	located.
*/
	virtual int initialize(OVDictionary* moduleCfg, OVService* srv, 
        const char *modulePath) { return 1; }
/*!
	@function update
	@abstract To update the configuration dictionary and write it into
	the disk drive.
	@param moduleCfg The configuration dictionary.
	@param srv The shared service object.
*/
    virtual void update(OVDictionary* moduleCfg, OVService* srv) {}
};

/*!
	@class OVInputMethod
	@abstract The InputMethod module interface
	@discussion
	OVInputMethod is a subclass of OVModule. If you want to start 
	working with your own Input Method module, you should work with
	your own OVInputmethod subclass and OVInputMethodContext subclass.
*/
class OVInputMethod : public OVModule
{
public:
/*!
	@function newContext
	@abstract The input method context.
	@return The input method context.
*/	
    virtual OVInputMethodContext* newContext() = 0;
/*!
	@function moduleType
	@abstract The type of OVInputMethod
	@discussion 
	There are some varied typed of OpenVanilla modules. So we
	need an identifier to tell if a module is an Input Method module.
	@return The "OVInputMethod" characters.
*/
    virtual const char *moduleType() { return "OVInputMethod"; }
};

/*!
	@class OVOutputFilter
	@abstract The Output Filter module interface. 
	@discussion 
	
	The Output Filter is a special module type of OpenVanilla, which haneles
	the output text of an Input Method module. For example, when a user typed
	a character in Traditional Chinese, he or she can use an Output Filter
	module to convert the character to Simplified Chinese. So he or she can
	use his or her favorite Traditional Chinese Input Method to type Simplified
	Chinese without learning any Simplified Chinese Input Methods.
	
	Most methods in an Ouput Filter module are the same as the methods in an
	Input Method module. The difference between an Output Filter and an Input
	Method modules is, an Output Filter has no Input Method context, and you 
	need to implement the process() method. You can obtain the source text
	to convert in this method, and just return the converted result.
	
	OVOutputFilter is a subclass of OVModule. If you want to start 
	working with your own Output Filter module, you should work with
	your own OVOutputFilter subclass.
*/
class OVOutputFilter : public OVModule
{
public:
/*!
	@function process
	@abstract To proccess the output filtering.
	@discussion
	Note we don't care in which encoding the source is given.
	It is recommended that the source be in UTF-8 and that the ouput be the
	same. Also note we don't care about the length of the source, nor do we
	care about the length of the output. See the "lazy design" principle in
	the beginning of this header file. One more thing: it should be possible
	for the OutputFilter to return the source as output
	
	Implementation of the function is required in your Output Filter module.
	
	@param source The source text to filter.
	@param srv The shared OpenVanilla service.	
	@return The processed text.
*/	
    virtual const char *process(const char *source, OVService* srv) = 0;
/*!
	@function moduleType
	@abstract The type of OVOutputFilter.
	@discussion 
	There are some varied typed of OpenVanilla modules. So we
	need an identifier to tell the loader if a module is an Output Filter
	module or not.
	@return The "OVOutputFilter" characters.
*/
    virtual const char *moduleType() { return "OVOutputFilter"; }
};

/*!
	@enum Common key codes. 
	@discussion 
	Note these are FIXED DEFINITIONS in OpenVanilla.h; an
	IM environment should map different key codes into these when offering
	OVKeyCode-derived objects to an IM context

	@constant ovkEsc The keycode of the escape key.
	@constant ovkSpace The keycode of the space key.
	@constant ovkReturn	The keycode of the return key.
	@constant ovkDelete The keycode of the delete key.
	@constant ovkBackspace The keycode of the backSpace key.
	@constant ovkUp The keycode of the up arrow key.
	@constant ovkDown The keycode of the down arrow key.
	@constant ovkLeft The keycode of the left arrow key.
	@constant ovkRight The keycode of the right arrow key.
	@constant ovkHome The keycode of the home key.
	@constant ovkEnd The keycode of the end key.
	@constant ovkPageUp The keycode of the page Up key.
	@constant ovkPageDown The keycode of the page Down key.
	@constant ovkTab The keycode of the tab key.	
*/ 
enum 
{
    ovkEsc =27, ovkSpace = 32, ovkReturn = 13,
    ovkDelete = 127, ovkBackspace = 8,
    ovkUp = 30, ovkDown = 31, ovkLeft = 28, ovkRight = 29,
    ovkHome = 1, ovkEnd = 4, ovkPageUp = 11, ovkPageDown = 12,
    ovkTab = 9
};

#endif
