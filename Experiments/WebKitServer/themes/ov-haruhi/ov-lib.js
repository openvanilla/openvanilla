//// JSAN.js
/*

*/

var JSAN = function () { JSAN.addRepository(arguments) }

JSAN.VERSION = 0.10;

/*

*/

JSAN.globalScope   = self;
JSAN.includePath   = ['.', 'lib'];
JSAN.errorLevel    = "none";
JSAN.errorMessage  = "";
JSAN.loaded        = {};

/*

*/

JSAN.use = function () {
    var classdef = JSAN.require(arguments[0]);
    if (!classdef) return null;

    var importList = JSAN._parseUseArgs.apply(JSAN, arguments).importList;
    JSAN.exporter(classdef, importList);

    return classdef;
}

/*

*/

JSAN.require = function (pkg) {
    var path = JSAN._convertPackageToPath(pkg);
    if (JSAN.loaded[path]) {
        return JSAN.loaded[path];
    }

    try {
        var classdef = eval(pkg);
        if (typeof classdef != 'undefined') return classdef;
    } catch (e) { /* nice try, eh? */ }


    for (var i = 0; i < JSAN.includePath.length; i++) {
        var js;
        try{
            var url = JSAN._convertPathToUrl(path, JSAN.includePath[i]);
                js  = JSAN._loadJSFromUrl(url);
        } catch (e) {
            if (i == JSAN.includePath.length - 1) throw e;
        }
        if (js != null) {
            var classdef = JSAN._createScript(js, pkg);
            JSAN.loaded[path] = classdef;
            return classdef;
        }
    }
    return false;

}

/*

*/

JSAN.exporter = function () {
    JSAN._exportItems.apply(JSAN, arguments);
}

/*

*/

JSAN.addRepository = function () {
    var temp = JSAN._flatten( arguments );
    // Need to go in reverse to do something as simple as unshift( @foo, @_ );
    for ( var i = temp.length - 1; i >= 0; i-- )
        JSAN.includePath.unshift(temp[i]);
    return JSAN;
}

JSAN._flatten = function( list1 ) {
    var list2 = new Array();
    for ( var i = 0; i < list1.length; i++ ) {
        if ( typeof list1[i] == 'object' ) {
            list2 = JSAN._flatten( list1[i], list2 );
        }
        else {
            list2.push( list1[i] );
        }
    }
    return list2;
};

JSAN._findMyPath = function () {
    if (document) {
        var scripts = document.getElementsByTagName('script');
        for ( var i = 0; i < scripts.length; i++ ) {
            var src = scripts[i].getAttribute('src');
            if (src) {
                var inc = src.match(/^(.*?)\/?JSAN.js/);
                if (inc && inc[1]) {
                    var repo = inc[1];
                    for (var j = 0; j < JSAN.includePath.length; j++) {
                        if (JSAN.includePath[j] == repo) {
                            return;
                        }
                    }
                    JSAN.addRepository(repo);
                }
            }
        }
    }
}
JSAN._findMyPath();

JSAN._convertPathToUrl = function (path, repository) {
    return repository.concat('/' + path);
};
    

JSAN._convertPackageToPath = function (pkg) {
    var path = pkg.replace(/\./g, '/');
        path = path.concat('.js');
    return path;
}

JSAN._parseUseArgs = function () {
    var pkg        = arguments[0];
    var importList = [];

    for (var i = 1; i < arguments.length; i++)
        importList.push(arguments[i]);

    return {
        pkg:        pkg,
        importList: importList
    }
}

JSAN._loadJSFromUrl = function (url) {
    return new JSAN.Request().getText(url);
}

JSAN._findExportInList = function (list, request) {
    if (list == null) return false;
    for (var i = 0; i < list.length; i++)
        if (list[i] == request)
            return true;
    return false;
}

JSAN._findExportInTag = function (tags, request) {
    if (tags == null) return [];
    for (var i in tags)
        if (i == request)
            return tags[i];
    return [];
}

JSAN._exportItems = function (classdef, importList) {
    var exportList  = new Array();
    var EXPORT      = classdef.EXPORT;
    var EXPORT_OK   = classdef.EXPORT_OK;
    var EXPORT_TAGS = classdef.EXPORT_TAGS;
    
    if (importList.length > 0) {
       importList = JSAN._flatten( importList );

       for (var i = 0; i < importList.length; i++) {
            var request = importList[i];
            if (   JSAN._findExportInList(EXPORT,    request)
                || JSAN._findExportInList(EXPORT_OK, request)) {
                exportList.push(request);
                continue;
            }
            var list = JSAN._findExportInTag(EXPORT_TAGS, request);
            for (var i = 0; i < list.length; i++) {
                exportList.push(list[i]);
            }
        }
    } else {
        exportList = EXPORT;
    }
    JSAN._exportList(classdef, exportList);
}

JSAN._exportList = function (classdef, exportList) {
    if (typeof(exportList) != 'object') return null;
    for (var i = 0; i < exportList.length; i++) {
        var name = exportList[i];

        if (JSAN.globalScope[name] == null)
            JSAN.globalScope[name] = classdef[name];
    }
}

JSAN._makeNamespace = function(js, pkg) {
    var spaces = pkg.split('.');
    var parent = JSAN.globalScope;
    eval(js);
    var classdef = eval(pkg);
    for (var i = 0; i < spaces.length; i++) {
        var name = spaces[i];
        if (i == spaces.length - 1) {
            if (typeof parent[name] == 'undefined') {
                parent[name] = classdef;
                if ( typeof classdef['prototype'] != 'undefined' ) {
                    parent[name].prototype = classdef.prototype;
                }
            }
        } else {
            if (parent[name] == undefined) {
                parent[name] = {};
            }
        }

        parent = parent[name];
    }
    return classdef;
}

JSAN._handleError = function (msg, level) {
    if (!level) level = JSAN.errorLevel;
    JSAN.errorMessage = msg;

    switch (level) {
        case "none":
            break;
        case "warn":
            alert(msg);
            break;
        case "die":
        default:
            throw new Error(msg);
            break;
    }
}

JSAN._createScript = function (js, pkg) {
    try {
        return JSAN._makeNamespace(js, pkg);
    } catch (e) {
        JSAN._handleError("Could not create namespace[" + pkg + "]: " + e);
    }
    return null;
}


JSAN.prototype = {
    use: function () { JSAN.use.apply(JSAN, arguments) }
};


// Low-Level HTTP Request
JSAN.Request = function (jsan) {
    if (JSAN.globalScope.XMLHttpRequest) {
        this._req = new XMLHttpRequest();
    } else {
        this._req = new ActiveXObject("Microsoft.XMLHTTP");
    }
}

JSAN.Request.prototype = {
    _req:  null,
    
    getText: function (url) {
        this._req.open("GET", url, false);
        try {
            this._req.send(null);
            if (this._req.status == 200 || this._req.status == 0)
                return this._req.responseText;
        } catch (e) {
            JSAN._handleError("File not found: " + url);
            return null;
        };

        JSAN._handleError("File not found: " + url);
        return null;
    }
};

/*

*/

//// HTTP.Query
// # $Id: Kinetic.pm 1493 2005-04-07 19:20:18Z theory $

// Set up namespace.
if (typeof HTTP == 'undefined') HTTP = {};
HTTP.Query = function (qry) {
    this.query = [];
    // Do nothing with empty query strings.
    // Use the current browser document location search if there is no query.
    if (!arguments.length && typeof window != 'undefined')
        qry = window.document.location.search;
    if (qry == null || qry == '') return;
    var pairs = qry.substring(1).split(/[;&]/);
    for (var i = 0; i < pairs.length; i++) {
        var parts = pairs[i].split('=');
        if (parts[0] == null) continue;
        var key = unescape(parts[0]), val = unescape(parts[1]);
        if (this.query[key] == null) {
            this.query[key] = unescape(val);
        } else {
            if (typeof this.query[key] == 'string') {
                this.query[key] = [this.query[key], unescape(val)];
            } else this.query[key].push(unescape(val));
        }
    }
};

HTTP.Query.VERSION = '0.03';

HTTP.Query.prototype = {
    get:   function (key)      { return this.query[key] },
    set:   function (key, val) { this.query[key] = val },
    unset: function (key)      { delete this.query[key] },
    clear: function ()         { this.query = [] },

    add:   function (key, val) {
        if (this.query[key] != null) {
            if (typeof this.query[key] != 'string') this.query[key].push(val);
            else this.query[key] = [this.query[key], val];
        } else {
            this.query[key] = val;
        }
    },
    act:    function (fn) {
        for (var k in this.query) {
            if (typeof this.query[k] == 'string') {
                if (!fn(k, this.query[k])) return;
            } else {
                // XXX What if someone has changed Array.prototype or
                // Object.prototype??
                for (var i in this.query[k])
                    if (!fn(k, this.query[k][i])) return;
            }
        }
    },
    toString: function (sep) {
        var ret = '';
        if (sep == null) sep = ';';
        this.act(function (k, v) {
             ret += sep + escape(k) + '=' + escape(v);
             return true;
        });
        return ret.replace(/^[;&]/, '?');
    }
};

//// HTTP.Request
/*

*/

if ( typeof( Method ) == "undefined" ) {
    Method = {};
}

if ( typeof( Method["bind"] ) == "undefined" ) {
    Method.bind = function ( method, object ) {
        return function() {
            method.apply(object, arguments);
        }
    };
}

/*

*/

if ( typeof( HTTP ) == "undefined" ) {
    HTTP = {};
}

if ( typeof( HTTP.Request ) == "undefined" ) {
    HTTP.Request = function ( options ) {
        if ( !options ) options = {};

        this.options = {};
        for ( var i in options ) {
            this.setOption( i, options[i] );
        }

        if ( this.getOption( "method" ) == undefined ) {
            this.setOption( "method", "post" );
        }

        if ( this.getOption( "asynchronous" ) == undefined ) {
            this.setOption( "asynchronous", true );
        }

        if ( this.getOption( "parameters" ) == undefined ) {
            this.setOption( "parameters", "" );
        }

        if ( this.getOption( "transport" ) == undefined ) {
            this.setOption( "transport", HTTP.Request.Transport );
        }

        if ( this.getOption( "uri" ) )
            this.request();
    };

    HTTP.Request.EventNames = [
        "uninitialized"
       ,"loading"
       ,"loaded"
       ,"interactive"
       ,"complete"
    ];

/*

*/

    HTTP.Request.prototype.getOption = function( name ) {
        if ( typeof( name ) != "string" ) {
            return;
        }
        return this.options[name.toLowerCase()];
    };

/*

*/

    HTTP.Request.prototype.setOption = function( name, value ) {
        if ( typeof( name ) != "string" ) {
            return;
        }

        name = name.toLowerCase();

        this.options[name] = value;

        if ( name == "method" ) {
            if ( ! ( this.options.method == "get" || this.options.method == "post" || this.options.method == "head" ) ) {
                this.options.method = "post";
            }
        }

        if ( name == "transport" ) {
            if ( typeof( value ) != "function" ) {
                this.options.transport = HTTP.Request.Transport;
            }
        }
    };

/*

*/

    HTTP.Request.prototype.request = function ( uri ) {
        if ( ! uri ) uri = this.getOption( "uri" );
        if ( ! uri ) return;

        var parameters = this.getOption( "parameters" );
        // XXX Why?
        if (parameters.length > 0) parameters += "&_=";

        var method = this.getOption( "method" );
        if ( method == "get" ) {
            uri += "?" + parameters;
        }

        this.transport = new (this.getOption( "transport" ))();

        var async = this.getOption( "asynchronous" );
        this.transport.open( method ,uri ,async );

        if ( async ) {
            this.transport.onreadystatechange = Method.bind(
                this.onStateChange, this
            );

            setTimeout(
                Method.bind(
                    function() { this.respondToReadyState(1) }
                   ,this
                )
               ,10
           );
        }

        this.setRequestHeaders();

        if ( method == "post" ) {
            var body = this.getOption( "postbody" );
            if ( ! body ) body = parameters;

            this.transport.send( body );
        }
        else {
            this.transport.send( null );
        }
    };

/*

*/

    HTTP.Request.prototype.setRequestHeaders = function() {
        this.transport.setRequestHeader( "X-Requested-With", "HTTP.Request" );
        this.transport.setRequestHeader( "X-HTTP-Request-Version", HTTP.Request.VERSION );

        if (this.getOption( "method" ) == "post") {
            this.transport.setRequestHeader( "Content-type", "application/x-www-form-urlencoded" );

            /* Force "Connection: close" for Mozilla browsers to work around
             * a bug where XMLHttpReqeuest sends an incorrect Content-length
             * header. See Mozilla Bugzilla #246651.
             */
            if (this.transport.overrideMimeType) {
                this.transport.setRequestHeader( "Connection", "close" );
            }
        }

/* TODO Add support for this back in later
        if (this.options.requestHeaders)
            requestHeaders.push.apply(requestHeaders, this.options.requestHeaders);
*/
    };

/*

*/

    // XXX This confuses me a little ... how are undefined and 0 considered a success?
    HTTP.Request.prototype.isSuccess = function () {
        return this.transport.status == undefined
            || this.transport.status == 0
            || (this.transport.status >= 200 && this.transport.status < 300);
    };

    HTTP.Request.prototype.onStateChange = function() {
        var readyState = this.transport.readyState;
        if (readyState != 1) {
            this.respondToReadyState( this.transport.readyState );
        }
    };

    HTTP.Request.prototype.respondToReadyState = function( readyState ) {
        var event = HTTP.Request.EventNames[readyState];

        if (event == "complete") {
            var func = this.getOption( "on" + this.transport.status );
            if ( ! func ) {
                if ( this.isSuccess() ) {
                    func = this.getOption( "onsuccess" );
                }
                else {
                    func = this.getOption( "onfailure" );
                }
            }

            if ( func ) {
                ( func )( this.transport );
            }
        }

        if ( this.getOption( "on" + event ) )
            ( this.getOption( "on" + event ) )( this.transport );

        /* Avoid memory leak in MSIE: clean up the oncomplete event handler */
        if (event == "complete") {
            this.transport.onreadystatechange = function (){};
        }
    };

    HTTP.Request.VERSION = 0.03;
}

/*

*/

if ( typeof( HTTP.Request.Transport ) == "undefined" ) {
    if ( window.XMLHttpRequest ) {
        HTTP.Request.Transport = window.XMLHttpRequest;
    }
    // This tests for ActiveXObject in IE5+
    else if ( window.ActiveXObject && window.clipboardData ) {
        var msxmls = new Array(
            "Msxml2.XMLHTTP.5.0"
           ,"Msxml2.XMLHTTP.4.0"
           ,"Msxml2.XMLHTTP.3.0"
           ,"Msxml2.XMLHTTP"
           ,"Microsoft.XMLHTTP"
        );
        for ( var i = 0; i < msxmls.length; i++ ) {
            try {
                new ActiveXObject(msxmls[i]);
                HTTP.Request.Transport = function () {
                    return new ActiveXObject(msxmls[i]);
                };
                break;
            }
            catch(e) {
            }
        }
    }

    if ( typeof( HTTP.Request.Transport ) == "undefined" ) {
        // This is where we add DIV/IFRAME support masquerading as an XMLHttpRequest object
    }

    if ( typeof( HTTP.Request.Transport ) == "undefined" ) {
        throw new Error("Unable to locate XMLHttpRequest or other HTTP transport mechanism");
    }
}

/*

*/

//// DOM.Ready
if ( typeof DOM == "undefined" ) DOM = {};

DOM.Ready = {};

DOM.Ready.VERSION = '0.13';

DOM.Ready.finalTimeout = 15;
DOM.Ready.timerInterval = 50;
DOM.Ready._timePassed = 0;

DOM.Ready._isReady = 0;

DOM.Ready._onDOMReadyCallbacks = [];

DOM.Ready._onIdReadyCallbacks = {};

DOM.Ready._checkDOM = function () {
    if ( DOM.Ready._isReady ) return DOM.Ready._isReady;

    if (    typeof document.getElementsByTagName != 'undefined'
         && typeof document.getElementById != 'undefined' 
         && ( document.getElementsByTagName('body')[0] != null
              || document.body != null ) ) {

        DOM.Ready._isReady = 1;
    }

    return DOM.Ready._isReady;

};

DOM.Ready.onDOMReady = function (callback) {
    if ( DOM.Ready._checkDOM() ) {
        callback();
    }
    else {
        DOM.Ready._onDOMReadyCallbacks.push(callback);
    }
}

DOM.Ready.onIdReady = function ( id, callback ) {
    if ( DOM.Ready._checkDOM() ) {
        var elt = document.getElementById(id);
        if (elt) {
            callback(elt);
            return;
        }
    }

    var callback_array = DOM.Ready._onIdReadyCallbacks[id];
    if ( ! callback_array ) {
        callback_array = [];
    }
    callback_array.push(callback);

    DOM.Ready._onIdReadyCallbacks[id] = callback_array;
}

DOM.Ready._runDOMCallbacks = function () {
    for ( var i = 0; i < DOM.Ready._onDOMReadyCallbacks.length; i++ ) {
        DOM.Ready._onDOMReadyCallbacks[i]();
    }

    DOM.Ready._onDOMReadyCallbacks = [];
}

DOM.Ready._runIdCallbacks = function () {
    for ( var id in DOM.Ready._onIdReadyCallbacks ) {
        // protect against changes to Object (ala prototype's extend)
        if ( ! DOM.Ready._onIdReadyCallbacks.hasOwnProperty(id) ) {
            continue;
        }

        var elt = document.getElementById(id);

        if (elt) {
            for ( var i = 0; i < DOM.Ready._onIdReadyCallbacks[id].length; i++) {
                DOM.Ready._onIdReadyCallbacks[id][i](elt);
            }

            delete DOM.Ready._onIdReadyCallbacks[id];
        }
    }
}

DOM.Ready._runReadyCallbacks = function () {
    if ( DOM.Ready._inRunReadyCallbacks ) return;

    DOM.Ready._inRunReadyCallbacks = 1;

    if ( DOM.Ready._checkDOM() ) {

        DOM.Ready._runDOMCallbacks();

        DOM.Ready._runIdCallbacks();
    }

    DOM.Ready._timePassed += DOM.Ready._lastTimerInterval;

    if ( ( DOM.Ready._timePassed / 1000 ) >= DOM.Ready.finalTimeout ) {
        DOM.Ready._clearTimer();
    }

    DOM.Ready._inRunReadyCallbacks = 0;
}

DOM.Ready._setTimer = function () {
    DOM.Ready._lastTimerInterval = DOM.Ready.timerInterval;
    DOM.Ready._intervalId = setInterval( DOM.Ready._runReadyCallbacks, DOM.Ready.timerInterval );
};

DOM.Ready._clearTimer = function () {
    clearInterval( DOM.Ready._intervalId );
    DOM.Ready._intervalId = null;
}

DOM.Ready._setTimer();

DOM.Ready.runCallbacks = function () { DOM.Ready._runReadyCallbacks() };


/*

*/

//// DOM.Element.Create
JSAN.use('DOM.Events','addListener');

/*

=head1 NAME

DOM.Element.Create - Create new DOM elments in a more declarative manner

=head1 SYNOPSIS

  // BEFORE 
    var link = document.createElement('a');
        link.href=    "http://www.google.com";
        link.title=   "Search the Web";
        link.target=  "_blank"
        link.onclick= function() { alert('here we go'); return true; };
        link.appendChild( document.createTextNode('link to Google') );
    var para = document.createElement('p');
        para.id        = 'foo';
        para.style.fontWeight = 'bold';
        para.style.border = '1px solid black';
        para.className = 'body-text';
        para.onmouseover = function(){ this.style.backgroundColor="red";   };
        para.onmouseout  = function(){ this.style.backgroundColor="white"; };
        para.appendChild( document.createTextNode('here is a ') );
        para.appendChild( link );

  // AFTER
    var para = createElement( 'p', {
        id:        'foo',
        className: 'body-text',
        style:     {
            fontWeight: 'bold',
            border: '1px solid black'
        },
        events:    {
            mouseover: function(){ this.style.backgroundColor="red";   },
            mouseout:  function(){ this.style.backgroundColor="white"; }
        },
        childNodes: [  'here is a ',

            createElement( 'a', {
                href:      'http://www.google.com',
                className: 'offsite',
                title:     'Search the Web',
                target:    '_blank',
                events:    {
                    click: function() { alert('here we go'); return true; }
                },
                childNodes: [ 'link to Google' ]
            })
        ]
    });

=head1 DESCRIPTION

It has been fairly common in code I've had to write to create one or more html elements
and subsequently set many of their attributes, append children and register event listeners.
Rather than write line after line of C<element.property='foo'> and
C<element.onevent=function(){}>, C<DOM.Element.Create> allows a more declarative approach to
defining new elements.

C<DOM.Element.Create> does not export any methods by default, but allows you to export either of the
methods described below.

=head2 Package Methods

=cut

*/

if ( typeof DOM == "undefined") DOM = {};
if ( typeof DOM.Element == "undefined") DOM.Element = {};

DOM.Element.Create = {

    VERSION: 0.01,

    EXPORT_OK: [ 'createElement','updateElement' ],

/*

=head3 C<createElement(  'tagName', {attributes}  ) >

Pass a tagName and an associative array of attributes to this method and get back the
correpsonding element.  attribute names are not checked for validity, allowing the possibility
of adding custom attributes to an element.  There are, however, 3 reserved keys:

=over

=item C<style>

Javascript typically manipulates style information in the form C<element.style.property='value'>.
The C<style> key in the attribute list is expected to point to an associative array of javascript-compatible
style properties.

=item C<events>

The C<events> key points to an associative array of eventlisteners.  The keys are expected to take the form
of 'click','mouseover' and the like, rather than 'onclick', 'onmouseover', etc.

=item C<childNodes>

This is an array of elements to be appended to the element being created.  Plain strings can be passed in as
well and will be turned into text nodes.

=back

=cut

*/

    createElement: function(tagName,attributes) {
        if( !tagName ) return null;
        var element = document.createElement(tagName);
        if( !element ) return null;

        return DOM.Element.Create.updateElement(element,attributes)
    },

/*

=head3 C<updateElement( element, {attributes} )>

This function applies the same attributes associative array described under C<createElement> to an already
existing element, C<element>.

=cut

*/

    updateElement: function(element,attributes) {
        if( !element ) return null;

        for( var attr in attributes ) {
            switch(attr) {
            case 'style':
                for( var property in attributes[attr] ) 
                    element.style[property] = attributes[attr][property];
                break;

            case 'events':
                for( var event in attributes[attr] ) 
                    addListener(element, event, attributes[attr][event] );
                break;

            case 'childNodes':
                for( var i = 0; i < attributes[attr].length; i++ ) {
                    var child = attributes[attr][i];

                    if( child ) {
                        if( typeof child == 'string' )
                            child = document.createTextNode( child );

                        element.appendChild( child );
                    }
                }
                break;

            default:
                element.setAttribute(attr,attributes[attr]);
            }
        }

        return element;
    }

}
/*

=head1 AUTHOR

Stephen Howard, <stephen@enterity.com>.

=head1 COPYRIGHT

  Copyright (c) 2005 Stephen Howard.  All rights reserved.
  This module is free software; you can redistribute it and/or modify it
  under the terms of the Artistic license.

=cut

*/

//// RSS.js
JSAN.use('HTTP.Query');
JSAN.use('HTTP.Request');
JSAN.use('DOM.Ready');
JSAN.use('DOM.Element.Create');

RSS = function() {};
RSS.VERSION = '0.01';

proto = RSS.prototype;

// divId is used to locate the div that's used as the reader pannel.
proto.createReader = function(divId, url, title) {
    this.readerPane = document.getElementById(divId);
    this.title = title;
    var nq = new HTTP.Query(url);
    this.slurpRequest(url, this.readerPane);
};

proto.slurpRequest = function(url, elem, method) {
    var self = this;
    if(!method) { method = 'get' };
    var r = new HTTP.Request({
            method: method,
            uri: url,
            onComplete: function(trans) {
                var rss = trans.responseXML;
                elem.innerHTML = self.toListItems(rss).innerHTML;
            }
        });
};

// Various OV Patch in this function.
proto.toListItems = function(rss) {
    var items = rss.getElementsByTagName("item");

    var ul_elem = document.createElement('span');
    for(var i = 0; i < items.length ; i++) {
        var title = items[i].getElementsByTagName("title")[0].firstChild.nodeValue;
        var link  = items[i].getElementsByTagName("link")[0].firstChild.nodeValue;
        var a_elem  = document.createElement('a');
        a_elem.title = title;
        a_elem.href  = link;
        a_elem.appendChild( document.createTextNode(title) );
        var li_elem = document.createElement('span');
        li_elem.appendChild(a_elem);
        li_elem.appendChild(document.createTextNode(" ... " ));
        ul_elem.appendChild(li_elem);
    }

    var title = this.title;
    if(!title) {
       title = rss.getElementsByTagName("title")[0].firstChild.nodeValue;
       title = title.replace(/^.*?-/,"");
       title = title.replace(/^\s*/,"");
       title = title.replace(/\s*$/,"");
    }
    var h1_elem = document.createElement("span");
    h1_elem.appendChild(document.createTextNode(title + ":" ));

    var div_elem = document.createElement('div');
    div_elem.appendChild(h1_elem);
    div_elem.appendChild(ul_elem);
    return div_elem;
}


