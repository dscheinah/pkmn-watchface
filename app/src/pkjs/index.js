var Clay = require('pebble-clay');
var config = require('./config.json');
var pokedex = require('./pokedex.js');

function customFn () {
    var c = this;
    c.on('AFTER_BUILD', function() {
        c.getItemByMessageKey('pokedex').setData(c.meta.userData);
    });
}
customFn.toString = function () {
    return 'function(){var c=this;c.on("AFTER_BUILD",function(){c.getItemByMessageKey("pokedex").setData(c.meta.userData)})}';
};

var clay = new Clay(config, customFn, {
    userData: pokedex.userData,
    autoHandleEvents: false
});

clay.components.select.initialize.toString = function () {
    return 'function (){var e,n=this;(e=n.$element?function(){var e=n.$element.select(".value"),t=n.$manipulatorTarget.get("selectedIndex"),o=n.$manipulatorTarget.select("option");e.set("innerHTML",o[t]&&o[t].innerHTML)}:function(){var e=document.querySelector(".component-select select");if(e){var n=document.querySelector(".component-select .value"),t=e.selectedIndex,o=e.options;n.innerHTML=o[t]&&o[t].innerHTML||""}})(),n.on("change",e)}'
};

pokedex.init(clay);

var timeout;

function openUrl() {
    timeout = null;
    Pebble.openURL(clay.generateUrl());
}

Pebble.addEventListener('showConfiguration', function() {
    timeout = setTimeout(openUrl, 1000);
    Pebble.sendAppMessage({});
});

Pebble.addEventListener('webviewclosed', function(e) {
    Pebble.sendAppMessage(clay.getSettings(e && e.response || {}));
});

Pebble.addEventListener('ready', function () {
    Pebble.addEventListener('appmessage', function (event) {
        if (!event.payload.pokedex0) {
            return;
        }
        pokedex.pebble(clay, event.payload);
        if (timeout) {
            clearTimeout(timeout);
            openUrl();
        }
    });
});
