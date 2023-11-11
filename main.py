import matplotlib.pyplot as plt
import networkx as nx

# Read the graph data from the exported file
def read_graph_data(filename):
    graph_data = {}
    with open(filename, 'r') as file:
        for line in file:
            parts = line.strip().split(':')
            vertex = int(parts[0])
            neighbors = list(map(int, parts[1].split()))
            graph_data[vertex] = neighbors
    return graph_data

# Create a networkx graph from the data
def create_networkx_graph(graph_data):
    G = nx.Graph()
    for vertex, neighbors in graph_data.items():
        G.add_node(vertex)
        G.add_edges_from([(vertex, neighbor) for neighbor in neighbors])
    return G

# Visualize the networkx graph using matplotlib
def visualize_networkx_graph(G, output_filename):
    plt.figure(figsize=(8, 8))
    pos = nx.spring_layout(G, seed=42)  # You can choose different layout algorithms

    nx.draw(G, pos, with_labels=True, node_size=200, node_color='skyblue', font_size=10, font_color='black', edge_color='gray')
    plt.axis('off')
    plt.savefig(output_filename + ".png", format="PNG")
    plt.show()

if __name__ == "__main__":
    # Load the graph data from the exported file
    graph_data = read_graph_data("graph_data(23).txt")

    # Create a networkx graph
    G = create_networkx_graph(graph_data)

    # Visualize the graph using matplotlib
    visualize_networkx_graph(G, "graph_visualization9")

    # Load the graph data from the exported file
    graph_data = read_graph_data("graph_data(24).txt")

    # Create a networkx graph
    G = create_networkx_graph(graph_data)

    # Visualize the graph using matplotlib
    visualize_networkx_graph(G, "graph_visualization10")
