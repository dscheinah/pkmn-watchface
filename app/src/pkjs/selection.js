function get() {
    return window.selected || [];
}
get.toString = function () {
    return 'function(){return window.selected||[]}'
};

function set() {
}
set.toString = function () {
    return 'function(){}';
};

var component = {
    name: 'selection',
    template: '<div id="selection" class="component pokedex"></div>',
    manipulator: {get: get, set: set}
};

module.exports = {
    init: function (clay) {
        clay.registerComponent(component);
    }
};
