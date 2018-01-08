/**
 * Created by Gerwa on 2017/12/29.
 */

const ffi = require('ffi'),
    path = require('path'),
    ref = require('ref');

const libgerw = ffi.Library(path.join(__dirname, /^win/.test(process.platform) ? "gerw" : "libgerw"), {
    'get_str': ['string', ['pointer']],
    'free_str': ['void', ['pointer']],
    'load_graph': ['pointer', []],
    'get_display': ['pointer', ['string']],
    'connected_component': ['pointer', ['string', 'int']],
    'Centrality': ['pointer', ['string']],
    'Single_Source_Shortest_Path': ['pointer', ['string', 'string', 'string']],
    'Minimum_Spanning_Tree': ['pointer', ['string']]
});

function json_wrapper(dll, callback) {
    return (err, res) => {
        if (err)
            callback(err, null);
        const result = JSON.parse(dll.get_str(res));
        dll.free_str(res);
        if (result.okay)
            callback(null, result.result);
        else
            callback(new Error(result.message), null);
    };
}


module.exports = class {
    constructor() {
        const str_ptr = libgerw.load_graph();
        const result = JSON.parse(libgerw.get_str(str_ptr));
        libgerw.free_str(str_ptr);
        if (result.okay) {
            this.graph_str = JSON.stringify(result.result);
        } else {
            console.log(result.message);
        }
    }

    display_graph(callback) {
        libgerw.get_display.async(this.graph_str, json_wrapper(libgerw, callback));
    }

    connected_component(threshold, callback) {
        libgerw.connected_component.async(this.graph_str, threshold, json_wrapper(libgerw, callback));
    }

    centrality(callback) {
        libgerw.Centrality.async(this.graph_str, json_wrapper(libgerw, callback));
    }

    single_source_shortest_path(source, target, callback) {
        libgerw.Single_Source_Shortest_Path.async(this.graph_str, source, target, json_wrapper(libgerw, callback));
    }

    minium_spanning_tree(callback) {
        libgerw.Minimum_Spanning_Tree.async(this.graph_str, json_wrapper(libgerw, callback));
    }
};
