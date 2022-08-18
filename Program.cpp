#include "Program.h"

namespace finding_max_flow
{
	Program::Program(const std::string& i_input_file_name, const std::string& i_output_file_name)
	{
		char key;
		int u = -1, v = -1;
		try
		{
			std::ifstream input_file;
			bool good_input = false;
			input_file.open(i_input_file_name);
			m_graph = new adjacency_list_graph();
			m_residual_graph = new adjacency_list_graph();
			good_input = m_graph->read_adjacency_list_graph(input_file, u, v, m_residual_graph);
			if (!good_input)
			{
				std::cout << "invalid input" << std::endl;
				exit(-1);
			}
		}
		catch (std::exception ex)
		{
			std::cout << "invalid input" << std::endl;
			exit(-1);
		}

		const auto size = m_graph->length();
		const auto algorithm_1 = new max_flow_ford_fulkerson(m_graph);
		const auto algorithm_2= new max_flow_greedy_method(m_graph);
		int size_s2,max_flow_1,  max_flow_2;
		int size_s1 = size_s2 = max_flow_1 = max_flow_2 = 0;
		auto t1 = new int[size];
		auto t2 = new int[size];
		const int* s1 = algorithm_1->ford_fulkerson_algorithm(u, v, max_flow_1, size_s1,t1);
		std::cout << "BFS Method: " << std::endl;
		std::cout << "Max Flow  = " << max_flow_1 << std::endl;
	
		std::cout << "Min Cut: S =  ";
		for(int vertex = 0; vertex < size_s1; vertex++)
		{
			if(vertex +1 < size_s1)
			std::cout << s1[vertex]<<", ";
			else
				std::cout << s1[vertex] << ". ";
		}
		std::cout << "T = ";
		for (int vertex = 0; vertex < size - size_s1-1; vertex++)
		{
			if (vertex + 1 < size - size_s1 - 1)
				std::cout << t1[vertex] << ", ";
			else
				std::cout << t1[vertex];
		}
		std::cout << std::endl;


		const int* s2 = algorithm_2->greedy_method_algorithm(u, v, max_flow_2, size_s2,t2);
		std::cout << std::endl;
		std::cout << "Greedy Method: " <<std::endl;
		std::cout << "Max Flow  = " << max_flow_2 << std::endl;
		std::cout << "Min Cut: S =  ";
		for (int vertex = 0; vertex < size_s2; vertex++)
		{
			if (vertex + 1 < size_s2)
				std::cout << s2[vertex] << ", ";
			else
				std::cout << s2[vertex] << ". ";
		}
		std::cout << "T = ";
		for (int vertex = 0; vertex < size - size_s2 - 1; vertex++)
		{
			if (vertex + 1 < size - size_s1 - 1)
				std::cout << t2[vertex] << ", ";
			else
				std::cout << t2[vertex];
		}
		std::cout << std::endl;


		delete algorithm_1;
		delete algorithm_2;
		delete s1;
		delete s2;
		delete[] t1;
		delete[] t2;
	}
}
