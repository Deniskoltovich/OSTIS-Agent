#include <sc-memory/sc_memory.hpp>
#include <sc-memory/sc_link.hpp>
#include <sc-memory/sc_common_templ.hpp>

#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <iostream>
#include<map>

#include "TestAgent.hpp"

using namespace std;
using namespace utils;

namespace exampleModule
{

// //Remove links from past executions
// void RemoveOldLinks(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& structure)
// {
//   //This iterator finds all graph nodes
//   ScIterator3Ptr it3 = ms_context->Iterator3(structure, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
//   while(it3->Next())
//   {
//    auto ourNode = it3->Get(2);
//    //This iterator finds links that connected to node
//    ScIterator3Ptr it3_2 = ms_context->Iterator3(ourNode, ScType::EdgeAccessConstPosPerm, ScType::LinkVar);
//    while(it3_2->Next())
//    {
//     //Remove edge between link and node 
//     ms_context->EraseElement(it3_2->Get(1)); //???
//     //Remove link
//     ms_context->EraseElement(it3_2->Get(2));
//    }   
//   }  
// }

// //Create new "clear" link to each graph node
// void CreateDistancesLinks(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& structure){
//   ScIterator3Ptr it3 = ms_context->Iterator3(structure, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
//   while(it3->Next())
//    {
//     ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, it3->Get(2), ms_context->CreateLink(ScType::LinkVar));
//    }   
// }

// //Get link that connected to known node
// ScAddr GetDistanceLink(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& node)
// {
//   ScIterator3Ptr it3 = ms_context->Iterator3(node, ScType::EdgeAccessConstPosPerm, ScType::LinkVar);
//   while(it3->Next())
//    {
//     return it3->Get(2);
//    }   
//    return {};
// }





// //Get link information, ???(cast)
// uint32_t GetLinkInfo(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr node)
// {
//   uint32_t res;
//   string strRes;
//   ScIterator3Ptr it3 = ms_context->Iterator3(node, ScType::EdgeAccessConstPosPerm, ScType::LinkVar);
//   while(it3->Next())
//    {
//     strRes = CommonUtils::getLinkContent(ms_context.get(), it3->Get(2));
//    }  
//    res = stoi(strRes);
//    return res;
// }

// //Checking distances node to find link with max value
// ScAddr GetMaxCycleSize(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr distances)
// {
//   ScAddr res = ms_context->CreateNode(ScType::LinkVar);
//   uint32_t maxSize = 0;
//   uint32_t tmp;
//   //This iterator finds all links that connected to distances node
//   ScIterator3Ptr it3 = ms_context->Iterator3(distances, ScType::EdgeAccessConstPosPerm, ScType::LinkVar);
//   //Common way to find max value
//   while(it3->Next())
//   {
//    auto strTmp = CommonUtils::getLinkContent(ms_context.get(), it3->Get(2));
//    tmp = stoi(strTmp); 
//    if (tmp > maxSize)
//    {
//     ms_context->SetLinkContent(res, to_string(tmp));
//     maxSize = tmp;
//    }
//   } 
//   return res; 
// }

// //Graph detour using DFS
// void DFS(ScLog *logger, std::unique_ptr<ScMemoryContext> &ms_context, ScAddr node, 
//          ScAddr& visited, ScAddr& globalVisited, uint32_t& tmpSize, ScAddr& distances)
// {
//   //Put tmpSize value to distanceLink
//   ScAddr distanceLink = GetDistanceLink(ms_context, node);
//   ms_context->SetLinkContent(distanceLink, to_string(tmpSize));
	
//   //Create edge between current node and visited node
//   ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, visited, node);

//   tmpSize++;

//   //This iterator finds nodes that are adjacent to starting node
//   ScIterator3Ptr it3 = ms_context->Iterator3(node, ScType::EdgeDCommonConst, ScType::NodeConst);
//   while(it3->Next())
//   {
//     //if adjacent node is visited and isn't glovalVisited 
//     if(ms_context->HelperCheckEdge(visited, it3->Get(2), ScType::EdgeAccessConstPosPerm) && 
//       !ms_context->HelperCheckEdge(globalVisited, it3->Get(2), ScType::EdgeAccessConstPosPerm))
//     {
//       //Put cycle length to link and connect this link to distances node
//       ScAddr cycleSize = ms_context->CreateNode(ScType::LinkVar);
//       uint32_t value = GetLinkInfo(ms_context, node) - GetLinkInfo(ms_context, it3->Get(2)) + (uint32_t)1;
//       ms_context->SetLinkContent(cycleSize, to_string(value));
//       ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, distances, cycleSize);
//     } 
//     //if adjacent node isn't visited
//     else if(!ms_context->HelperCheckEdge(visited, it3->Get(2), ScType::EdgeAccessConstPosPerm))
//     {
//       //Move to the next node 
//       DFS(logger, ms_context, it3->Get(2), visited, globalVisited, tmpSize, distances);
//     }
//   }
//   //Remove edge between visited and current node
//   //This iterator finds edge between visited and node 
//   ScIterator3Ptr it3_2 = ms_context->Iterator3(visited, ScType::EdgeAccessConstPosPerm, node);
//   while(it3_2->Next())
//   {
//    //Remove edge
//    ms_context->EraseElement(it3_2->Get(1)); 
//   } 
//   //Create edge between current node and globalVisited node
//   if(!ms_context->HelperCheckEdge(globalVisited, node, ScType::EdgeAccessConstPosPerm))//if this edge isn't exist 
//   {
//    //Create edge between current node and globalVisited node 
//    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, globalVisited, node);
//   }
//   tmpSize--;
//  }


void GetNodeIdtf(std::unique_ptr<ScMemoryContext> &ms_context, ScLog* logger, ScAddr  node)
{
	
	ScIterator3Ptr it3 = ms_context->Iterator3(node, ScType::EdgeDCommonConst, ScType::LinkConst);
	while(it3->Next())
	 {
		logger->Message(ScLog::Type::Info, "Idtf" + CommonUtils::getLinkContent(ms_context.get(), it3->Get(2)));
	 }  
}

std::string GetNodeIdtfForAnswer(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr node)
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
	 }   
}

bool nodeIsNotDeleted(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& NodesCheckingToBeDeleted, ScAddr& DeletedNodes, ScAddr node){
	return !ms_context->HelperCheckEdge(DeletedNodes, node, ScType::EdgeAccessConstPosPerm) &&
				 !ms_context->HelperCheckEdge(NodesCheckingToBeDeleted, node, ScType::EdgeAccessConstPosPerm);
}

std::map<std::string, int> GetNodesPower(std::unique_ptr<ScMemoryContext> &ms_context,
 																						 ScAddr& structure,
																						 ScAddr& nodesCheckingToBeDeleted,
																						 ScAddr& deletedNodes)
{
	std::map<std::string, int> power_of_nodes;

	ScIterator3Ptr it3_temp = ms_context->Iterator3(structure, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	// fills map where the keys are nodes_idtf and values are their powers
	while(it3_temp->Next()){
		if(nodeIsNotDeleted(ms_context, nodesCheckingToBeDeleted, deletedNodes, it3_temp->Get(2))){
			auto temp_node = it3_temp->Get(2);
			std::string node_idtf = GetNodeIdtfForAnswer(ms_context, temp_node);
			power_of_nodes[node_idtf] = 0;
		}
	}

	ScIterator3Ptr it3 = ms_context->Iterator3(structure, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(it3->Next())
	{ 
		if(nodeIsNotDeleted(ms_context, nodesCheckingToBeDeleted, deletedNodes, it3->Get(2))){
			auto temp_node = it3->Get(2);
			std::string node_idtf = GetNodeIdtfForAnswer(ms_context, temp_node);
			ScIterator3Ptr it3_1 = ms_context->Iterator3(temp_node, ScType::EdgeUCommonConst, ScType::NodeConst);
			while(it3_1->Next())
			{
				if(nodeIsNotDeleted(ms_context, nodesCheckingToBeDeleted, deletedNodes, it3_1->Get(2))){
					auto adjacent_node = it3_1->Get(2);
					std::string adjacent_node_idtf = GetNodeIdtfForAnswer(ms_context, adjacent_node);
					power_of_nodes[node_idtf] = power_of_nodes[node_idtf] + 1;
					power_of_nodes[adjacent_node_idtf] = power_of_nodes[adjacent_node_idtf] + 1;
				}
			}
		}     
	}
  return power_of_nodes;
}


bool isStructConnected(std::unique_ptr<ScMemoryContext> &ms_context,
											ScAddr& param,
											ScAddr& deletedNodes,
											ScAddr& nodesCheckingToBeDeleted, ScLog* logger)
{
	logger->Message(ScLog::Type::Info, "in fuction");
	ScIterator3Ptr it3 = ms_context->Iterator3(param, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(it3->Next()){
		logger->Message(ScLog::Type::Info, "entering node");
		if(nodeIsNotDeleted(ms_context, nodesCheckingToBeDeleted, deletedNodes, it3->Get(2)))
			{
				logger->Message(ScLog::Type::Info, "entering if state");

				ScIterator3Ptr it3_node = ms_context->Iterator3(it3->Get(2), ScType::EdgeUCommonConst, ScType::NodeConst);
				if(!it3_node->Next())
				{
					logger->Message(ScLog::Type::Info, "return false");
					return false;
				}

			}
	}
	logger->Message(ScLog::Type::Info, "return true");
	return true;
}

ScAddr GetNodeWithMaxPower(std::unique_ptr<ScMemoryContext> &ms_context, ScAddr& structure, ScAddr& nodesCheckingToBeDeleted, ScAddr& deletedNodes)
{
	
	std::map<std::string, int> power_of_nodes = GetNodesPower(ms_context, structure, nodesCheckingToBeDeleted, deletedNodes);
	std::pair<std::string, int> maxValuePair;
	int maxValue = 0;
	for (const auto &entry: power_of_nodes)
	{
		if (maxValue < entry.second)
		{
			maxValue = entry.second;
			maxValuePair = entry;
		}
	}
	ScIterator3Ptr return_it = ms_context->Iterator3(structure, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	while(return_it->Next()){
		if(maxValuePair.first == GetNodeIdtfForAnswer(ms_context, return_it->Get(2))){
			return return_it->Get(2);
		}
	} 
}




SC_AGENT_IMPLEMENTATION(TestAgent)
{
	//Main nodes and tmpSize initialization 
	ScLog *logger = ScLog::GetInstance();
	ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
	ScAddr param = IteratorUtils::getAnyFromSet(ms_context.get(), questionNode);
	ScAddr deletedNodes = ms_context->CreateNode(ScType::NodeConstClass);
	ScAddr nodesCheckingToBeDeleted = ms_context->CreateNode(ScType::NodeConstClass);
	ScAddr checkedNodes = ms_context->CreateNode(ScType::NodeConstClass);




	//ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
	logger->Message(ScLog::Type::Info, "There are no cycles in the graph");
	
	ScIterator3Ptr it3 = ms_context->Iterator3(param, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

	//This iterator finds all nodes of graph in the structure
	//logger->Message(ScLog::Type::Info, "node with max power is " + CommonUtils::getLinkContent(ms_context.get(), GetNodeWithMaxPower(ms_context, param)));
	// int NumOfNOdes = 0;
	// ScAddr node_with_max_power; // TODO: Find node with max power
	// while(it3->Next()){
	//   auto temp_node = it3->Get(2);
	//   GetPowerOfNode(ms_context, temp_node, logger);
	//   GetNodeIdtf(ms_context, logger, it3->Get(2));
	//   logger->Message(ScLog::Type::Info, "NODE");
	// }


	ScAddr max_power_node = GetNodeWithMaxPower(ms_context, param, nodesCheckingToBeDeleted, deletedNodes);
	logger->Message(ScLog::Type::Info, GetNodeIdtfForAnswer(ms_context, max_power_node));
	// while(true){
	//   // ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, nodesCheckingToBeDeleted, node_with_max_power);

	//   // TODO: check if graph is connected
	//   if(isStructConnected(ms_context, param, deletedNodes, nodesCheckingToBeDeleted, logger)){
	//     MoveNodesToDeleted(ms_context, nodesCheckingToBeDeleted, deletedNodes);
	//     if (NumOfEdges() == NumOfNOdes() - 1){
	//       // ScIterator3Ptr it3_ans = ms_context->Iterator3(deletedNodes, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	//       // std::string ans = "";
	//       // while(it3_ans->Next()){
	//       //   ans += GetNodeIdtfForAnswer(ms_context, logger, it3_ans->Get(2)) + "  ";
	//       // }
	//       // logger->Message(ScLog::Type::Info, "Min set of deleted nodes is {" + ans + "}");
	//     }
	//   }
	//   else  //remove nodes from nodesCheckingToBeDeleted 
	//   {
	//     ScIterator3Ptr it3_1 = ms_context->Iterator3(nodesCheckingToBeDeleted, ScType::EdgeAccessConstPosPerm, ScType::NodeConst); 
		
	//     while(it3_1->Next()) 
	//       ms_context->EraseElement(it3_2->Get(1));  
	//   }  
	// }
	// MoveNodesToDeleted(ms_context, param, deletedNodes);
	// ScIterator3Ptr it3_ans = ms_context->Iterator3(deletedNodes, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	// std::string ans = "";
	// while(it3_ans->Next()){
	//   ans += GetNodeIdtfForAnswer(ms_context, logger, it3_ans->Get(2)) + "  ";
	// }
	// logger->Message(ScLog::Type::Info, "Min set of deleted nodes is {" + ans + "}");


	// ScIterator3Ptr it3 = ms_context->Iterator3(structure, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
	// while(it3->Next())
	// {
	//  if(!ms_context->HelperCheckEdge(globalVisited, it3->Get(2), ScType::EdgeAccessConstPosPerm)) // if node isn't globalVisited
	//  {
	//   RemoveOldLinks(ms_context, structure); //Clear past links
	//   CreateDistancesLinks(ms_context, structure); //Create new "clear" links
	//   DFS(logger, ms_context, it3->Get(2), visited, globalVisited, tmpSize, distances); //Call DFS to current node
	//  }
	// }   
	// //Check if DFS found cycles
	// bool hasCycles = false;
	// //This iterator finds links that are connected with distances link
	// ScIterator3Ptr it3_2 = ms_context->Iterator3(distances, ScType::EdgeAccessConstPosPerm, ScType::LinkVar);
	// while(it3_2->Next())
	//  {
	//   hasCycles = true;
	//   break;
	//  }  
	// //If Cycles exist
	// if(hasCycles)
	// {
	//   ScAddr result = GetMaxCycleSize(ms_context, distances); //Create link that contains the greatest cycle size
	//   ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, result); //Connect result with answer 
	//   logger->Message(ScLog::Type::Info, "Graph circumference:  "+ CommonUtils::getLinkContent(ms_context.get(), result)); //Send log to console
	// }
	// else
	//   logger->Message(ScLog::Type::Info, "There are no cycles in the graph");

	utils::AgentUtils::finishAgentWork(ms_context.get(), questionNode, param);
	return SC_RESULT_OK;
}

}
