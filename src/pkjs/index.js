var Clay = require('pebble-clay');
var config = require('./config.json');
var clay = new Clay(config);

clay.registerComponent({
    name: 'pokedex',
    template: '<div class="component pokedex"></div>',
    manipulator: {
        get: function () {
        },
        set: function (index) {
            var count = 0;
            for (var i = 0; i < 2; i++) {
                var current = parseInt(index[i]);
                for (var j = 0; j < 32; j++) {
                    if (current & (1 << j)) {
                        count++;
                    }
                }
            }
            this.$element.set('innerHTML', count);
        }
    },
    style: ''
});
clay.setSettings('pokedex', [localStorage.getItem('pokedex0'), localStorage.getItem('pokedex1')]);
Pebble.addEventListener('ready', function () {
    Pebble.addEventListener('appmessage', function (event) {
        var index = [event.payload['pokedex0'].toString(), event.payload['pokedex1'].toString()];
        localStorage.setItem('pokedex0', index[0]);
        localStorage.setItem('pokedex1', index[1]);
        clay.setSettings('pokedex', index);
    });
});
