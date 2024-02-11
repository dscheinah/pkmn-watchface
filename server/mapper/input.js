var types = {
    PLANT: 1,
    FIRE: 2,
    WATER: 3
};

var mapper = {
    '0': {
        id: '001',
        types: [types.PLANT],
        weak: [types.FIRE],
        resist: [types.PLANT, types.WATER]
    },
    '3': {
        id: '002',
        types: [],
        weak: [],
        resist: []
    },
    '6': {
        id: '003',
        types: [],
        weak: [],
        resist: []
    },
    '1': {
        id: '004',
        types: [],
        weak: [],
        resist: []
    },
    '4': {
        id: '005',
        types: [],
        weak: [],
        resist: []
    },
    '7': {
        id: '006',
        types: [],
        weak: [],
        resist: []
    },
    '2': {
        id: '007',
        types: [],
        weak: [],
        resist: []
    },
    '5': {
        id: '008',
        types: [],
        weak: [],
        resist: []
    },
    '8': {
        id: '009',
        types: [],
        weak: [],
        resist: []
    },
    '9': {
        id: '025',
        types: [],
        weak: [],
        resist: []
    },
    '10': {
        id: '026',
        types: [],
        weak: [],
        resist: []
    },
    '43': {
        id: '039',
        types: [],
        weak: [],
        resist: []
    },
    '44': {
        id: '040',
        types: [],
        weak: [],
        resist: []
    },
    '11': {
        id: '063',
        types: [],
        weak: [],
        resist: []
    },
    '12': {
        id: '064',
        types: [],
        weak: [],
        resist: []
    },
    '13': {
        id: '065',
        types: [],
        weak: [],
        resist: []
    },
    '14': {
        id: '086',
        types: [],
        weak: [],
        resist: []
    },
    '15': {
        id: '087',
        types: [],
        weak: [],
        resist: []
    },
    '16': {
        id: '092',
        types: [],
        weak: [],
        resist: []
    },
    '17': {
        id: '093',
        types: [],
        weak: [],
        resist: []
    },
    '18': {
        id: '094',
        types: [],
        weak: [],
        resist: []
    },
    '53': {
        id: '125',
        types: [],
        weak: [],
        resist: []
    },
    '19': {
        id: '132',
        types: [],
        weak: [],
        resist: []
    },
    '20': {
        id: '133',
        types: [],
        weak: [],
        resist: []
    },
    '21': {
        id: '134',
        types: [],
        weak: [],
        resist: []
    },
    '24': {
        id: '135',
        types: [],
        weak: [],
        resist: []
    },
    '22': {
        id: '136',
        types: [],
        weak: [],
        resist: []
    },
    '46': {
        id: '143',
        types: [],
        weak: [],
        resist: []
    },
    '37': {
        id: '144',
        types: [],
        weak: [],
        resist: []
    },
    '38': {
        id: '145',
        types: [],
        weak: [],
        resist: []
    },
    '39': {
        id: '146',
        types: [],
        weak: [],
        resist: []
    },
    '42': {
        id: '150',
        types: [],
        weak: [],
        resist: []
    },
    '25': {
        id: '151',
        types: [],
        weak: [],
        resist: []
    },
    '34': {
        id: '172',
        types: [],
        weak: [],
        resist: []
    },
    '45': {
        id: '174',
        types: [],
        weak: [],
        resist: []
    },
    '35': {
        id: '175',
        types: [],
        weak: [],
        resist: []
    },
    '36': {
        id: '176',
        types: [],
        weak: [],
        resist: []
    },
    '52': {
        id: '185',
        types: [],
        weak: [],
        resist: []
    },
    '23': {
        id: '196',
        types: [],
        weak: [],
        resist: []
    },
    '26': {
        id: '197',
        types: [],
        weak: [],
        resist: []
    },
    '55': {
        id: '200',
        types: [],
        weak: [],
        resist: []
    },
    '27': {
        id: '203',
        types: [],
        weak: [],
        resist: []
    },
    '28': {
        id: '213',
        types: [],
        weak: [],
        resist: []
    },
    '29': {
        id: '214',
        types: [],
        weak: [],
        resist: []
    },
    '47': {
        id: '216',
        types: [],
        weak: [],
        resist: []
    },
    '48': {
        id: '217',
        types: [],
        weak: [],
        resist: []
    },
    '30': {
        id: '225',
        types: [],
        weak: [],
        resist: []
    },
    '31': {
        id: '235',
        types: [],
        weak: [],
        resist: []
    },
    '54': {
        id: '239',
        types: [],
        weak: [],
        resist: []
    },
    '32': {
        id: '241',
        types: [],
        weak: [],
        resist: []
    },
    '49': {
        id: '243',
        types: [],
        weak: [],
        resist: []
    },
    '50': {
        id: '244',
        types: [],
        weak: [],
        resist: []
    },
    '51': {
        id: '245',
        types: [],
        weak: [],
        resist: []
    },
    '40': {
        id: '249',
        types: [],
        weak: [],
        resist: []
    },
    '41': {
        id: '250',
        types: [],
        weak: [],
        resist: []
    },
    '33': {
        id: '251',
        types: [],
        weak: [],
        resist: []
    }
};

module.exports = function (json) {
    var input = JSON.parse(json);

    var steps = input.steps.split(',');
    var sleep = input.sleep.split(',');
    var restful = input.restful.split(',');
    var active = input.active.split(',');

    var monsters = [];
    for (var i = 0; i < 6; i++) {
        var monster = mapper[(input.monsters[i] || '').toString()];
        if (!monster) {
            break;
        }
        monsters.push({
            id: monster.id,
            types: monster.types,
            weak: monster.weak,
            resist: monster.resist,
            level: steps[i] || 0,
            agility: sleep[i] || 0,
            critical: restful[i] || 0,
            extra: active[i] || 0,
            health: 100
        })
    }

    return {
        monsters: monsters
    };
};
