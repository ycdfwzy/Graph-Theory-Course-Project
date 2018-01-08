/**
 * Created by Gerwa on 2017/12/28.
 */

const color_list = ["#FF0EF0",//用于染色的调色板
    "#FF0DF0",
    "#FF0CF0",
    "#FF0BF0",
    "#FF0AF0",
    "#FF09F0",
    "#FF08F0",
    "#FF07F0",
    "#FF06F0",
    "#FF05F0",
    "#FF04F0",
    "#FF03F0",
    "#FF02F0",
    "#FF01F0",
    "#FF00F0",
    "#FF00E0",
    "#FF00D0",
    "#FF00C0",
    "#FF00B0",
    "#FF00A0",
    "#FF0090",
    "#FF0080",
    "#FF0070",
    "#FF0060",
    "#FF0050",
    "#FF0040",
    "#FF0030",
    "#FF0020",
    "#FF0010",
    "#FF0000",
    "#FF0a00",
    "#FF1400",
    "#FF1e00",
    "#FF2800",
    "#FF3200",
    "#FF3c00",
    "#FF4600",
    "#FF5000",
    "#FF5a00",
    "#FF6400",
    "#FF6e00",
    "#FF7800",
    "#FF8200",
    "#FF8c00",
    "#FF9600",
    "#FFa000",
    "#FFaa00",
    "#FFb400",
    "#FFbe00",
    "#FFc800",
    "#FFd200",
    "#FFdc00",
    "#FFe600",
    "#FFf000",
    "#FFfa00",
    "#fdff00",
    "#d7ff00",
    "#b0ff00",
    "#8aff00",
    "#65ff00",
    "#3eff00",
    "#17ff00",
    "#00ff10",
    "#00ff36",
    "#00ff5c",
    "#00ff83",
    "#00ffa8",
    "#00ffd0",
    "#00fff4",
    "#00e4ff",
    "#00d4ff",
    "#00c4ff",
    "#00b4ff",
    "#00a4ff",
    "#0094ff",
    "#0084ff",
    "#0074ff",
    "#0064ff",
    "#0054ff",
    "#0044ff",
    "#0032ff",
    "#0022ff",
    "#0012ff",
    "#0002ff",
    "#0000ff",
    "#0100ff",
    "#0200ff",
    "#0300ff",
    "#0400ff",
    "#0500ff"];

const svg = d3.select("svg"),
    width = +svg.attr("width"),
    height = +svg.attr("height");

const close_color = d3.scaleOrdinal().range(color_list),
    between_color = d3.scaleOrdinal().range(color_list);

const simulation = d3.forceSimulation()
    .force("link", d3.forceLink().id(function (d) {
        return d.id;
    }))
    .force("charge", d3.forceManyBody())
    .force("center", d3.forceCenter(width / 2, height / 2))
    .force("x", d3.forceX())
    .force("y", d3.forceY());

$("a").click((event) => {
    if (shared.working) {
        event.preventDefault();
    }
});

nprogress.start();
shared.working = true;
cpp.centrality(render);

function render(error, graph) {
    nprogress.done();
    shared.working = false;
    if (error) {
        alert(error.message);
        throw error;
    }

    //建立颜色的映射
    close_color.domain(graph.nodes.map(x => x.closeness).sort((a, b) => b - a));
    between_color.domain(graph.nodes.map(x => x.betweenness).sort((a, b) => b - a));
    global.max_close = Math.max.apply(null, graph.nodes.map(x => x.closeness));
    global.max_between = Math.max.apply(null, graph.nodes.map(x => x.betweenness));

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
            return Math.log10(d.value) / 3 + 1;
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
            $('#num_betweenness').text(d.betweenness);
            $('#num_closeness').text(d.closeness);
            $('#information').text(d.id);
        });

    $('#color-type').change(() => {
        const value = $('#color-type').find(':selected').val();
        if (value === 'between') {
            node.attr("r", function (d) {
                return 5 + d.betweenness / global.max_between * 10;
            }).attr("fill", function (d) {
                return between_color(d.betweenness);
            });
        } else {
            node.attr("r", function (d) {
                return 5 + d.closeness / global.max_close * 5;
            }).attr("fill", function (d) {
                return close_color(d.closeness);
            });
        }
    }).change();

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