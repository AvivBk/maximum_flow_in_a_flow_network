#include "max_flow_greedy_method.h"

namespace finding_max_flow
{
	max_flow_greedy_method::max_flow_greedy_method(const adjacency_list_graph* i_undirected_graph)
	{
		const auto size = i_undirected_graph->length();
		m_graph = new adjacency_list_graph(size);
		m_residual_graph = new adjacency_list_graph(size);
		for (int u = 1; u < size; u++)
		{
			auto v = i_undirected_graph->get_adjacency_list_graph(u)->get_head();
			while (v != nullptr)
			{
				m_graph->add_edge(u, v->m_edge->get_y(), v->m_edge->get_capacity());
				m_residual_graph->add_edge_no_direction(u, v->m_edge->get_y(), v->m_edge->get_capacity());
				v = v->m_next;
			}
		}
	}
	bool max_flow_greedy_method::update_residual_graph_maximum_path(const int* i_parents, const int i_source, const int i_dest, int& flow) const
	{
		bool res = false;
		if (i_parents[i_dest] > 0)
		{
			const int min_flow = m_graph->find_minimum_capacity_on_path(i_parents, i_source, i_dest);
			for (int v = i_dest; v != i_source; v = i_parents[v])
			{
				const int u = i_parents[v];
				res = m_residual_graph->update_flow_in_edge(u, v, min_flow);
				res = m_graph->update_capacity_in_edge(u, v, min_flow);
			}
			flow = min_flow;
		}
		return res;
	}

	int* max_flow_greedy_method::greedy_method_algorithm(const int i_source, const int i_dest, int& r_flow, int& r_s_size, int*& i_t) const
	{
		bool res = true;
		auto s = new int[m_graph->length()];
		auto t = new int[m_graph->length()];
		while (res)
		{
			std::vector<int> parents(m_graph->length() + 1, 0);
			parents = m_graph->fattest_path_dijkstra(i_source);
			int flow = 0;
			const auto p = new int[m_graph->length() + 1];
			for (int i = 1; i < m_graph->length(); i++)
			{
				p[i] = parents[i];
			}

			res = update_residual_graph_maximum_path(p, i_source, i_dest, flow);
			if (res)
			{
				m_graph->increase_max_flow(flow);
			}
			else
			{
				s = find_min_cut_by_residual_graph(i_source, i_dest, r_s_size, t);
			}
		}
		r_flow = m_graph->get_max_flow();
		i_t = t;
		return s;
	}

	int* max_flow_greedy_method::find_min_cut_by_residual_graph(const int i_source, const int i_dest, int& r_length, int*& i_t) const
	{
		int* parents;
		parents = new int[m_graph->length() + 1];
		const int* distance_array = m_graph->run_bfs(i_source, i_dest, &parents);
		const auto s = new int[m_graph->length()];
		auto p_size = 0, t_size = 0;
		for (int v = 1; v < m_graph->length(); v++)
		{
			if (distance_array[v] != INT_MAX)
			{
				s[p_size++] = v;
			}
			else
			{
				i_t[t_size] = v;
				t_size++;
			}
		}
		delete[] distance_array;
		r_length = p_size;
		return s;
	}
}