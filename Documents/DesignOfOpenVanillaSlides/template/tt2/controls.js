// BEGIN controls.js
function nextSlide() {
    window.location = '[% next_slide %]';
}

function prevSlide() {
    window.location = '[% prev_slide %]';
}

function indexSlide() {
    window.location = 'index.html';
}

function startSlide() {
    window.location = 'start.html';
}

function closeSlide() {
    window.close();
}

function handleKey(e) {
    var key;
    if (e == null) {
        // IE
        key = event.keyCode
    } 
    else {
        // Mozilla
        if (e.altKey || e.ctrlKey) {
            return true
        }
        key = e.which
    }
    switch(key) {
        case 8: prevSlide(); break
        case 13: nextSlide(); break
        case 32: nextSlide(); break
        case 81: closeSlide(); break
        case 105: indexSlide(); break
        case 110: nextSlide(); break
        case 112: prevSlide(); break
        case 115: startSlide(); break
        default: //xxx(e.which)
    }
}

document.onkeypress = handleKey
[% IF mouse_controls -%]
document.onclick = nextSlide
[% END -%]
// END controls.js
