var types = {
    PLANT: 1,
    FIRE: 2,
    WATER: 3,
    POISON: 4,
    BATTLE: 5,
    ELECTRIC: 6,
    FAIRY: 7,
    NORMAL: 8,
    GROUND: 9,
    STONE: 10,
    BUG: 11,
    GHOST: 12,
    STEEL: 13,
    DRAGON: 14,
    DARK: 15,
    FLIGHT: 16,
    PSYCHO: 17,
    ICE: 18
};

var mapper = {
    '0': {
        id: '001',
        types: [types.PLANT, types.POISON],
        weak: [types.FLIGHT, types.FIRE, types.PSYCHO, types.ICE],
        resist: [types.PLANT, types.BATTLE, types.WATER, types.ELECTRIC, types.FAIRY]
    },
    '3': {
        id: '002',
        types: [types.PLANT, types.POISON],
        weak: [types.FLIGHT, types.FIRE, types.PSYCHO, types.ICE],
        resist: [types.PLANT, types.BATTLE, types.WATER, types.ELECTRIC, types.FAIRY]
    },
    '6': {
        id: '003',
        types: [types.PLANT, types.POISON],
        weak: [types.FLIGHT, types.FIRE, types.PSYCHO, types.ICE],
        resist: [types.PLANT, types.BATTLE, types.WATER, types.ELECTRIC, types.FAIRY]
    },
    '1': {
        id: '004',
        types: [types.FIRE],
        weak: [types.GROUND, types.STONE, types.WATER],
        resist: [types.BUG, types.STEEL, types.FIRE, types.PLANT, types.FAIRY]
    },
    '4': {
        id: '005',
        types: [types.FIRE],
        weak: [types.GROUND, types.STONE, types.WATER],
        resist: [types.BUG, types.STEEL, types.FIRE, types.PLANT, types.FAIRY]
    },
    '7': {
        id: '006',
        types: [types.FIRE, types.FLIGHT],
        weak: [types.WATER, types.ELECTRIC, types.STONE, types.ICE],
        resist: [types.GROUND, types.BUG, types.PLANT, types.BATTLE, types.STEEL, types.FIRE, types.FAIRY]
    },
    '2': {
        id: '007',
        types: [types.WATER],
        weak: [types.PLANT, types.ELECTRIC],
        resist: [types.STEEL, types.FIRE, types.WATER, types.ICE]
    },
    '5': {
        id: '008',
        types: [types.WATER],
        weak: [types.PLANT, types.ELECTRIC],
        resist: [types.STEEL, types.FIRE, types.WATER, types.ICE]
    },
    '8': {
        id: '009',
        types: [types.WATER],
        weak: [types.PLANT, types.ELECTRIC],
        resist: [types.STEEL, types.FIRE, types.WATER, types.ICE]
    },
    '9': {
        id: '025',
        types: [types.ELECTRIC],
        weak: [types.GROUND],
        resist: [types.FLIGHT, types.STEEL, types.ELECTRIC]
    },
    '10': {
        id: '026',
        types: [types.ELECTRIC],
        weak: [types.GROUND],
        resist: [types.FLIGHT, types.STEEL, types.ELECTRIC]
    },
    '43': {
        id: '039',
        types: [types.NORMAL, types.FAIRY],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '44': {
        id: '040',
        types: [types.NORMAL, types.FAIRY],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '11': {
        id: '063',
        types: [types.PSYCHO],
        weak: [types.BUG, types.DARK],
        resist: [types.GHOST, types.BATTLE, types.PSYCHO]
    },
    '12': {
        id: '064',
        types: [types.PSYCHO],
        weak: [types.BUG, types.DARK],
        resist: [types.GHOST, types.BATTLE, types.PSYCHO]
    },
    '13': {
        id: '065',
        types: [types.PSYCHO],
        weak: [types.BUG, types.DARK],
        resist: [types.GHOST, types.BATTLE, types.PSYCHO]
    },
    '14': {
        id: '086',
        types: [types.WATER],
        weak: [types.PLANT, types.ELECTRIC],
        resist: [types.STEEL, types.FIRE, types.WATER, types.ICE]
    },
    '15': {
        id: '087',
        types: [types.WATER, types.ICE],
        weak: [types.BATTLE, types.STONE, types.PLANT, types.ELECTRIC],
        resist: [types.WATER, types.ICE]
    },
    '16': {
        id: '092',
        types: [types.GHOST, types.POISON],
        weak: [types.GHOST, types.PSYCHO, types.DARK],
        resist: [types.NORMAL, types.BATTLE, types.POISON, types.PLANT, types.FAIRY]
    },
    '17': {
        id: '093',
        types: [types.GHOST, types.POISON],
        weak: [types.GHOST, types.PSYCHO, types.DARK],
        resist: [types.NORMAL, types.BATTLE, types.POISON, types.PLANT, types.FAIRY]
    },
    '18': {
        id: '094',
        types: [types.GHOST, types.POISON],
        weak: [types.GHOST, types.PSYCHO, types.DARK],
        resist: [types.NORMAL, types.BATTLE, types.POISON, types.PLANT, types.FAIRY]
    },
    '53': {
        id: '125',
        types: [types.ELECTRIC],
        weak: [types.GROUND],
        resist: [types.FLIGHT, types.STEEL, types.ELECTRIC]
    },
    '19': {
        id: '132',
        types: [types.NORMAL],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '20': {
        id: '133',
        types: [types.NORMAL],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '21': {
        id: '134',
        types: [types.WATER],
        weak: [types.PLANT, types.ELECTRIC],
        resist: [types.STEEL, types.FIRE, types.WATER, types.ICE]
    },
    '24': {
        id: '135',
        types: [types.ELECTRIC],
        weak: [types.GROUND],
        resist: [types.FLIGHT, types.STEEL, types.ELECTRIC]
    },
    '22': {
        id: '136',
        types: [types.FIRE],
        weak: [types.GROUND, types.STONE, types.WATER],
        resist: [types.BUG, types.STEEL, types.FIRE, types.PLANT, types.FAIRY]
    },
    '46': {
        id: '143',
        types: [types.NORMAL],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '37': {
        id: '144',
        types: [types.ICE, types.FLIGHT],
        weak: [types.STONE, types.STEEL, types.FIRE, types.ELECTRIC],
        resist: [types.GROUND, types.BUG, types.PLANT]
    },
    '38': {
        id: '145',
        types: [types.ELECTRIC, types.FLIGHT],
        weak: [types.STONE, types.ICE],
        resist: [types.GROUND, types.BATTLE, types.FLIGHT, types.BUG, types.STEEL, types.PLANT]
    },
    '39': {
        id: '146',
        types: [types.FIRE, types.FLIGHT],
        weak: [types.STONE, types.WATER, types.ELECTRIC, types.ICE],
        resist: [types.GROUND, types.BUG, types.PLANT, types.BATTLE, types.STEEL, types.FIRE, types.FAIRY]
    },
    '42': {
        id: '150',
        types: [types.PSYCHO],
        weak: [types.BUG, types.DARK],
        resist: [types.GHOST, types.BATTLE, types.PSYCHO]
    },
    '25': {
        id: '151',
        types: [types.PSYCHO],
        weak: [types.BUG, types.DARK],
        resist: [types.GHOST, types.BATTLE, types.PSYCHO]
    },
    '34': {
        id: '172',
        types: [types.ELECTRIC],
        weak: [types.GROUND],
        resist: [types.FLIGHT, types.STEEL, types.ELECTRIC]
    },
    '45': {
        id: '174',
        types: [types.NORMAL, types.FAIRY],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '35': {
        id: '175',
        types: [types.FAIRY],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '36': {
        id: '176',
        types: [types.FAIRY, types.FLIGHT],
        weak: [types.STONE, types.ELECTRIC, types.ICE],
        resist: [types.GROUND, types.GHOST, types.BUG, types.PLANT]
    },
    '52': {
        id: '185',
        types: [types.STONE],
        weak: [types.BATTLE, types.GROUND, types.STEEL, types.WATER, types.PLANT],
        resist: [types.NORMAL, types.FLIGHT, types.POISON, types.FIRE]
    },
    '23': {
        id: '196',
        types: [types.PSYCHO],
        weak: [types.BUG, types.GHOST, types.DARK],
        resist: [types.BATTLE, types.PSYCHO]
    },
    '26': {
        id: '197',
        types: [types.DARK],
        weak: [types.BATTLE, types.BUG, types.FAIRY],
        resist: [types.PSYCHO, types.GHOST, types.DARK]
    },
    '55': {
        id: '200',
        types: [types.GHOST],
        weak: [types.GHOST, types.DARK],
        resist: [types.NORMAL, types.BATTLE, types.GROUND, types.POISON, types.BUG]
    },
    '27': {
        id: '203',
        types: [types.NORMAL, types.PSYCHO],
        weak: [types.BUG, types.DARK],
        resist: [types.GHOST, types.PSYCHO]
    },
    '28': {
        id: '213',
        types: [types.BUG, types.STONE],
        weak: [types.STONE, types.STEEL, types.WATER],
        resist: [types.NORMAL, types.POISON]
    },
    '29': {
        id: '214',
        types: [types.BUG, types.BATTLE],
        weak: [types.FLIGHT, types.FIRE, types.PSYCHO, types.FAIRY],
        resist: [types.BATTLE, types.GROUND, types.BUG, types.PLANT, types.DARK]
    },
    '47': {
        id: '216',
        types: [types.NORMAL],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '48': {
        id: '217',
        types: [types.NORMAL],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '30': {
        id: '225',
        types: [types.ICE, types.FLIGHT],
        weak: [types.STONE, types.STEEL, types.FIRE, types.ELECTRIC],
        resist: [types.GROUND, types.BUG, types.PLANT]
    },
    '31': {
        id: '235',
        types: [types.NORMAL],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '54': {
        id: '239',
        types: [types.ELECTRIC],
        weak: [types.GROUND],
        resist: [types.FLIGHT, types.STEEL, types.ELECTRIC]
    },
    '32': {
        id: '241',
        types: [types.NORMAL],
        weak: [types.BATTLE],
        resist: [types.GHOST]
    },
    '49': {
        id: '243',
        types: [types.ELECTRIC],
        weak: [types.GROUND],
        resist: [types.FLIGHT, types.STEEL, types.ELECTRIC]
    },
    '50': {
        id: '244',
        types: [types.FIRE],
        weak: [types.GROUND, types.STONE, types.WATER],
        resist: [types.BUG, types.STEEL, types.FIRE, types.PLANT, types.ICE, types.FAIRY]
    },
    '51': {
        id: '245',
        types: [types.WATER],
        weak: [types.PLANT, types.ELECTRIC],
        resist: [types.STEEL, types.FIRE, types.WATER, types.ICE]
    },
    '40': {
        id: '249',
        types: [types.PSYCHO, types.FLIGHT],
        weak: [types.STONE, types.GHOST, types.ELECTRIC, types.ICE, types.DARK],
        resist: [types.PLANT, types.PSYCHO, types.BATTLE, types.GROUND]
    },
    '41': {
        id: '250',
        types: [types.FIRE, types.FLIGHT],
        weak: [types.STONE, types.WATER, types.ELECTRIC],
        resist: [types.GROUND, types.BUG, types.PLANT, types.BATTLE, types.STEEL, types.FIRE, types.FAIRY]
    },
    '33': {
        id: '251',
        types: [types.PSYCHO, types.PLANT],
        weak: [types.BUG, types.FLIGHT, types.POISON, types.GHOST, types.FIRE, types.ICE, types.DARK],
        resist: [types.BATTLE, types.GROUND, types.WATER, types.PLANT, types.ELECTRIC, types.PSYCHO]
    }
};

module.exports = function (json) {
    if (!json) {
        return null;
    }
    try {
        var input = JSON.parse(json);
    } catch (e) {
        return null;
    }
    if (!input.monsters || !input.monsters.length) {
        return null;
    }

    var stepsPerDay = input.steps ? input.steps.split(',') : [];
    var sleepPerDay = input.sleep ? input.sleep.split(',') : [];
    var restfulPerDay = input.restful ? input.restful.split(',') : [];
    var activePerDay = input.active ? input.active.split(',') : [];

    var monsters = [];
    for (var i = 0; i < 6; i++) {
        if (input.monsters[i] === undefined) {
            break;
        }
        var monster = mapper[input.monsters[i].toString()];
        if (!monster) {
            break;
        }
        var steps = Math.abs(parseInt(stepsPerDay[i]) || 0);
        var sleep = Math.abs(parseInt(sleepPerDay[i]) || 0);
        var restful = Math.abs(parseInt(restfulPerDay[i]) || 0);
        var active = Math.abs(parseInt(activePerDay[i]) || 0);
        monsters.push({
            id: monster.id,
            types: monster.types,
            weak: monster.weak,
            resist: monster.resist,
            level: Math.max(1, Math.min(100, (steps + sleep) / 1.5)),
            power: Math.max(10, Math.min(100, (steps + active + restful) / 1.5)),
            offense: Math.max(100, Math.min(350, steps * 4)),
            defense: Math.max(100, Math.min(450, sleep * 5)),
            agility: Math.max(0, Math.min(20, Math.round(restful / 2))),
            critical: Math.max(0, Math.min(100, active)),
            health: 100
        })
    }

    if (!monsters.length) {
        return null;
    }

    return {
        monsters: monsters
    };
};
