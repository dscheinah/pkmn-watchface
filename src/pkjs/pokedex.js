function get() {
    return this.x || ['', ''];
}
get.toString = function () {
    return 'function(){return this.x||["",""]}';
};
function set(index) {
    this.x = index;
    if (!this.i) {
        return;
    }
    var html = '';
    var length = this.m.length;
    for (var i = 0; i < length; i++) {
        var entry = this.m[i];
        if ((parseInt(index[entry < 32 ? 0 : 1]) || 0) & (1 << (entry < 32 ? entry : entry - 32))) {
            html += this.e + this.p[entry + 1] + this.t;
        } else {
            html += this.u;
        }
    }
    document.getElementById('pokedex').innerHTML = html;
}
set.toString = function () {
    return 'function(a){if(this.x=a,!!this.i){for(var b,c="",d=this.m.length,e=0;e<d;e++)b=this.m[e],c+=(parseInt(a[32>b?0:1])||0)&1<<(32>b?b:b-32)?this.e+this.p[b+1]+this.t:this.u;document.getElementById("pokedex").innerHTML=c}}';
};

function setData(userData) {
    this.i = true;
    this.p = userData.pokedex;
    this.m = userData.pokedex_mapper;
    this.e = '<div><img src="data:image;base64,';
    this.t = '"/></div>';
    this.u = '<div><span>?</span></div>';
    if (this.x) {
        this.set(this.x);
    }
}
setData.toString = function () {
    return 'function(a){this.i=!0,this.p=a.pokedex,this.m=a.pokedex_mapper,this.e="<div><img src=\\"data:image;base64,",this.t="\\"/></div>",this.u="<div><span>?</span></div>",this.x&&this.set(this.x)}';
};

var component = {
    name: 'pokedex',
    template: '<div id="pokedex" class="component pokedex"></div>',
    manipulator: {get: get, set: set, setData: setData},
    style:
        '.pokedex { display: flex; flex-wrap: wrap; margin-bottom: 80px; }' +
        '.pokedex div { margin: 1rem; display: flex; align-items: center; justify-content: center; background: white; border-radius: .25rem; width: 64px; height: 64px; }' +
        '.pokedex div span { font-size: 32px; color: rgba(0, 0, 0, .2); }'
};

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
            /* 039 */ 43,
            /* 040 */ 44,
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
            /* 143 */ 46,
            /* 144 */ 37,
            /* 145 */ 38,
            /* 146 */ 39,
            /* 150 */ 42,
            /* 151 */ 25,
            /* 172 */ 34,
            /* 174 */ 45,
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
};
