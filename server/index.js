var express = require('express');
var expressWs = require('express-ws');
var createInputFromJson = require('./mapper/input.js');
var createOutputFromGameData = require('./mapper/output.js');
var runGame = require('./game/game.js');

var app = express();
expressWs(app);

var games = [];

app.get('/', function (req, res) {
    res.send('');
});
app.ws('/multiplayer', function (ws) {
    ws.on('message', function (json) {
        var data = createInputFromJson(json);
        if (games.length) {
            var game = games.pop();
            var gameData = runGame(game.data, data);
            ws.send(createOutputFromGameData(gameData, 'player2', 'player1'));
            game.socket.send(createOutputFromGameData(gameData, 'player1', 'player2'));
            return;
        }
        games.push({
            data: data,
            socket: ws
        });
    });
});

app.listen(80);
