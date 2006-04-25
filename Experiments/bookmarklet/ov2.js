
	input = "";
	words = [];
	position = 0;
	onoff = false;



	function imedown (e) {

		var code = e.keyCode;
		var ch = String.fromCharCode(code);
		var il = input.length;
		var r = true;

		if (code == 17) switchonoff();
		if (!onoff) return true;

		if (code == 13 && words.length > 0) {
			r = false;
			insertAtCursor(e.target, words[position]);
			input = "";
			words = [];
			position = 0;
			imeupdate();
			document.getElementById('imelist').parentNode.removeChild(document.getElementById('imelist'));
		}

		if ((code == 38 || code == 40) && words.length > 0) {
			if (code == 38 && position != 0) {
				var w = document.getElementById("ime_word"+position);
				w.style.background = "#fff";
				position -= 1;
				var w = document.getElementById("ime_word"+position);
				w.style.background = "#99f";
			}
			if (code == 40 && position != words.length-1) {
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
			
			imeupdate();
			
			if (il == 0) {
				input = "";
				words = [];
				position = 0;
				if (document.getElementById('imelist'))
					document.getElementById('imelist').parentNode.removeChild(document.getElementById('imelist'));
			}
		
		}

		if (((code >= 65 && code <= 90) || (code >= 48 && code <= 57) || code == 59 || code == 109 || code == 188 || code == 190 || code == 191 )
		&& !e.shiftKey) {
			input += ch;
			imeupdate();
			r = false;
		}

		var reg = new RegExp("\n%endkey[ \t]+([^\n]+)", "ig");
		var ar = reg.exec(cin);
		if (ar) {
			var endkey = ar[1]+" ";
		}
		else {
			var endkey = " ";
		}
		var reg = new RegExp(""+ch+"", "ig");
		var ar = reg.exec(endkey);

		if (ar) {

			imeget(input.toLowerCase());
			r = false;

		}
		
		return r;
	
	}

	function switchonoff() {
		onoff = !onoff;
		
		if (onoff) {

			var div = document.createElement('div');
			div.id = "imeblock";
			div.style.position = "absolute";
			div.style.top = "100px";
			div.style.left = "100px";
			div.style.width = "60px";
			div.style.border = "2px solid gray";

			var inputdiv = document.createElement('div');
			inputdiv.id = "inputdiv";
			inputdiv.style.background = "#ccc";
			inputdiv.style.padding = "2px";
			inputdiv.style.height = "1.2em";
			var inputtext = document.createTextNode("");
			inputdiv.appendChild(inputtext);
			div.appendChild(inputdiv);
			
			document.getElementsByTagName('body')[0].appendChild(div);

		}
		
		else {
		
			r = false;
			input = "";
			words = [];
			position = 0;
			document.getElementById('imeblock').parentNode.removeChild(document.getElementById('imeblock'));
		
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
	
	function imeupdate () {
	
		var input2 = input.toLowerCase();
		var inputdisplay = "";
		for (i=0; i<input2.length; i++) {
		
			var reg = new RegExp("\n"+input2[i]+"[ \t]+", "ig");
			var ar = reg.exec(cin);
			inputdisplay += cin.substring((ar.index+ar[0].length), (ar.index+ar[0].length+1));
		
		}
	
		var inputdiv = document.getElementById('inputdiv');
		inputdiv.removeChild(inputdiv.firstChild);
		var inputtext = document.createTextNode(inputdisplay);
		inputdiv.style.background = "#ccc";
		inputdiv.style.padding = "2px";
		inputdiv.appendChild(inputtext);
	
	}
	
	function imeshow () {
	
		var div = document.getElementById('imeblock');

		var list = document.createElement('ol');
		list.id = "imelist";
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
		
		if (document.getElementById('imelist')) {
			document.getElementById('imelist').parentNode.removeChild(document.getElementById('imelist'));
		}

		div.appendChild(list);
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
	

