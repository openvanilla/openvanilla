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
