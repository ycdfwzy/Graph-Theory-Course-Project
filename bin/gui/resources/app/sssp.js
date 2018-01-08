/**
 * Created by Gerwa on 2017/12/28.
 */
const svg = d3.select("svg"),
    width = +svg.attr("width"),
    height = +svg.attr("height");

const color = d3.scaleOrdinal(d3.schemeCategory20);

const simulation = d3.forceSimulation()
    .force("link", d3.forceLink().id(function (d) {
        return d.id;
    }))
    .force("charge", d3.forceManyBody())
    .force("center", d3.forceCenter(width / 2, height / 2))
    .force("x", d3.forceX())
    .force("y", d3.forceY());

$('#Compute').click((event) => {
    if (shared.working) {
        return;
    }
    const source = $('#source').text(), target = $('#target').text();
    if (!source || !target) {
        return;
    }
    shared.working = true;
    nprogress.start();
    cpp.single_source_shortest_path(source, target, render);
});

$("a").click((event) => {
    if (shared.working) {
        event.preventDefault();
    }
});

$('#Set_Source').click(function () {
    $('#source').text($('#information').text());
});

$('#Set_Target').click(function () {
    $('#target').text($('#information').text());
});

shared.working = true;
nprogress.start();
cpp.display_graph(render);

function render(error, graph) {
    nprogress.done();
    shared.working = false;
    if (error) {
        alert(error.message);
        throw error;
    }

    if (graph.distance !== null) {
        $('#distance').text(graph.distance);
    }
    if (graph.display !== null) {
        $('#display').text(graph.display);
    }

    let link_g = svg.select("g.links");
    if (!link_g.size()) {
        link_g = svg.append("g")
            .classed("links", true);
    }

    const link_data = link_g
        .selectAll("line")
        .data(graph.links);

    link_data.exit().remove();
    link_data.enter().append("line");

    const link = link_g.selectAll("line")
        .attr("stroke-width", function (d) {
            if (d.path) {
                return 5;
            }
            return Math.log10(d.value) / 3 + 1;
        }).attr("stroke-opacity", function (d) {
            return d.path ? 1 : 0.6;
        }).attr("stroke", function (d) {
            return d.path ? "#000" : "#999";
        });

    let node_g = svg.select("g.nodes");
    if (!node_g.size()) {
        node_g = svg.append("g")
            .classed("nodes", true);
    }

    const node_data = node_g
        .selectAll("circle")
        .data(graph.nodes);

    node_data.exit().remove();
    node_data.enter().append("circle");

    const node = node_g.selectAll("circle")
        .attr("r", function (d) {
            if (d.status === 1) {
                return 7;
            }
            if (d.status === 2) {
                return 9;
            }
            return 5;
        }).attr("fill", function (d) {
            if (d.status === 1) {
                return color(3);
            }
            if (d.status === 2) {
                return color(7);
            }
            return color(1);
        })
        .call(d3.drag()
            .on("start", dragstarted)
            .on("drag", dragged)
            .on("end", dragended))
        .on("click", function (d) {
            if (global.previous_node) {
                global.previous_node.classed('selected', false);
            }
            d3.select(this).classed('selected', true);
            global.previous_node = d3.select(this);
            $('#information').text(d.id);
        });

    node.append("title")
        .text(function (d) {
            return d.id;
        });

    simulation
        .nodes(graph.nodes)
        .on("tick", ticked);

    simulation.force("link")
        .links(graph.links);

    function ticked() {
        link
            .attr("x1", function (d) {
                return d.source.x;
            })
            .attr("y1", function (d) {
                return d.source.y;
            })
            .attr("x2", function (d) {
                return d.target.x;
            })
            .attr("y2", function (d) {
                return d.target.y;
            });

        node
            .attr("cx", function (d) {
                return d.x;
            })
            .attr("cy", function (d) {
                return d.y;
            });
    }
}

function dragstarted(d) {
    if (!d3.event.active) simulation.alphaTarget(0.3).restart();
    d.fx = d.x;
    d.fy = d.y;
}

function dragged(d) {
    d.fx = d3.event.x;
    d.fy = d3.event.y;
}

function dragended(d) {
    if (!d3.event.active) simulation.alphaTarget(0);
    d.fx = null;
    d.fy = null;
}