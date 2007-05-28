// filter.js
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

var preholder = "";

function load()
{
	setupParts();
	
	if (OVFilter) {
		var line = OVFilter.genList();
	}  
	if(line) {
	    line = "<select id=\"myfilter\" style=\"width:195px;\" onchange=\"convert(''); return false;\" " + line + "</select>";		
	    document.getElementById("filters").innerHTML= line;
	    document.getElementById("myfilter").selectedIndex = 0;	
	} else {
	    document.getElementById("filters").innerHTML= "Unable to load Modules";
	}
	
	if (window.getLocalizedString) {
		document.getElementById("input").value = getLocalizedString("Please input text here...");
	} else {
		document.getElementById("input").value = "Please input text here...";
	}
	preholder = document.getElementById("input").value;
	
	if (window.getLocalizedString) {
		document.getElementById("output").value = getLocalizedString("Converted result goes here...");
	} else {
		document.getElementById("output").value = "Converted result goes here...";
	}	
	// document.getElementById("output").disabled = true;
}

function remove()
{
	// your widget has just been removed from the layer
	// remove any preferences as needed
	// widget.setPreferenceForKey(null, createInstancePreferenceKey("your-key"));
}

function hide()
{
	// your widget has just been hidden stop any timers to
	// prevent cpu usage
}

function show()
{
	// your widget has just been shown.  restart any timers
	// and adjust your interface as needed
}

function showBack(event)
{
	// your widget needs to show the back

	var front = document.getElementById("front");
	var back = document.getElementById("back");

	if (window.widget)
		widget.prepareForTransition("ToBack");

	front.style.display="none";
	back.style.display="block";
	
	if (window.widget)
		setTimeout('widget.performTransition();', 0);
}

function showFront(event)
{
	// your widget needs to show the front

	var front = document.getElementById("front");
	var back = document.getElementById("back");

	if (window.widget)
		widget.prepareForTransition("ToFront");

	front.style.display="block";
	back.style.display="none";
	
	if (window.widget)
		setTimeout('widget.performTransition();', 0);
}

if (window.widget)
{
	widget.onremove = remove;
	widget.onhide = hide;
	widget.onshow = show;
}


function convert(event) 
{
	document.getElementById("output").value = "Converting";
    var myinput = document.getElementById("input").value;
    var myfilter = document.getElementById("myfilter").value;
	var myoutput;
	if (OVFilter) {
		myoutput = OVFilter.myConvert(myinput, myfilter);		
	} else {
		if (window.getLocalizedString) {
			myoutput = getLocalizedString("Error! Unable to covert your text.");
		} else {
			myoutput = "Error! Unable to covert your text.";
		}	
	}
	if(myoutput) { 
		document.getElementById("output").value = myoutput;
		document.getElementById("output").focus();
		document.getElementById("output").select();
	}
}


function clearDefaultText(event) 
{
	if(document.getElementById("input").value == preholder) {
		document.getElementById("input").value = "";
	}
	document.getElementById("input").select();	
}


function resetDefaultText(event) 
{
	if(document.getElementById("input").value == "") {
		document.getElementById("input").value = preholder;
	}
}


function clearDefaultText(event) 
{
	// Insert Code Here
}


function clickOnProvider(event)
{
    widget.openURL("http://openvanilla.org/");
}