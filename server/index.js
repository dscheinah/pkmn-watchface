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

function selectClosedGame() {
    while (closed.length) {
        var game = closed.shift();
        if (!game.finished) {
            return game;
        }
    }
    return null
}

function clear(game) {
    game.socket = null;
    if (game.timeout) {
        clearTimeout(game.timeout);
        game.timeout = null;
    }
}

function runAndFinish(player1, player2) {
    var gameData = runGame(player1, player2);
    player1.finished = true;
    player2.finished = true;
    if (player1.socket) {
        player1.socket.send(createOutputFromGameData(gameData, 'player1', 'player2'));
    }
    clear(player1);
    if (player2.socket) {
        player2.socket.send(createOutputFromGameData(gameData, 'player2', 'player1'));
    }
    clear(player2);
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
        data.socket = ws;

        var game = selectOpenGame();
        if (game) {
            runAndFinish(game, data);
            return;
        }

        games.push(data);

        data.timeout = setTimeout(function () {
            var game = selectClosedGame();
            if (game) {
                runAndFinish(game, data);
            }
        }, 30000);

        ws.on('close', function () {
            clear(data);
        });
    });

});

app.listen(80);
