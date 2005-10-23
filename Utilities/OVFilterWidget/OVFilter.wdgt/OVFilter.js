function checklist() {
    var output = widget.system("/usr/local/bin/ovfilter -l",null).outputString;
    startPos = output.search("available:");
    output = output.replace(/available:/gi, "<select id=\"myfilter\" style=\"width:180px;\">");
    output = output.substring(startPos);
    output = output.replace(/OVOF(\w+)\n/gi, "<option value=\"OVOF$1\">OVOF$1</option>\n");
    output = output + "</select>"
    document.getElementById("filters").innerHTML= output;
}

function convert(){
    var myinput = "\"" + document.getElementById("input").value + "\n\"";
    /* myinput = myinput.replace(/\n/g, "\\n"); */  
    var myfilter = document.getElementById("myfilter").value;
    var mycmd ="echo " + myinput +"| /usr/local/bin/ovfilter " + myfilter;
    var myoutput = widget.system(mycmd,null).outputString;
    var myoutput = myoutput + widget.system(mycmd,null).errortString;
    document.getElementById("output").value = myoutput;
}

function setup() {
    createGenericButton(document.getElementById("donePrefsButton"),"OK",hidePrefs,60);
    document.getElementById("donePrefsButton").display = "none";

    checklist();
    // put your setup code here...
}

// everything below here handles showing and flipping to the prefs on the back and back again...
function showPrefs()
{
    var front = document.getElementById("front");
    var back = document.getElementById("back");

    if (window.widget)
	widget.prepareForTransition("ToBack");

    front.style.display="none";
    back.style.display="block";
    document.getElementById("donePrefsButton").display = "block";

    if (window.widget)
	setTimeout ('widget.performTransition();', 0);
}

function hidePrefs()
{
    var front = document.getElementById("front");
    var back = document.getElementById("back");

    if (window.widget)
	widget.prepareForTransition("ToFront");

    back.style.display="none";
    document.getElementById("donePrefsButton").display = "none";
    front.style.display="block";

    if (window.widget)
	setTimeout ('widget.performTransition();', 0);
}

function enterflip(event)
{
    document.getElementById('fliprollie').style.display = 'block';
}
function exitflip(event)
{
    document.getElementById('fliprollie').style.display = 'none';
}

var flipShown = false;
var animation = {duration:0, starttime:0, to:1.0, now:0.0, from:0.0, firstElement:null, timer:null};
function mousemove (event)
{
    if (!flipShown)
    {
	if (animation.timer != null)
	{
	    clearInterval (animation.timer);
	    animation.timer  = null;
	}

	var starttime = (new Date).getTime() - 13;

	animation.duration = 500;
	animation.starttime = starttime;
	animation.firstElement = document.getElementById ('flip');
	animation.timer = setInterval ("animate();", 13);
	animation.from = animation.now;
	animation.to = 1.0;
	animate();
	flipShown = true;
    }
}
function mouseexit (event)
{
    if (flipShown)
    {
	// fade in the info button
	if (animation.timer != null)
	{
	    clearInterval (animation.timer);
	    animation.timer  = null;
	}

	var starttime = (new Date).getTime() - 13;

	animation.duration = 500;
	animation.starttime = starttime;
	animation.firstElement = document.getElementById ('flip');
	animation.timer = setInterval ("animate();", 13);
	animation.from = animation.now;
	animation.to = 0.0;
	animate();
	flipShown = false;
    }
}
function animate()
{
    var T;
    var ease;
    var time = (new Date).getTime();


    T = limit_3(time-animation.starttime, 0, animation.duration);

    if (T >= animation.duration)
    {
	clearInterval (animation.timer);
	animation.timer = null;
	animation.now = animation.to;
    }
    else
    {
	ease = 0.5 - (0.5 * Math.cos(Math.PI * T / animation.duration));
	animation.now = computeNextFloat (animation.from, animation.to, ease);
    }

    animation.firstElement.style.opacity = animation.now;
}
function limit_3 (a, b, c)
{
    return a < b ? b : (a > c ? c : a);
}
function computeNextFloat (from, to, ease)
{
    return from + (to - from) * ease;
}
