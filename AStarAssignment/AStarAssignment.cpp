#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include "Node.h"

// Prototypes
Graph* FindPath(Graph* graph, int first, int last);
void RetrievePath(Graph* graph, int first, int last, std::string outfileName);

struct PointHolder
{
    PointHolder(int pOne, int pTwo)
    {
        pointOne = pOne;
        pointTwo = pTwo;
    }
    int pointOne;
    int pointTwo;
};

int main(int argc, char* argv[])
{
    // USED FOR TESTING
    const bool isTesting = false;

    const std::string outfileName = "NodeCoordinates.txt";
    std::ofstream outfile;
    outfile.open(outfileName);
    outfile.close();
    
    auto* graph = new Graph(&Node::CreateNodeList(), &Connection::CreateConList(), outfileName);
    graph->PrintAll();
    
    if(!isTesting)
    {
        const std::vector<PointHolder*> pointHolder { new PointHolder(1, 29), new PointHolder(1, 38),
             new PointHolder(11, 1), new PointHolder(33, 66), new PointHolder(58, 43) };
        
        for(const auto* pHolder : pointHolder)
        {
            graph = FindPath(graph, pHolder->pointOne, pHolder->pointTwo);
            RetrievePath(graph, pHolder->pointOne, pHolder->pointTwo, outfileName);
        }
    } else
    {
        const std::vector<PointHolder*> testPoints { new PointHolder(1, 59), new PointHolder(45, 66),
                                                        new PointHolder(60, 9)};
        for(const auto* pHolder : testPoints)
        {
            graph = FindPath(graph, pHolder->pointOne, pHolder->pointTwo);
            RetrievePath(graph, pHolder->pointOne, pHolder->pointTwo, outfileName);
        }
    }
    
    return 0;
}

Graph* FindPath(Graph* graph, int first, int last)
{
    // Reset all node back to default.
    for(auto* node : graph->nodes)
    {
        if(node->GetNodeNumber() == INT_MIN)
            continue;
        
        node->SetStatus(UNVISITED);
        node->SetCostSoFar(INT_MAX);
        node->SetPrevious(0);
    }

    // Add the First Node to the hash map.
    // Due to how LowestIndex works instead of completely reworking it to return index of 0, 1,
    // to be used in a vector<int> openNode I can just keep the original form of return the node_number.
    graph->nodes[first]->SetStatus(OPEN);
    graph->nodes[first]->SetCostSoFar(0);
    std::unordered_map<int, int> openNodeMap;
    openNodeMap[first] = first;

    // Main Loop
    while(!openNodeMap.empty())
    {
        const int currentNode = Node::FindLowest(graph, openNodeMap);
        if(currentNode == last)
            break;
        const std::vector<int>* currentCon = Connection::FindConnection(graph, currentNode);
        for(const auto con : *currentCon)
        {
            int to_node = graph->connections[con]->GetToNode();
            const double to_cost = graph->nodes[currentNode]->GetCostSoFar() + graph->connections[con]->GetCost();
            if(to_cost < graph->nodes[to_node]->GetCostSoFar())
            {
                graph->nodes[to_node]->SetStatus(OPEN);
                graph->nodes[to_node]->SetCostSoFar(to_cost);
                graph->nodes[to_node]->SetHeuristic(Node::Heuristic(graph, to_node, last));
                graph->nodes[to_node]->SetTotal(static_cast<int>(graph->nodes[to_node]->GetCostSoFar() + graph->nodes[to_node]->GetHeuristic()));
                graph->nodes[to_node]->SetPrevious(currentNode);
                openNodeMap[to_node] = to_node;
            }
        }
        // Delete dynamically allocated vector.
        delete currentCon;

        // Remove from open node map.
        graph->nodes[currentNode]->SetStatus(CLOSED);
        openNodeMap.erase(currentNode);
    }
    return graph;
}

void RetrievePath(Graph* graph, int first, int last, std::string outfileName)
{
    std::vector<int> nodes;
    int current = last;
    std::ofstream outfile;
    outfile.open(outfileName, std::ios::app);
    if(!outfile)
    {
        std::cout << "Failed to load outfile." << std::endl;
        throw std::invalid_argument("WriteToFile Error: Outfile must load a file before preceding.");
    }
    
    while(current != first && graph->nodes[current]->GetStatus() != UNDEFINED)
    {
        nodes.push_back(current);
        current = graph->nodes[current]->GetPreviousIndex();
    }

    if(current == first)
    {
        nodes.push_back(first);
        outfile << "Path from " << first << " to " << last << " path= ";

        for(int i = static_cast<int>(nodes.size()) - 1; i >= 0; i--)
        {
            outfile << nodes[i] << " ";
        }
        
        outfile << "cost= " << graph->nodes[last]->GetCostSoFar() << std::endl;
    }
}