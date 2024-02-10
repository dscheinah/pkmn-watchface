var mapper = {
    1: {
        ally: 1,
        enemy: 1
    }
};

module.exports = function (data, ally, enemy) {
    var mapped = [];
    data.forEach(function (entry) {
        var cmd = entry.cmd;
        if (ally === 'player2') {
            switch (cmd) {
                case 100:
                    cmd = 101;
                    break;
                case 101:
                    cmd = 100;
                    break;
            }
        }
        mapped.push({
            mp_cmd: cmd,
            mp_ally: mapper[entry[ally].id].ally,
            mp_enemy: mapper[entry[enemy].id].enemy,
            mp_ally_health: entry[ally].health,
            mp_enemy_health: entry[enemy].health
        })
    });
    return JSON.stringify(mapped);
};
