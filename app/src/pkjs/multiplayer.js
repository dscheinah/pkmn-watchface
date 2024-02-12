var url = 'ws://pkmn.dscheinah.de/multiplayer';

var lastSelected;
var socket;

var running = false;

module.exports = {
    init: function (selected) {
        if (running) {
            return;
        }
        lastSelected = selected;
        Pebble.sendAppMessage({'mp_cmd': 0});
    },
    start: function (payload) {
        if (running) {
            return;
        }
        var currentSelected = lastSelected;
        if (!currentSelected) {
            return;
        }
        if (socket) {
            socket.close();
        }
        socket = new WebSocket(url);
        socket.onopen = function () {
            socket.send(JSON.stringify({
                monsters: currentSelected,
                steps: payload.mp_steps,
                sleep: payload.mp_sleep,
                restful: payload.mp_restful,
                active: payload.mp_active
            }));
        };
        socket.onmessage = function (event) {
            running = true;
            var data = JSON.parse(event.data);
            var dataLength = data.length;
            for (var i = 0; i < dataLength; i++) {
                setTimeout(function () {
                    Pebble.sendAppMessage(this);
                }.bind(data[i]), i * 5000);
            }
            setTimeout(function () {
                running = false;
            }, dataLength * 5000);
            socket.close();
            socket = null;
        };
    }
};
