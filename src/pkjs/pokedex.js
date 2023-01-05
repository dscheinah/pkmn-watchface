var component = {
    name: 'pokedex',
    template: '<div class="component pokedex"></div>',
    manipulator: {
        get: function () {
            return '';
        },
        set: function (index) {
            this.index = index;
            if (!this.pokedex) {
                return;
            }
            var html = '';
            var length = this.pokedex_mapper.length;
            for (var i = 0; i < length; i++) {
                var entry = this.pokedex_mapper[i];
                if ((parseInt(index[entry < 32 ? 0 : 1]) || 0) & (1 << (entry < 32 ? entry : entry - 32))) {
                    html += '<div><img src="data:image/png;base64,' + this.pokedex[entry + 1] + '"/></div>';
                } else {
                    html += '<div><span>?</span></div>';
                }
            }
            this.$element.set('innerHTML', html);
        },
        setUserData: function (userData) {
            this.pokedex = userData.pokedex;
            this.pokedex_mapper = userData.pokedex_mapper;
            if (this.index) {
                this.set(this.index);
            }
        }
    },
    style:
        '.pokedex { display: flex; flex-wrap: wrap; gap: 1rem; }' +
        '.pokedex div { display: flex; align-items: center; justify-content: center; background: white; border-radius: .25rem; width: 64px; height: 64px; }' +
        '.pokedex div span { font-size: 32px; color: rgba(0, 0, 0, .2); }'
}

module.exports = {
    init: function (clay) {
        clay.registerComponent(component);
        clay.setSettings('pokedex', [localStorage.getItem('pokedex0'), localStorage.getItem('pokedex1')]);
    },
    pebble: function (clay, payload) {
        var index = [payload['pokedex0'].toString(), payload['pokedex1'].toString()];
        localStorage.setItem('pokedex0', index[0]);
        localStorage.setItem('pokedex1', index[1]);
        clay.setSettings('pokedex', index);
    },
    userData: {
        pokedex: require('./pokedex.json'),
        pokedex_mapper: [
            /* 001 */ 0,
            /* 002 */ 3,
            /* 003 */ 6,
            /* 004 */ 1,
            /* 005 */ 4,
            /* 006 */ 7,
            /* 007 */ 2,
            /* 008 */ 5,
            /* 009 */ 8,
            /* 025 */ 9,
            /* 026 */ 10,
            /* 063 */ 11,
            /* 064 */ 12,
            /* 065 */ 13,
            /* 086 */ 14,
            /* 087 */ 15,
            /* 092 */ 16,
            /* 093 */ 17,
            /* 094 */ 18,
            /* 132 */ 19,
            /* 133 */ 20,
            /* 134 */ 21,
            /* 135 */ 24,
            /* 136 */ 22,
            /* 144 */ 37,
            /* 145 */ 38,
            /* 146 */ 39,
            /* 150 */ 42,
            /* 151 */ 25,
            /* 172 */ 34,
            /* 175 */ 35,
            /* 176 */ 36,
            /* 196 */ 23,
            /* 197 */ 26,
            /* 203 */ 27,
            /* 213 */ 28,
            /* 214 */ 29,
            /* 225 */ 30,
            /* 235 */ 31,
            /* 241 */ 32,
            /* 249 */ 40,
            /* 250 */ 41,
            /* 251 */ 33
        ]
    }
}
