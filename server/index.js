var express = require('express');

var app = express();

app.get('/', function (req, res) {
    res.send('');
});

app.listen(80);
