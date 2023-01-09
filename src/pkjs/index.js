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
    userData: pokedex.userData
});

pokedex.init(clay);

Pebble.addEventListener('ready', function () {
    Pebble.addEventListener('appmessage', function (event) {
        pokedex.pebble(clay, event.payload);
    });
});
