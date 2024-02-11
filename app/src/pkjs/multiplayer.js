var lastSelected;

module.exports = {
    init: function (selected) {
        lastSelected = selected;
        Pebble.sendAppMessage({'mp_cmd': 0});
    },
    start: function (payload) {
        var socket = new WebSocket('ws://pkmn.dscheinah.de/multiplayer');
        socket.onopen = function () {
            socket.send(JSON.stringify({
                monsters: lastSelected,
                steps: payload.mp_steps,
                sleep: payload.mp_sleep,
                restful: payload.mp_restful,
                active: payload.mp_active
            }));
        };
        socket.onmessage = function (event) {
            var data = JSON.parse(event.data);
            var dataLength = data.length;
            for (var i = 0; i < dataLength; i++) {
                setTimeout(function () {
                    Pebble.sendAppMessage(this);
                }.bind(data[i]), i * 5000);
            }
        };
    }
};
