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
    this.oldstring = "OpenVanilla";
    this.candi = document.getElementById("candi");      	
}

OV.CandidateController.prototype.update = function(str) {
	if(this.string) this.oldstring = this.string;
    this.string = str;
    this.refresh();
}

OV.CandidateController.prototype.clear = function() {
	if(this.string) this.oldstring = this.string;
    this.string = "";
    this.refresh();
}


OV.CandidateController.prototype.refresh = function() {
	if(this.candi.loaded()){
		if(this.string) {
	    	this.candi.setInputValue("Candis", this.string);
	    	if(this.oldstring){ 
    			this.candi.setInputValue("OldCandis", this.oldstring);
	    	}
	    	if(this.candi.playing()) {
				this.candi.pause();	    	
		    	this.candi.setInputValue("start", 0);		    	
		    	this.candi.play();
		    	this.candi.setInputValue("start", 1);
	    	}	    	
		}
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