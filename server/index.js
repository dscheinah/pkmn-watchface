var express = require('express');
var expressWs = require('express-ws');
var createInputFromJson = require('./mapper/input.js');
var createOutputFromGameData = require('./mapper/output.js');
var runGame = require('./game/game.js');

var app = express();
expressWs(app);

var games = [];

function selectOpenGame() {
    while (games.length) {
        var game = games.pop();
        if (game.socket) {
            return game;
        }
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
            return;
        }
        data.socket = ws;
        games.push(data);
        ws.on('close', function () {
            data.socket = null;
        });
    });

});

app.listen(80);
