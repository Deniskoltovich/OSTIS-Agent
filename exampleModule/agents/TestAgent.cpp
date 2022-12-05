#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_link.hpp>
#include <sc-memory/sc_common_templ.hpp>

#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>

#include "TestAgent.hpp"

using namespace std;
using namespace utils;

namespace exampleModule
{


std::string GetStringNodeIdtf(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr node)
{
	
	ScIterator3Ptr it3 = ms_context->Iterator3(node, ScType::EdgeDCommonConst, ScType::LinkConst);
	while(it3->Next())
	{
		return CommonUtils::getLinkContent(ms_context.get(), it3->Get(2));
	}
	return CommonUtils::getLinkContent(ms_context.get(), it3->Get(2));
}

void MoveNodesToDeleted(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& NodesCheckingToBeDeleted, ScAddr& DeletedNodes){
	ScIterator3Ptr it3 = ms_context->Iterator3(NodesCheckingToBeDeleted, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(it3->Next())
	{
		ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, DeletedNodes, it3->Get(2));
		ms_context->EraseElement(it3->Get(1)); 
	}   
}

bool nodeIsNotDeleted(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& NodesCheckingToBeDeleted, ScAddr& DeletedNodes, ScAddr node){
	return !ms_context->HelperCheckEdge(DeletedNodes, node, ScType::EdgeAccessConstPosPerm) &&
				 !ms_context->HelperCheckEdge(NodesCheckingToBeDeleted, node, ScType::EdgeAccessConstPosPerm);
}

void dfs(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& deletedNodes, ScAddr& nodesCheckingToBeDeleted, ScAddr node, ScAddr& visited ){
	ScIterator3Ptr connected_nodes_1 = ms_context->Iterator3(node, ScType::EdgeDCommonConst, ScType::NodeConst);
	ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, visited, node);
	// visiting all connected nodes
	while(connected_nodes_1->Next())
	{
		if(nodeIsNotDeleted(ms_context, deletedNodes, nodesCheckingToBeDeleted, connected_nodes_1->Get(2)) &&
		   !ms_context->HelperCheckEdge(visited, connected_nodes_1->Get(2), ScType::EdgeAccessConstPosPerm))
		{
			dfs(ms_context, deletedNodes, nodesCheckingToBeDeleted, connected_nodes_1->Get(2), visited);
		}
	}
}

bool isStructConnected(std::unique_ptr<ScMemoryContext> &ms_context,ScAddr& param, ScAddr& deletedNodes, ScAddr& nodesCheckingToBeDeleted)
{	
	ScAddr visited = ms_context->CreateNode(ScType::NodeConstClass);
	ScIterator3Ptr it_avalible = ms_context->Iterator3(param, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(it_avalible->Next())
	{	
		if(nodeIsNotDeleted(ms_context, deletedNodes, nodesCheckingToBeDeleted, it_avalible->Get(2)))
		{
			dfs(ms_context, deletedNodes,nodesCheckingToBeDeleted,it_avalible->Get(2) ,visited);
			break;
		}
			
	}
	
	ScIterator3Ptr it_avalible_2 = ms_context->Iterator3(param, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(it_avalible_2->Next()){
		if(!ms_context->HelperCheckEdge(visited, it_avalible_2->Get(2), ScType::EdgeAccessConstPosPerm) &&
			nodeIsNotDeleted(ms_context, deletedNodes, nodesCheckingToBeDeleted, it_avalible_2->Get(2)))
		{
			ms_context->EraseElement(visited); 
			return false;
		}
	}
	ms_context->EraseElement(visited); 
	return true;
}

ScAddr GetNodeWithMaxPower(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& param ,ScAddr& nodesCheckingToBeDeleted, ScAddr& deletedNodes, ScAddr& available_nodes)
{
	
	ScAddr max_power_node;
	int max_power = 0;
	ScIterator3Ptr it = ms_context->Iterator3(param, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(it->Next())
	{
		if(ms_context->HelperCheckEdge(available_nodes, it->Get(2), ScType::EdgeAccessConstPosPerm) &&
			nodeIsNotDeleted(ms_context, deletedNodes, nodesCheckingToBeDeleted, it->Get(2)))
		{
			int temp_power = 0;
			ScIterator3Ptr it_2 = ms_context->Iterator3(it->Get(2), ScType::EdgeDCommonConst, ScType::NodeConst);
			while(it_2->Next()){
				if (nodeIsNotDeleted(ms_context, deletedNodes, nodesCheckingToBeDeleted, it_2->Get(2))){
						temp_power++;
					}
			}
			if (temp_power >= max_power){
				max_power = temp_power;
				max_power_node = it->Get(2);
			}
		}	
	}
	return max_power_node;
}



SC_AGENT_IMPLEMENTATION(TestAgent)
{
	//Main nodes and tmpSize initialization 
	ScLog *logger = ScLog::GetInstance();
	ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
	ScAddr param = IteratorUtils::getAnyFromSet(ms_context.get(), questionNode);
	ScAddr deletedNodes = ms_context->CreateNode(ScType::NodeConstClass);
	ScAddr nodesCheckingToBeDeleted = ms_context->CreateNode(ScType::NodeConstClass);
	//ScAddr checkedNodes = ms_context->CreateNode(ScType::NodeConstClass);
	ScAddr available_nodes = ms_context->CreateNode(ScType::NodeConstClass);
	//ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
	
	
	//add all adges to available
	ScIterator3Ptr it3 = ms_context->Iterator3(param, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(it3->Next()){
		ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, available_nodes, it3->Get(2));	
	}


	while(true){
		ScAddr max_power_node = GetNodeWithMaxPower(ms_context, param,nodesCheckingToBeDeleted, deletedNodes, available_nodes);
		// del max_power_node from available_nodes
		ScIterator3Ptr it3_del = ms_context->Iterator3(available_nodes, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
		while(it3_del->Next()){
			if (it3_del->Get(2) == max_power_node){
				ms_context->EraseElement(it3_del->Get(1)); 
			}
		}

		ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, nodesCheckingToBeDeleted, max_power_node);
		////ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, checkedNodes, max_power_node);

	  	if(isStructConnected(ms_context, param, deletedNodes, nodesCheckingToBeDeleted)){
	    	MoveNodesToDeleted(ms_context, nodesCheckingToBeDeleted, deletedNodes);

			// counting numbers of nodes and edges
			int NumOfNodes = 0;
			int NumOfEdges = 0;
			ScIterator3Ptr it3_nodes = ms_context->Iterator3(param, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
			while(it3_nodes->Next()){
				if (nodeIsNotDeleted(ms_context, deletedNodes, nodesCheckingToBeDeleted, it3_nodes->Get(2))){
					NumOfNodes++;
					ScIterator3Ptr it3_edges = ms_context->Iterator3(it3_nodes->Get(2), ScType::EdgeDCommonConst, ScType::NodeConst);
					while(it3_edges->Next()){
						if (nodeIsNotDeleted(ms_context, deletedNodes, nodesCheckingToBeDeleted, it3_edges->Get(2))){
							NumOfEdges++;
						}
					}
				}
			}
			NumOfEdges = NumOfEdges / 2;
			// condition for connected graph to be the tree
	    	if (NumOfEdges == NumOfNodes - 1){
				ScIterator3Ptr it3_ans = ms_context->Iterator3(deletedNodes, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
				std::string ans = "";
				while(it3_ans->Next()){
				  ans += GetStringNodeIdtf(ms_context, it3_ans->Get(2)) + "  ";
				}
				logger->Message(ScLog::Type::Info, "Min set of deleted nodes is {" + ans + "}");

				utils::AgentUtils::finishAgentWork(ms_context.get(), questionNode, param);
				return SC_RESULT_OK;
				break;
	    	}
	    }
	    else  //remove nodes from nodesCheckingToBeDeleted 
	  	{
	    	ScIterator3Ptr it3_1 = ms_context->Iterator3(nodesCheckingToBeDeleted, ScType::EdgeAccessConstPosPerm, ScType::NodeConst); 
	    	while(it3_1->Next()) {
	      		ms_context->EraseElement(it3_1->Get(1));  
	  		}
		}   
	}

	utils::AgentUtils::finishAgentWork(ms_context.get(), questionNode, param);
	return SC_RESULT_OK;
}

}
