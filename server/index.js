var express = require('express');
var expressWs = require('express-ws');

var app = express();
expressWs(app);

var games = [];

app.get('/', function (req, res) {
    res.send('');
});
app.ws('/multiplayer', function (ws) {
    ws.on('start', function (json) {
        var data = JSON.parse(json);
        if (games.length) {
            var game = games.pop();
            ws.send('data', JSON.stringify({}));
            game.socket.send('data', JSON.stringify({}));
            return;
        }
        data.socket = ws;
        games.push(data);
    });
});

app.listen(80);
