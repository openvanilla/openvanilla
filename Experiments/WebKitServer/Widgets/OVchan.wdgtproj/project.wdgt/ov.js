OV = {};

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

OV.CandidateController.prototype.refresh = function() {
    this.elem.innerHTML = this.string;
    if(this.elem.innerHTML == "") {
    	this.elem.style.display = "none";
    } else {
    	this.elem.style.display = "block";  
    }
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

    var e = document.getElementById("ovchan");
    setInterval(function() {
    	e.src="url(Default2.png)";
    }, 10000);
    setInterval(function() {
    	e.src="url(Default.png)";
    }, 10500);
}

function test(str) {
    return str;
}
