if (typeof OV == 'undefined') {
    OV = {};
}

OV.Notify = {
    items: [],
    setup: function() {
        var notifier = function() {
            if (OV.Notify.items.length > 0) {
                var t = OV.Notify.items.pop();
                ov_update(t);
                setTimeout(function() {
                    ov_clear();
                }, 2900)
            }
        }
        setInterval(notifier, 5000);
    },
    rss: function(url) {
        new Ajax.Request(url, {
            method: 'get',
            onComplete: function(trans) {
                var text = trans.responseText
                var lines = text.split(/\n/)
                var title = [];
                for( var i=0; i<lines.length; i++ ) {
                    if (lines[i].indexOf("title") > 0) {
                        (function(l) {
                            l=l.replace(/<\/?title>/g,"")
                            OV.Notify.items.push(l)
                        })(lines[i])
                    }
                }
            }
        });
    }
}


