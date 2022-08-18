#pragma once
#include "adjacency_list_graph.h"

namespace finding_max_flow
{
	class max_flow_greedy_method
	{
	private:
		adjacency_list_graph* m_graph;
		adjacency_list_graph* m_residual_graph;

	public:
		//ctor d_tor
		explicit max_flow_greedy_method(const adjacency_list_graph*);
		~max_flow_greedy_method() { delete m_graph; delete m_residual_graph; }

		// algorithm 2 using greedy method , finding fattest path using dikjstra  
		bool update_residual_graph_maximum_path(const int*, int, int, int&) const;
		int* greedy_method_algorithm(const int, const int, int&, int&, int*&) const;
	
		int* find_min_cut_by_residual_graph(int, int, int&, int*&) const;
	};
}
