#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graph_traits.hpp>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>   
#include <sstream>  

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef std::pair<int, int> Edge;

class OnlineEdgeDeletion {
private:
    Graph G;
    std::vector<Edge> edges;
    std::vector<int> parent;
	std::vector<int> rank;  
	std::set<int> deletedEdges; 

     int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

public:
    OnlineEdgeDeletion(int numVertices) : G(numVertices), rank(numVertices, 0) {
        for (int i = 0; i < numVertices; ++i)
            parent.push_back(i);
    }

    void addEdge(int u, int v) {
        edges.push_back({u, v});
        add_edge(u, v, G);
    }

    bool areVerticesConnected(int u, int v) {
        return find(u) == find(v);
    }

    void deleteEdge(int edgeIndex) {
		if (deletedEdges.count(edgeIndex) > 0) {
            
            return;
        }
		
        int u = edges[edgeIndex].first;
        int v = edges[edgeIndex].second;

        int rootU = find(u);
        int rootV = find(v);

        if (rootU != rootV) {
            if (rank[rootU] < rank[rootV]) {
                std::swap(rootU, rootV);
            }
            parent[rootV] = rootU;
            if (rank[rootU] == rank[rootV]) {
                rank[rootU]++;
            }
            remove_edge(u, v, G);
			
			
            deletedEdges.insert(edgeIndex);
        }
    }

    const std::vector<Edge>& getEdges() const {
        return edges;
    }

    const Graph& getGraph() const {
        return G;
    }
	
	
};


void exportGraphData(const Graph& G, const std::string& filename) {
    std::ofstream dataFile(filename + ".txt");
    if (!dataFile.is_open()) {
        std::cerr << "Error: Unable to open data file for writing." << std::endl;
        return;
    }

    
    for (auto vertexIt = vertices(G); vertexIt.first != vertexIt.second; ++vertexIt.first) {
        Vertex v = *vertexIt.first;
        dataFile << v << ": ";

        for (auto neighborIt = adjacent_vertices(v, G); neighborIt.first != neighborIt.second; ++neighborIt.first) {
            Vertex u = *neighborIt.first;
            dataFile << u << " ";
        }

        dataFile << "\n";
    }

    dataFile.close();
    std::cout << "Graph data saved as " << filename << ".txt" << std::endl;
}


int main() {
    int numVertices, numEdges;
    std::cout << "Enter the number of vertices: ";
    std::cin >> numVertices;
    std::cout << "Enter the number of edges: ";
    std::cin >> numEdges;

    OnlineEdgeDeletion graph(numVertices);

	
	std::set<Edge> generatedEdges;  

    
    std::srand(static_cast<unsigned int>(std::time(0)));
    while (generatedEdges.size() < numEdges) {
        int u = std::rand() % numVertices;
        int v = std::rand() % numVertices;
        if (u != v) {
            
            Edge edge = std::make_pair(std::min(u, v), std::max(u, v));
            if (generatedEdges.count(edge) == 0) {
                
                generatedEdges.insert(edge);
                graph.addEdge(u, v);
            }
        }
    }
	
    int fileCounter = 1;
    std::ifstream counterFile("file_counter.txt");
    if (counterFile.is_open()) {
       counterFile >> fileCounter;
       counterFile.close();
    }
	
	
    std::string initialFilename = "graph_data(" + std::to_string(static_cast<long long>(fileCounter++)) + ")";
    exportGraphData(graph.getGraph(), initialFilename);

   

	
	
    int numDeletions;
    std::cout << "Enter the number of edge deletions: ";
    std::cin >> numDeletions;
	
	
    std::vector<int> uniqueEdgeIndices(graph.getEdges().size());
    for (int i = 0; i < graph.getEdges().size(); ++i) {
       uniqueEdgeIndices[i] = i;
    }

    
    std::random_shuffle(uniqueEdgeIndices.begin(), uniqueEdgeIndices.end());


    for (int i = 0; i < numDeletions; ++i) {
        int edgeIndex = uniqueEdgeIndices[i];
		int u = graph.getEdges()[edgeIndex].first;
        int v = graph.getEdges()[edgeIndex].second;
        graph.deleteEdge(edgeIndex);
        std::cout << "Deleted edge: " << graph.getEdges()[edgeIndex].first << " - " << graph.getEdges()[edgeIndex].second << std::endl;
		
		std::vector<int> component(numVertices);
        int numComponents = connected_components(graph.getGraph(), &component[0]);
		
        bool isConnected = graph.areVerticesConnected(u, v);
        if (component[u] == component[v]) {
            std::cout << "Vertices " << u << " and " << v << " are in the same connected component." << std::endl;
        } else {
            std::cout << "Vertices " << u << " and " << v << " are NOT in the same connected component." << std::endl;
        }
    }
	
	
    std::string afterDeletionFilename = "graph_data(" + std::to_string(static_cast<long long>(fileCounter++)) + ")";
    exportGraphData(graph.getGraph(), afterDeletionFilename);
  
    
    std::ofstream updatedCounterFile("file_counter.txt");
    if (updatedCounterFile.is_open()) {
      updatedCounterFile << fileCounter;
      updatedCounterFile.close();
	}

    return 0;
}
