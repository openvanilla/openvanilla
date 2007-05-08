if (typeof OV == 'undefined') {
    OV = {};
}

OV.CandidateController = function (params) {
    // Parameter checking, setup default values
    if (typeof params == "undefined") {
        params = {};
    }
    if (typeof params["elementId"] == "undefined") {
        params["elementId"] = "candi";
    }

    // The candidate window content.
    this.string = "";

    // The HTML element that shows candidates
    this.elem = document.getElementById(params["elementId"]);
	if(this.elem.loaded()){
	    this.elem.pause();
	    this.elem.setInputValue("Candis", "歡迎、歡迎，熱烈歡迎");
	    this.elem.setInputValue("show", 1);
	    this.elem.play();
	}
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
	if(this.elem.loaded()){
    	this.elem.pause();
    	this.elem.setInputValue("Candis", this.string);    	
    	if(this.string != "") {
    		this.elem.setInputValue("show", 1); 
    	} else {
    		this.elem.setInputValue("show", 0);     		
    	}
    	this.elem.play();  
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
}

function test(str) {
    return str;
}
