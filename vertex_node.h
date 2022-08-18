#pragma once
#include "edge_node.h"

namespace finding_max_flow
{
	class vertex_node
	{
	public:
		edge_node* m_edge;
		vertex_node* m_prev;
		vertex_node* m_next;

		explicit vertex_node(const int i_source) : m_edge(new edge_node(i_source)), m_prev(nullptr), m_next(nullptr) {}
		vertex_node(const int i_source, const int i_dest, const int i_weight) :m_edge(new edge_node(i_source, i_dest, i_weight)), m_prev(nullptr), m_next(nullptr) {}
		void set_node_ptr(vertex_node* i_prev, vertex_node* i_next) { this->m_prev = i_prev; this->m_next = i_next; }
		void set_node_values(const int i_source, const int i_dest, const int i_weight) const { m_edge->set_node_values(i_source, i_dest, i_weight); }
		~vertex_node() { delete m_edge; }
	};
}
