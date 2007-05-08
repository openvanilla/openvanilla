if (typeof OV == 'undefined') {
    OV = {};
}

OV.CandidateController = function (params) {
    // Parameter checking, setup default values
    if (typeof params == "undefined") {
        params = {};
    }
    if (typeof params["elementId"] == "undefined") {
        params["elementId"] = "content";
    }

    // The candidate window content.
    this.string = "";

    // The HTML element that shows candidates
    this.elem = document.getElementById(params["elementId"]);
    // Give a welcome
    this.elem.innerHTML = "歡迎、歡迎，熱烈歡迎"; 
}

OV.CandidateController.prototype.update = function(str) {
    this.string = str;
    this.refresh();
}

OV.CandidateController.prototype.clear = function() {
    this.string = "";
    this.refresh();
}

OV.CandidateController.prototype.genhtml = function() {
	return "<object classid=\"clsid:d27cdb6e-ae6d-11cf-96b8-444553540000\" codebase=\"http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0\" width=\"400\" height=\"60\" id=\"candis\" align=\"middle\"><param name=\"allowScriptAccess\" value=\"sameDomain\" /><param name=\"movie\" value=\"candis.swf\" /><param name=\"quality\" value=\"high\" /><param name=\"bgcolor\" value=\"#ffffff\" /><param name=\"FlashVars\" value=\"myCandis=" + this.string + "\"><embed src=\"candis.swf\" FlashVars=\"myCandis=" + this.string + "\" quality=\"high\" bgcolor=\"#ffffff\" width=\"400\" height=\"60\" name=\"candis\" align=\"middle\" allowScriptAccess=\"sameDomain\" type=\"application/x-shockwave-flash\" pluginspage=\"http://www.macromedia.com/go/getflashplayer\" id=\"candivar\"/></object>";
}

OV.CandidateController.prototype.refresh = function() {
    this.elem.innerHTML = this.genhtml();
}

var ovc = {};
// The external API.
function ov_update (str) {
    ovc.update(str);
}

function ov_clear() {
    ovc.clear();
}

function ov_init() {
    ovc = new OV.CandidateController({});
    ov_update("");
}

function test(str) {
    return str;
}
