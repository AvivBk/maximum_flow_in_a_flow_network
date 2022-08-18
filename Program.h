#pragma once
#include "max_flow_ford_fulkerson.h"
#include "max_flow_greedy_method.h"

namespace finding_max_flow
{
	class Program
	{
		adjacency_list_graph* m_graph;
		adjacency_list_graph* m_residual_graph;
	
	public:
		//Ctor and d_tor
		Program(const std::string&, const std::string&);
		~Program() { delete m_graph; delete m_residual_graph; }
	};
}
