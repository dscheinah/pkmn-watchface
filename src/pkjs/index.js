var Clay = require('pebble-clay');
var config = require('./config.json');
var pokedex = require('./pokedex.js');

var clay = new Clay(config, function () {
    var clayConfig = this;
    clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
        clayConfig.getItemByMessageKey('pokedex').setUserData(clayConfig.meta.userData);
    });
}, {
    userData: pokedex.userData
});

pokedex.init(clay);

Pebble.addEventListener('ready', function () {
    Pebble.addEventListener('appmessage', function (event) {
        pokedex.pebble(clay, event.payload);
    });
});
