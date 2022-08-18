#include "adjacency_list_graph.h"
#include <queue>
using namespace std;
namespace finding_max_flow
{
	adjacency_list_graph::adjacency_list_graph(const int i_length) : m_VerticesAmount(i_length + 1), m_EdgesAmount(0),
		m_verticesArray(make_empty_adjacency_list_graph()), m_edges_array(nullptr), m_deleted_edge(-1, -1, 0)
	{
	}

	int adjacency_list_graph::add_edge(const int u, const int v, const int i_weight)
	{
		m_EdgesAmount += 1;
		return m_verticesArray[u].add_to_list(u, v, i_weight);
	}

	int adjacency_list_graph::add_edge_no_direction(const int u, const int v, const int i_weight)
	{
		m_verticesArray[u].add_to_list(u, v, i_weight);
		m_EdgesAmount += 1;
		if (m_verticesArray[v].is_in_list(u) && this->get_edge_weight(v, u) == 0)
		{
			set_capacity(v, u, -i_weight);
		}
		else
		{
			m_verticesArray[v].add_to_list(v, u, 0);
			m_EdgesAmount += 1;
		}
		return 1;
	}

	bool adjacency_list_graph::remove_edge_no_direction(const int u, const int v)
	{
		bool res = false;
		if (is_value_valid(u) && is_value_valid(v))
			res = remove_edge(u, v);
		res = remove_edge(v, u);
		if (res) m_EdgesAmount--;
		return res;
	}

	vertex_list* adjacency_list_graph::make_empty_adjacency_list_graph() const
	{
		const auto array = new vertex_list[m_VerticesAmount];
		for (int i = 1; i < m_VerticesAmount; i++) {
			array[i].make_empty_list();
		}
		return array;
	}

	bool adjacency_list_graph::read_adjacency_list_graph(std::ifstream& i_input_file, int& i_u, int& i_v, adjacency_list_graph* i_residual_graph)
	{
		bool res = true;
		int size, dest, v, weight, edges_amount = 0;
		int idx = 0;
		int u = v = -1;
		i_input_file >> size;
		if (size < 1)
		{
			res = false;
		}
		m_VerticesAmount = size + 1;
		i_residual_graph->m_VerticesAmount = size + 1;
		if (!i_input_file.eof())
			i_input_file >> edges_amount;
		else res = false;
		m_verticesArray = make_empty_adjacency_list_graph();
		i_residual_graph->m_verticesArray = make_empty_adjacency_list_graph();
		m_edges_array = new edge_node * [edges_amount];
		i_residual_graph->m_edges_array = new edge_node * [edges_amount];
		if (!i_input_file.eof())
		{
			i_input_file >> i_u;
			if (!i_input_file.eof())
				i_input_file >> i_v;
			if (!is_value_valid(i_u) || !is_value_valid(i_v)) res = false;
		}
		else res = false;
		while (res && i_input_file >> u)
		{
			if (is_value_valid(u) && i_input_file >> v && is_value_valid(v))
			{
				if (!i_input_file.eof())
				{
					i_input_file >> weight;
					add_edge(u, v, weight);
					i_residual_graph->add_edge_no_direction(u, v, weight);
					m_edges_array[idx] = new edge_node(u, v, weight);
					i_residual_graph->m_edges_array[idx++] = new edge_node(u, v, weight);
				}
			}
			else
				res = false;
		}
		if (idx != edges_amount)
		{
			res = false;
		}
		m_EdgesAmount = edges_amount;
		return res;
	}

	int* adjacency_list_graph::run_bfs(const int i_source, int i_dest, int** i_parents) const
	{
		const auto queue = new vertex_list();
		const auto distance_array = new int[m_VerticesAmount + 1];
		const auto parents = new int[m_VerticesAmount + 1];

		distance_array[i_source] = 0;
		parents[i_source] = -1;
		for (int i = 1; i <= m_VerticesAmount; i++)
		{
			if (i != i_source) {
				distance_array[i] = INT_MAX;
			}
		}
		const auto source_vertex = new vertex_node(i_source);

		queue->enqueue(source_vertex);

		while (!queue->is_empty())
		{
			const vertex_node* current_vertex = queue->dequeue(); // currentVertex  = u
			const int current_vertex_value = current_vertex->m_edge->get_x();
			vertex_node* current_neighbor = m_verticesArray[current_vertex_value].get_head();

			while (current_neighbor != nullptr) { // currentNeighbor = v 
				if (distance_array[current_neighbor->m_edge->get_y()] == INT_MAX && current_neighbor->m_edge->m_weight > 0) {
					distance_array[current_neighbor->m_edge->get_y()] = distance_array[current_vertex_value] + 1;
					parents[current_neighbor->m_edge->get_y()] = current_vertex_value;
					auto u = m_verticesArray[current_neighbor->m_edge->get_y()].get_head();
					if (u != nullptr && u->m_edge->get_y() > 0)
					{
						auto cpy = new vertex_node(u->m_edge->get_x());
						queue->enqueue(cpy);
					}
				}
				current_neighbor = current_neighbor->m_next;
			}
			delete current_vertex;
		}
		delete queue;
		*i_parents = parents;
		return distance_array;
	}

	vector<int> adjacency_list_graph::fattest_path_dijkstra(const int i_source) const
	{
		const auto size = m_VerticesAmount;
		vector<int> fattest_dist(size + 1, 0);
		vector<int> parents(size + 1, -1);
		parents[i_source] = 0;
		priority_queue<pair<int, int>, vector<pair<int, int>>,greater<>> queue;
		vector <pair<int, int>> pairs;

		for (int i = 1; i < size; i++)
		{
			if (i != i_source) {
				auto p = make_pair(0, i);
				pairs.push_back(p);
			}
			else
			{
				auto p = make_pair(INT_MAX, i);
				pairs.push_back(p);
				queue.push(p);
			}
		}
		while (!queue.empty())
		{
			pair<int, int> temp = queue.top();
			pair<int, int> new_v;

			int u = temp.second;
			queue.pop();
			auto adj = m_verticesArray[u].get_head();
			while (adj != nullptr)
			{
				int v = adj->m_edge->get_y();
				if (fattest_dist[v] < min(temp.first, get_edge_weight(u, v)))
				{
					fattest_dist[v] = min(temp.first, get_edge_weight(u, v));
					new_v.first = fattest_dist[v];
					new_v.second = v;
					queue.push(new_v);
					parents[v] = u;
				}
				adj = adj->m_next;
			}
		}
		return parents;
	}

	int adjacency_list_graph::find_minimum_capacity_on_path(const int* i_parents, const int i_source, const int i_dest) const
	{
		int path_flow = INT_MAX;
		for (int v = i_dest; v != i_source; v = i_parents[v])
		{
			int u = i_parents[v];
			path_flow = std::min(path_flow, get_edge_weight(u, v));
		}
		return path_flow;
	}

	void adjacency_list_graph::print() const
	{
		for (int i = 1; i < m_VerticesAmount; i++)
		{
			m_verticesArray[i].print();
		}
	}

	int adjacency_list_graph::get_edge_weight(const int i_u, const int i_v) const
	{
		if (is_value_valid(is_value_valid(i_u) && is_value_valid(i_v) && is_adjacent(i_u, i_v)))
		{
			return m_verticesArray[i_u].get_node_by_data(i_v)->m_edge->get_weight();
		}
		return -1;
	}

	void adjacency_list_graph::set_capacity(const int i_u, const int i_v, const int i_val) const
	{
		if (is_value_valid(is_value_valid(i_u) && is_value_valid(i_v) && is_adjacent(i_u, i_v)))
		{
			m_verticesArray[i_u].get_node_by_data(i_v)->m_edge->m_weight = i_val;
		}
	}

	int adjacency_list_graph::get_edge_flow(const int i_u, const int i_v) const
	{
		if (is_value_valid(is_value_valid(i_u) && is_value_valid(i_v) && is_adjacent(i_u, i_v)))
		{
			return m_verticesArray[i_u].get_node_by_data(i_v)->m_edge->m_flow;
		}
		return -1;
	}

	bool adjacency_list_graph::update_flow_in_edge(const int i_u, const int i_v, int i_flow) const
	{
		bool res = false;
		if (is_value_valid(is_value_valid(i_u) && is_value_valid(i_v) && is_adjacent(i_u, i_v)))
		{
			m_verticesArray[i_u].get_node_by_data(i_v)->m_edge->m_flow += i_flow;
			m_verticesArray[i_v].get_node_by_data(i_u)->m_edge->m_flow -= i_flow;
			res = true;
		}
		return res;
	}
	bool adjacency_list_graph::update_capacity_in_edge(const int i_u, const int i_v, const int i_flow) const
	{
		bool res = false;
		if (is_value_valid(is_value_valid(i_u) && is_value_valid(i_v) && is_adjacent(i_u, i_v)))
		{
			m_verticesArray[i_u].get_node_by_data(i_v)->m_edge->m_weight -= i_flow;
			m_verticesArray[i_u].get_node_by_data(i_v)->m_edge->m_flow += i_flow;
			res = true;
		}
		return res;
	}

	bool adjacency_list_graph::is_value_valid(const int input_value) const
	{
		bool res = false;
		if (input_value > 0 && input_value <= m_VerticesAmount)
			res = true;
		return res;
	}
}
