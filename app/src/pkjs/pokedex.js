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
            html += this.e + this.p[entry + 1] + this.v + entry + this.t;
        } else {
            html += this.u;
        }
    }
    var pokedex = document.getElementById('pokedex');
    pokedex.innerHTML = html;

    pokedex.addEventListener('click', function (event) {
        var clickedId = parseInt(event.target && event.target.dataset.id);
        if (!clickedId) {
            return;
        }
        var index = window.selected.indexOf(clickedId);
        if (index > -1) {
            window.selected.splice(index, 1);
        } else {
            window.selected.push(clickedId);
            if (window.selected.length > 6) {
                window.selected.shift();
            }
        }

        document.querySelectorAll('.pokedex-selected').forEach(function (node) {
            node.className = '';
        });

        var selectedHtml = '';
        var selectedLength = window.selected.length;
        for (var j = 0; j < selectedLength; j++) {
            var id = window.selected[j];
            selectedHtml += this.e + this.p[id + 1] + this.t;
            document.querySelectorAll('[data-id="' + id + '"]').forEach(function (node) {
                node.parentNode.className = 'pokedex-selected';
            });
        }
        document.getElementById('selection').innerHTML = selectedHtml;
    }.bind(this));
}
set.toString = function () {
    return 'function(e){if(this.x=e,this.i){for(var t="",i=this.m.length,n=0;n<i;n++){var s=this.m[n];(parseInt(e[s<32?0:1])||0)&1<<(s<32?s:s-32)?t+=this.e+this.p[s+1]+this.v+s+this.t:t+=this.u}var d=document.getElementById("pokedex");d.innerHTML=t,d.addEventListener("click",function(e){var t=parseInt(e.target&&e.target.dataset.id);if(t){var i=window.selected.indexOf(t);i>-1?window.selected.splice(i,1):(window.selected.push(t),window.selected.length>6&&window.selected.shift()),document.querySelectorAll(".pokedex-selected").forEach((function(e){e.className=""}));for(var n="",s=window.selected.length,d=0;d<s;d++){var c=window.selected[d];n+=this.e+this.p[c+1]+this.t,document.querySelectorAll(\'[data-id="\'+c+\'"]\').forEach((function(e){e.parentNode.className="pokedex-selected"}))}document.getElementById("selection").innerHTML=n}}.bind(this))}}';
};

function setData(userData) {
    window.selected = [];
    this.i = true;
    this.p = userData.pokedex;
    this.m = userData.pokedex_mapper;
    this.e = '<div><img src="data:image;base64,';
    this.v = '" data-id="';
    this.t = '"/></div>';
    this.u = '<div><span>?</span></div>';
    if (this.x) {
        this.set(this.x);
    }
}
setData.toString = function () {
    return 'function(i){window.selected=[],this.i=!0,this.p=i.pokedex,this.m=i.pokedex_mapper,this.e=\'<div><img src="data:image;base64,\',this.v=\'" data-id="\',this.t=\'"/></div>\',this.u="<div><span>?</span></div>",this.x&&this.set(this.x)}';
};

var component = {
    name: 'pokedex',
    template: '<div id="pokedex" class="component pokedex"></div>',
    manipulator: {get: get, set: set, setData: setData},
    style:
        '.pokedex { display: flex; flex-wrap: wrap; }' +
        '.pokedex div { margin: .4rem; display: flex; align-items: center; justify-content: center; background: white; border-radius: .25rem; width: 64px; height: 64px; border: 3px solid white; }' +
        '.pokedex div span { font-size: 32px; color: rgba(0, 0, 0, .2); }' +
        '.pokedex .pokedex-selected { border-color: lightgreen; }' +
        '#main-form > .component:last-child, #main-form > .section:last-child { margin-bottom: 80px; }'
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
            /* 125 */ 53,
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
            /* 185 */ 52,
            /* 196 */ 23,
            /* 197 */ 26,
            /* 200 */ 55,
            /* 203 */ 27,
            /* 213 */ 28,
            /* 214 */ 29,
            /* 216 */ 47,
            /* 217 */ 48,
            /* 225 */ 30,
            /* 235 */ 31,
            /* 239 */ 54,
            /* 241 */ 32,
            /* 243 */ 49,
            /* 244 */ 50,
            /* 245 */ 51,
            /* 249 */ 40,
            /* 250 */ 41,
            /* 251 */ 33
        ]
    }
};
