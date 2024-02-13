var express = require('express');
var expressWs = require('express-ws');
var createInputFromJson = require('./mapper/input.js');
var createOutputFromGameData = require('./mapper/output.js');
var runGame = require('./game/game.js');

var app = express();
expressWs(app);

var games = [];
var closed = [];

function selectOpenGame() {
    while (games.length) {
        var game = games.pop();
        if (game.socket) {
            return game;
        }
        closed.push(game);
    }
    return null;
}

app.get('/', function (req, res) {
    res.send('');
});
app.ws('/multiplayer', function (ws) {
    ws.on('message', function (json) {
        var data = createInputFromJson(json);
        if (!data) {
            ws.close();
            return;
        }

        var game = selectOpenGame();
        if (game) {
            var gameData = runGame(game, data);
            ws.send(createOutputFromGameData(gameData, 'player2', 'player1'));
            if (game.socket) {
                game.socket.send(createOutputFromGameData(gameData, 'player1', 'player2'));
            }
            if (game.timeout) {
                clearTimeout(game.timeout);
            }
            return;
        }

        data.socket = ws;
        data.timeout = setTimeout(function () {
            if (!data.socket || !closed.length) {
                return;
            }
            var gameData = runGame(closed.shift(), data);
            ws.send(createOutputFromGameData(gameData, 'player2', 'player1'));
        }, 30000);

        games.push(data);

        ws.on('close', function () {
            data.socket = null;
            clearTimeout(data.timeout);
            data.timeout = null;
        });
    });

});

app.listen(80);
