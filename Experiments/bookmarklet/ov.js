
	input = "";
	words = [];
	position = 0;
	onoff = false;



	function imedown (e) {

		if (e.ctrlKey) switchonoff();

		if (!onoff) return true;

		var code = e.keyCode;
		var il = input.length;
		var r = true;


		if (code == 13 && words.length > 0) {
			r = false;
			insertAtCursor(e.target, words[position]);
			input = "";
			words = [];
			position = 0;
			document.getElementById('imeblock').parentNode.removeChild(document.getElementById('imeblock'));
		}

		if ((code == 38 || code == 40) && words.length > 0) {
			if (code == 38 && position != 0) {
				var w = document.getElementById("ime_word"+position);
				w.style.background = "#fff";
				position -= 1;
				var w = document.getElementById("ime_word"+position);
				w.style.background = "#99f";
			}
			if (code == 40 && position != words.length) {
				var w = document.getElementById("ime_word"+position);
				w.style.background = "#fff";
				position = position + 1;
				var w = document.getElementById("ime_word"+position);
				w.style.background = "#99f";
			}
		}

		if (code == 8 && il > 0) {
		
			input = input.substring(0,(il-1));
			il -= 1;
			r = false;
			
			if (il == 0) {
				input = "";
				words = [];
				position = 0;
				document.getElementById('imeblock').parentNode.removeChild(document.getElementById('imeblock'));
			}
		
		}

		if (((code >= 65 && code <= 90) || (code >= 48 && code <= 57) || code == 59 || code == 109 || code == 188 || code == 190 || code == 191 )
		&& !e.shiftKey) {
			input += String.fromCharCode(code);
			r = false;
		}
		
		if (code == 32) {
		
			imeget(input.toLowerCase());
			r = false;
		
		}
		
		return r;
	
	}

	function switchonoff() {
		onoff = !onoff;
		
		if (onoff) {
		
			var div = document.createElement('div');
			div.id = "imeonoff";
			div.style.position = "absolute";
			div.style.top = "60px";
			div.style.left = "100px";
			div.style.width = "60px";
			div.style.border = "2px solid yellow";
			div.style.background = "yellow";

			var text = document.createTextNode("IME ON");
			div.appendChild(text);

			document.getElementsByTagName('body')[0].appendChild(div);
		}
		
		else {
		
			if (document.getElementById('imeonoff')) {
				document.getElementById('imeonoff').parentNode.removeChild(document.getElementById('imeonoff'));
			}

		
		}
	}


	function imeget (i) {

		var reg = new RegExp("\n"+i+"[ \t]+", "ig");
		var ar = reg.exec(cin);
		while (ar) {
			words.push(cin.substring((ar.index+ar[0].length), (ar.index+ar[0].length+1)));
			ar = reg.exec(cin);
		}

		imeshow();

	}
	
	function imeshow () {
	
		var div = document.createElement('div');
		div.id = "imeblock";
		div.style.position = "absolute";
		div.style.top = "100px";
		div.style.left = "100px";
		div.style.width = "60px";
		div.style.border = "2px solid gray";

		var inputdiv = document.createElement('div');
		var inputtext = document.createTextNode(input.toLowerCase());
		inputdiv.style.background = "#ccc";
		inputdiv.style.padding = "2px";
		inputdiv.appendChild(inputtext);
		div.appendChild(inputdiv);

		var list = document.createElement('ol');
		list.style.margin = "1px";
		list.style.padding = "0";
		for (i=0;i<words.length;i++) {
			var word = document.createElement('li');
			word.id = "ime_word"+i;
			if (i == 0) word.style.background = "#99f";
			word.style.margin = "0";
			word.style.padding = "2px";
			word.style.listStylePosition = "inside";
			var text = document.createTextNode(words[i]);
			word.appendChild(text);
			list.appendChild(word);
		}
		div.appendChild(list);
		
		if (document.getElementById('imeblock')) {
			document.getElementById('imeblock').parentNode.removeChild(document.getElementById('imeblock'));
		}
		
		document.getElementsByTagName('body')[0].appendChild(div);
		
	}
	
	function insertAtCursor(myField, myValue) {
		//IE support
		if (document.selection) {
			myField.focus();
			sel = document.selection.createRange();
			sel.text = myValue;
		}
		//MOZILLA/NETSCAPE support
		else if (myField.selectionStart || myField.selectionStart == '0') {
			var startPos = myField.selectionStart;
			var endPos = myField.selectionEnd;
			myField.value = myField.value.substring(0, startPos)
			+ myValue
			+ myField.value.substring(endPos, myField.value.length);
		} else {
				myField.value += myValue;
		}
		myField.selectionStart = startPos+1;
		myField.selectionEnd = startPos+1;
	}


function imeinit() {

	var inputs = document.getElementsByTagName('input');
	for (i = 0; i<inputs.length; i++)
		if(inputs[i].type != "password" && inputs[i].type != "radio" && inputs[i].type != "checkbox" )
			addEvent(inputs[i], "keydown", imedown);

	var textareas = document.getElementsByTagName('textarea');
	for (i = 0; i<textareas.length; i++)
		addEvent(textareas[i], "keydown", imedown);

	}
	
	
imeinit();