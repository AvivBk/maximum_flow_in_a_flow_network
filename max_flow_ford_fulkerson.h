#pragma once
#include "adjacency_list_graph.h"

namespace finding_max_flow
{
	class max_flow_ford_fulkerson
	{
	private:
		adjacency_list_graph* m_graph;
		adjacency_list_graph* m_residual_graph;

	public:
		// ctor and d_tor
		explicit max_flow_ford_fulkerson(const adjacency_list_graph*);
		~max_flow_ford_fulkerson() { delete m_graph; delete m_residual_graph; }

		// algorithm 1 using ford fulkerson , finding the shortest path using bfs
		int* ford_fulkerson_algorithm(const int, const int, int&, int&, int*&) const;
		bool update_residual_graph(const int*, int, int, int&) const;
		int* find_min_cut_by_residual_graph(int,int,int&, int*&) const;
	};
}
