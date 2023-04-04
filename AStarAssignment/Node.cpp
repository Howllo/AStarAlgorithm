#include "Node.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#pragma region Graph

Graph::Graph(const std::vector<Node*>* _nodes, const std::vector<Connection*>* _con, std::string outfileName)
{
    this->outfileName = outfileName;
    
    for(auto* node : *_nodes)
    {
        nodes.push_back(node);
        hash_nodes[node->GetNodeNumber()] = node;
    }

    for(auto* con : *_con)
    {
        connections.push_back(con);
    }

    n_len = static_cast<int>(nodes.size());
    c_len = static_cast<int>(connections.size());
}

void Graph::PrintAll() const
{
    std::ofstream outfile;
    outfile.open(outfileName, std::ios::app);
    if(!outfile)
    {
        std::cout << "Failed to open outfile in PrintAll." << std::endl;
        throw std::invalid_argument("PrintAll Error: Failed to open outfile.");
    }

    outfile << "CS 330, Pathfinding" << std::endl << std::endl;

    outfile << "Nodes" << std::endl;
    for(const auto* node : nodes)
    {
        if(node->GetCharacterType() == 'D')
            continue;
        
        outfile << node->GetCharacterType() << " ";
        outfile << node->GetNodeNumber() << " ";
        outfile << node->GetStatus() << " ";
        outfile << node->GetCostSoFar() << " ";
        outfile << node->GetHeuristic() << " ";
        outfile << node->GetTotal() << " ";
        outfile << node->GetPreviousIndex() << " ";
        outfile << node->GetXCoordinate() << " ";
        outfile << node->GetZCoordinate() << " ";
        outfile << node->GetNumPlotPos() << " ";
        if(node->GetLocation().size() > 1)
        {
            outfile << node->GetNamePlotPos() << " ";
            outfile << node->GetLocation();
        } else
            outfile << node->GetNamePlotPos();
        outfile << std::endl;
    }
    
    outfile << std::endl << "Connections" << std::endl;
    for(const auto* con : connections)
    {
        if(con->GetCharacterType() == 'D')
            continue;
        
        outfile << con->GetCharacterType() << " ";
        outfile << con->GetConnection() << " ";
        outfile << con->GetFromNode() << " ";
        outfile << con->GetToNode() << " ";
        outfile << con->GetCost() << " ";
        outfile << con->GetCostPlot() << " ";
        outfile << con->GetType();
        outfile << std::endl;
    }

    outfile << std::endl;
    outfile.close();
}

#pragma endregion 

#pragma region Node

Node::Node()
{
    type = 'D';
    nodeNumber = INT_MIN;
    status = UNDEFINED;
    costSoFar = INT_MIN;
    heuristic = INT_MIN;
    total = INT_MIN;
    previousIndex = INT_MIN;
    x = INT_MIN;
    z = INT_MIN;
    numPlotPos = INT_MIN;
    namePlotPos = INT_MIN;
    location = "";
}

Node::Node(char inType, int inNodeNumber, NodeStatus inStatus, int inCostSoFar, int inHeuristic, int inTotal, int inPrevious,
            double inX, double inZ, int inNumPlotPos, int inNamePlotPos, std::string inLocation)
{
    type = inType;
    nodeNumber = inNodeNumber;
    status = inStatus;
    costSoFar = inCostSoFar;
    heuristic = inHeuristic;
    total = inTotal;
    previousIndex = inPrevious;
    x = inX;
    z = inZ;
    numPlotPos = inNumPlotPos;
    namePlotPos =inNamePlotPos; 
    location = std::move(inLocation);
}

#pragma region Get / Set

char Node::GetCharacterType() const
{
    return type;
}

int Node::GetNodeNumber() const
{
    return nodeNumber;
}

NodeStatus Node::GetStatus() const
{
    return status;
}

double Node::GetCostSoFar() const
{
    return costSoFar;
}

double Node::GetHeuristic() const
{
    return heuristic;
}

int Node::GetTotal() const
{
    return total; // 55 click to break
}

int Node::GetPreviousIndex() const
{
    return previousIndex;
}

double Node::GetXCoordinate() const
{
    return x;
}

double Node::GetZCoordinate() const
{
    return z;
}

int Node::GetNumPlotPos() const
{
    return numPlotPos;
}

int Node::GetNamePlotPos() const
{
    return namePlotPos;
}

std::string Node::GetLocation() const
{
    return location;
}

void Node::SetStatus(NodeStatus newStatus)
{
    status = newStatus;
}

void Node::SetCostSoFar(double cost)
{
    costSoFar = cost;
}

void Node::SetPrevious(int previous)
{
    previousIndex = previous;
}

void Node::SetTotal(int inTotal)
{
    total = inTotal;
}

#pragma endregion 

std::vector<Node*>& Node::CreateNodeList()
{
    std::ifstream inFile;
    inFile.open("I:\\College\\UAH\\CS - 330 - Game Dev AI\\C++\\AStarAssignment\\CS 330, Pathfinding, Graph AB Nodes v3.txt", std::ios::in);
    if(!inFile)
    {
        std::cout << "File failed to load." << std::endl;
        throw std::invalid_argument("CreateNodeList Error: inFile must load a file before preceding.");
    }
    
    std::vector<std::string> row;
    auto* nodes = new std::vector<Node*>;
    nodes->push_back(new Node());
    std::string line, word;

    while(true)
    {
        row.clear();
        std::getline(inFile, line);
        std::stringstream stream(line);

        if(line[0] == '#')
            continue;
        if( inFile.eof()) break;
        
        while(std::getline(stream, word, ','))
        {
            word.erase(remove_if(word.begin(), word.end(), isspace), word.end());
            word.erase(remove(word.begin(), word.end(), '\"'), word.end());
            row.push_back(word);
        }

        NodeStatus status = GetStatusFromIndex(std::stoi(row[2]));
        auto* new_node = new Node(row[0][0], std::stoi(row[1]), status, std::stoi(row[3]),
        std::stoi(row[4]), std::stoi(row[5]), std::stoi(row[6]), std::stod(row[7]),
                std::stod(row[8]), std::stoi(row[9]), std::stoi(row[10]), row[11]);
        
        nodes->push_back(new_node);
    }
    inFile.close();
    return *nodes;
}

NodeStatus Node::GetStatusFromIndex(int in)
{
    switch(in)
    {
        case 0:
            return UNDEFINED;
        case 1:
            return UNVISITED;
        case 2:
            return OPEN;
        case 3:
            return CLOSED;
        default:
            std::cout << "Warning default activated!" << std::endl;
        return UNDEFINED;
    }
}

int Node::FindLowest(Graph* graph, std::unordered_map<int, int> openNode)
{
    const int lowest = MinNode(graph, openNode);
    std::vector<int>* resultsIndexes = which(graph, openNode, lowest);
    const int index = min(resultsIndexes);
    delete resultsIndexes;
    return openNode[index];
}

void Node::SetHeuristic(double newHeuristic)
{
    heuristic = newHeuristic;
}

int Node::min(const std::vector<int>* indexes)
{
    int smallest = INT_MAX;
    for(const auto i : *indexes)
    {
        if(i < smallest)
            smallest = i;
    }
    return smallest;
}

std::vector<int>* Node::which(Graph* graph, std::unordered_map<int, int>& openNode, int target)
{
    auto* indexes = new std::vector<int>();
    for(auto i : openNode)
    {
        if(graph->hash_nodes[i.second]->GetTotal() == graph->hash_nodes[target]->GetTotal())
        {
            indexes->push_back(i.second);
        }
    }
    return indexes;
}

int Node::MinNode(Graph* graph, const std::unordered_map<int, int>& openNode)
{
    int lowestIndex = 0;
    Node testSmallest;
    testSmallest.SetTotal(INT_MAX);
    for(auto i : openNode)
    {
        if(graph->nodes[i.second]->GetTotal() < testSmallest.GetTotal())
        {
            lowestIndex = graph->nodes[i.second]->GetNodeNumber();
            testSmallest.SetTotal(graph->nodes[i.second]->GetTotal());
        }
    }
    return lowestIndex;
}

double Node::Heuristic(Graph* graph, int node_1, int node_2)
{
    const std::vector<Node*> nodes = graph->nodes;
    return static_cast<int>(std::sqrt(
        std::pow( nodes[node_2]->GetXCoordinate() - nodes[node_1]->GetXCoordinate(), 2 ) +
          std::pow( nodes[node_2]->z - nodes[node_1]->GetZCoordinate(), 2 )
        ));
}

#pragma endregion 

#pragma region Connection

Connection::Connection()
{
    nameType = 'D';
    conNum = INT_MIN;
    fromNode = INT_MIN;
    toNode = INT_MIN;
    cost = INT_MIN;
    costPlot = INT_MIN;
    type = INT_MIN;
}

Connection::Connection(char inNameType, int connectionNum, int fromNode, int toNode, int conCost, int coPlot, int inType)
{
    nameType = inNameType;
    conNum = connectionNum;
    this->fromNode = fromNode;
    this->toNode = toNode;
    cost = conCost;
    costPlot = coPlot;
    type = inType;
}

std::vector<int>* Connection::FindConnection(Graph* graph, int target)
{
    auto* indexes = new std::vector<int>();

    for(int i = 0; i < graph->c_len; i++)
    {
        if(graph->connections[i]->fromNode == target)
            indexes->push_back(i);
    }
    
    return indexes;
}

int Connection::GetConnection() const
{
    return conNum;
}

int Connection::GetFromNode() const
{
    return fromNode;
}

int Connection::GetToNode() const
{
    return toNode;
}

int Connection::GetCost() const
{
    return cost;
}

std::vector<Connection*>& Connection::CreateConList()
{
    std::ifstream inFile;
    inFile.open("I:\\College\\UAH\\CS - 330 - Game Dev AI\\C++\\AStarAssignment\\CS 330, Pathfinding, Graph AB Connections v3.txt", std::ios::in);
    if(!inFile)
    {
        std::cout << "File failed to load." << std::endl;
        throw std::invalid_argument("CreateConList Error: inFile must load a file before preceding.");
    }
    
    std::vector<std::string> row;
    auto* connections = new std::vector<Connection*>;
    connections->push_back(new Connection());
    std::string line, word;

    while(true)
    {
        row.clear();
        std::getline(inFile, line);
        std::stringstream stream(line);

        if(line[0] == '#')
            continue;
        if( inFile.eof()) break;
        
        while(std::getline(stream, word, ','))
        {
            word.erase(remove_if(word.begin(), word.end(), isspace), word.end());
            word.erase(remove(word.begin(), word.end(), '\"'), word.end());
            row.push_back(word);
        }
        auto* connection = new Connection(row[0][0], std::stoi(row[1]), std::stoi(row[2]),
            std::stoi(row[3]), std::stoi(row[4]), std::stoi(row[5]), std::stoi(row[6]));
        
        connections->push_back(connection);
    }
    inFile.close();
    return *connections;
}

char Connection::GetCharacterType() const
{
    return nameType;
}

int Connection::GetCostPlot() const
{
    return costPlot;
}

int Connection::GetType() const
{
    return type;
}

#pragma endregion 