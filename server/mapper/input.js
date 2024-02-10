var types = {
    PLANT: 1,
    FIRE: 2,
    WATER: 3
};

var mapper = {
    1: {
        id: 1,
        types: [types.PLANT],
        weak: [types.FIRE],
        resist: [types.PLANT, types.WATER]
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
        var monster = mapper[input.monsters[i]];
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
