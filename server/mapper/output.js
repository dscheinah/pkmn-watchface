var mapper = {
    '001': {
        ally: 1,
        enemy: 45
    },
    '002': {
        ally: 4,
        enemy: 48
    },
    '003': {
        ally: 7,
        enemy: 51
    },
    '004': {
        ally: 2,
        enemy: 46
    },
    '005': {
        ally: 5,
        enemy: 49
    },
    '006': {
        ally: 8,
        enemy: 52
    },
    '007': {
        ally: 3,
        enemy: 47
    },
    '008': {
        ally: 6,
        enemy: 50
    },
    '009': {
        ally: 9,
        enemy: 53
    },
    '025': {
        ally: 105,
        enemy: 54
    },
    '026': {
        ally: 106,
        enemy: 55
    },
    '039': {
        ally: 139,
        enemy: 88
    },
    '040': {
        ally: 140,
        enemy: 89
    },
    '063': {
        ally: 107,
        enemy: 56
    },
    '064': {
        ally: 108,
        enemy: 57
    },
    '065': {
        ally: 109,
        enemy: 58
    },
    '086': {
        ally: 110,
        enemy: 59
    },
    '087': {
        ally: 111,
        enemy: 60
    },
    '092': {
        ally: 112,
        enemy: 61
    },
    '093': {
        ally: 113,
        enemy: 62
    },
    '094': {
        ally: 114,
        enemy: 63
    },
    '125': {
        ally: 149,
        enemy: 98
    },
    '132': {
        ally: 115,
        enemy: 64
    },
    '133': {
        ally: 116,
        enemy: 65
    },
    '134': {
        ally: 117,
        enemy: 66
    },
    '135': {
        ally: 120,
        enemy: 69
    },
    '136': {
        ally: 118,
        enemy: 67
    },
    '143': {
        ally: 142,
        enemy: 91
    },
    '144': {
        ally: 133,
        enemy: 82
    },
    '145': {
        ally: 134,
        enemy: 83
    },
    '146': {
        ally: 135,
        enemy: 84
    },
    '150': {
        ally: 138,
        enemy: 87
    },
    '151': {
        ally: 121,
        enemy: 70
    },
    '172': {
        ally: 130,
        enemy: 79
    },
    '174': {
        ally: 141,
        enemy: 90
    },
    '175': {
        ally: 131,
        enemy: 80
    },
    '176': {
        ally: 132,
        enemy: 81
    },
    '185': {
        ally: 148,
        enemy: 97
    },
    '196': {
        ally: 119,
        enemy: 68
    },
    '197': {
        ally: 122,
        enemy: 71
    },
    '200': {
        ally: 151,
        enemy: 100
    },
    '203': {
        ally: 123,
        enemy: 72
    },
    '213': {
        ally: 124,
        enemy: 73
    },
    '214': {
        ally: 125,
        enemy: 74
    },
    '216': {
        ally: 143,
        enemy: 92
    },
    '217': {
        ally: 144,
        enemy: 93
    },
    '225': {
        ally: 126,
        enemy: 75
    },
    '235': {
        ally: 127,
        enemy: 76
    },
    '239': {
        ally: 150,
        enemy: 99
    },
    '241': {
        ally: 128,
        enemy: 77
    },
    '243': {
        ally: 145,
        enemy: 94
    },
    '244': {
        ally: 146,
        enemy: 95
    },
    '245': {
        ally: 147,
        enemy: 96
    },
    '249': {
        ally: 136,
        enemy: 85
    },
    '250': {
        ally: 137,
        enemy: 86
    },
    '251': {
        ally: 129,
        enemy: 78
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
            mp_ally_health: Math.round(entry[ally].health),
            mp_enemy_health: Math.round(entry[enemy].health),
            mp_ally_level: Math.round(entry[ally].level),
            mp_enemy_level: Math.round(entry[enemy].level)
        })
    });
    return JSON.stringify(mapped);
};
