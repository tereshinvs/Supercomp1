#include <boost/graph/use_mpi.hpp>
#include <boost/graph/distributed/mpi_process_group.hpp>
#include <boost/mpi/timer.hpp>

#include <boost/graph/distributed/adjacency_list.hpp>
#include <boost/graph/distributed/dehne_gotz_min_spanning_tree.hpp>
#include <boost/graph/distributed/vertex_list_adaptor.hpp>
#include <boost/graph/parallel/distribution.hpp>

#include <boost/graph/metis.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

typedef std::pair<int, int> E;

typedef boost::adjacency_list<
	boost::listS,
	boost::distributedS<
		boost::graph::distributed::mpi_process_group,
		boost::vecS
	>,
	boost::undirectedS,
	boost::no_property,
	boost::property<
		boost::edge_weight_t,
		float
	>
> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;

#ifdef BOOST_NO_EXCEPTIONS
void boost::throw_exception(std::exception const& ex) {
    std::cout << ex.what() << std::endl;
    abort();
}
#endif

int main(int argc, char* argv[])
{
	boost::mpi::environment env(argc, argv);
	boost::mpi::communicator world;
	boost::graph::distributed::mpi_process_group process_group(world);

	std::cout << boost::graph::distributed::process_id(process_group) << std::endl;
	std::cout << "Reading graph..." << std::endl;

	std::string filename = "big.graph", dist_filename = "big.part";
	std::ifstream in(filename.c_str()), dist_in(dist_filename.c_str());

	boost::graph::metis_reader reader(in);
//	boost::graph::metis_distribution dist(dist_in, process_id(process_group));

	std::cout << "Creating graph..." << std::endl;

	Graph graph(
		reader.begin(), reader.end(),
		reader.weight_begin(),
		reader.num_vertices()
//		process_group,
//		dist
	);

	typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
	WeightMap weight_map = boost::get(boost::edge_weight, graph);

	std::vector<Edge> mst_edges;

	std::cout << "Starting..." << std::endl;
	boost::mpi::timer timer;

	boost::graph::distributed::dense_boruvka_minimum_spanning_tree(
		boost::graph::make_vertex_list_adaptor(graph),
	    weight_map,
	    std::back_inserter(mst_edges)
	);

	if (process_id(graph.process_group()) == 0) {
		std::cout << "MST size: " << mst_edges.size() << std::endl;
		std::cout << "Time ellapsed: " << timer.elapsed() << std::endl;
	}

	return 0;
}
