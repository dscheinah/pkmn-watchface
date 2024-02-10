function dealDamage(ally, enemy) {
    var modifier = 3;

    if (Math.random() * 100 < ally.extra) {
        modifier += 1;
    }
    if (Math.random() * 100 < ally.critical) {
        modifier += 1;
    }

    modifier += (ally.level - enemy.level) / 50;

    ally.types.forEach(function (current) {
        if (enemy.weak.includes(current)) {
            modifier += 1;
        }
        if (enemy.resist.includes(current)) {
            modifier -= 1;
        }
    });

    if (modifier < 1) {
        modifier = 1;
    }

    enemy.health -= 5 * modifier;
    return enemy.health < 1;
}

function transform(player) {
    if (!player.monsters[player.position]) {
        return {
            id: player.monsters[player.position - 1].id,
            health: 0
        };
    }
    return {
        id: player.monsters[player.position].id,
        health: player.monsters[player.position].health
    };
}

module.exports = function (player1, player2) {
    player1.position = 0;
    player1.finishCmd = 100;
    player2.position = 0;
    player2.finishCmd = 101;

    var games = [];

    var createEntry = function (cmd) {
        games.push({
            cmd: cmd,
            player1: transform(player1),
            player2: transform(player2)
        });
    };

    createEntry(11);

    while (player1.monsters[player1.position] && player2.monsters[player2.position]) {
        var ally = player1;
        var enemy = player2;
        var swap = player1.monsters[player1.position].agility < player2.monsters[player2.position].agility;
        if (player1.monsters[player1.position].agility === player2.monsters[player2.position].agility) {
            swap = Math.random() < 0.5;
        }
        if (swap) {
            ally = player2;
            enemy = player1;
        }
        if (dealDamage(ally.monsters[ally.position], enemy.monsters[enemy.position])) {
            enemy.position++;
            if (!enemy.monsters[enemy.position]) {
                createEntry(enemy.finishCmd);
                break;
            }
            createEntry(11);
            continue;
        }
        if (dealDamage(enemy.monsters[enemy.position], ally.monsters[ally.position])) {
            ally.position++;
            if (!ally.monsters[ally.position]) {
                createEntry(ally.finishCmd);
                break;
            }
            createEntry(11);
            continue;
        }
        createEntry(10);
    }

    return games;
};
