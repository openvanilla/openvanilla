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
