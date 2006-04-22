var $ = function(id) {
    return document.getElementById(id);
}

if (typeof OpenVanilla == 'undefined')
    OpenVanilla = {};

OpenVanilla.VERSION = '0.72';

OpenVanilla.IM = function() { }

proto = OpenVanilla.IM.prototype;

proto.query = function(key) {
    throw("Must be override by child");
}

OpenVanilla.IM.Web = function(base, module) {
    this.base = base;
    this.module = module;
}

OpenVanilla.IM.Web.prototype = new OpenVanilla.IM;

proto = OpenVanilla.IM.Web.prototype;

proto._make_query_url = function(key) {
    return this.base + "/" + this.module + "/" + key;
}

proto.query = function(key) {
    var req = new HTTP.Request({
        method: 'GET',
        asynchronous: false
    });
    req.request(this._make_query_url(key));
    var jresult = req.transport.responseText;
    var r;
    eval("r = " + jresult);
    return r.results;
}


