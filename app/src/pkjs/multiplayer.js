var url = 'ws://pkmn.dscheinah.de/multiplayer';

var lastSelected;

var socket;
var timeout;

var timer = 750;

var running = false;

function finish(newRunning) {
    running = newRunning;
    if (timeout) {
        clearTimeout(timeout);
        timeout = null;
    }
    if (socket) {
        socket.close();
        socket = null;
    }
}

module.exports = {
    init: function (selected) {
        if (running) {
            return;
        }
        lastSelected = selected;
        Pebble.sendAppMessage({'mp_cmd': 0});
    },
    start: function (payload) {
        var currentSelected = lastSelected;
        if (running || !currentSelected) {
            return;
        }
        finish(false);
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
            finish(true);
            var data = JSON.parse(event.data);
            var dataLength = data.length;
            for (var i = 0; i < dataLength; i++) {
                setTimeout(function () {
                    Pebble.sendAppMessage(this);
                }.bind(data[i]), (i + 1) * timer);
            }
            if (data[i - 1] && data[i - 1].mp_cmd === 101) {
                Pebble.sendAppMessage({
                    'mp_cmd': 1,
                    'mp_ally': data[i - 1].mp_ally
                });
            }
            setTimeout(function () {
                finish(false);
            }, dataLength * timer + 10000);
        };
        timeout = setTimeout(function () {
            finish(false);
        }, 60000);
    }
};
