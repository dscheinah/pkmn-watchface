function dealDamage(ally, enemy) {
    var level = 2 * ally.level / 5 + 2;

    var stats = ally.offense / enemy.defense;

    var critical = Math.random() * 100 < ally.critical ? 1.5 : 1;

    var modifier = 1;
    ally.types.forEach(function (current) {
        if (enemy.weak.includes(current)) {
            modifier += 0.5;
        }
        if (enemy.resist.includes(current)) {
            modifier -= 0.25;
        }
    });

    var random = 1 - Math.random() * 0.15;

    enemy.health -= Math.round(((level * ally.power * stats) / 50 * critical + 2) * modifier * random);
    if (ally.power > 25) {
        ally.power -= 5;
    }
    return enemy.health < 1 || enemy.health > 100;
}

function transform(player) {
    if (!player.monsters[player.position]) {
        return {
            id: player.monsters[player.position - 1].id,
            level: player.monsters[player.position - 1].level,
            health: 0
        };
    }
    return {
        id: player.monsters[player.position].id,
        level: player.monsters[player.position].level,
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
