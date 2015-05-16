module.exports = exports = {}
exports.addon_loaded = false

try {
    var binary = require('node-pre-gyp')
    var path = require('path')
    var binding_path = binary.find(path.resolve(path.join(__dirname, './package.json')))
    var binding = require(binding_path)

    exports.CloseMainWindowsByProcess = binding.CloseMainWindowsByProcess
    exports.addon_loaded = true
} catch (err) {
}
