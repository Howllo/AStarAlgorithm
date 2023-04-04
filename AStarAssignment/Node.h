#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum NodeStatus
{
    UNDEFINED = 0,
    UNVISITED = 1,
    OPEN      = 2,
    CLOSED     = 3
};

class Graph
{
public:
    std::vector<int> openNodes;
    std::vector<class Node*> nodes;
    std::vector<class Connection*> connections;
    int n_len;
    int c_len;
    std::unordered_map<int, Node*> hash_nodes;
    std::string outfileName;
    Graph(const std::vector<Node*>* _nodes, const std::vector<Connection*>* _con, std::string outfileName);

    /**
     * \brief Prints all the nodes first, then all of the connection.
     */
    void PrintAll() const;
};

class Node
{
    char type;
    int nodeNumber;
    NodeStatus status;
    double costSoFar;
    double heuristic;
    int total;
    int previousIndex;
    double x;
    double z;
    int numPlotPos;
    int namePlotPos;
    std::string location;
public:
    Node();
    Node(char inType, int inNodeNumber, NodeStatus inStatus, int inCostSoFar, int inHeuristic, int inTotal, int inPrevious,
        double inX, double inZ,int inNumPlotPos, int inNamePlotPos, std::string inLocation);

    // STATIC

    /**
     * \brief Used to create the primarily list for all the nodes.
     * \return Returns a vector full of node pointers.
     */
    static std::vector<Node*>& CreateNodeList();

    /**
     * \brief Used to get the status of the node from a file int.
     * \param in Take in an int to decided what the status of the node from reading the file.
     * \return Returns a NodeStatus enumerator.
     */
    static NodeStatus GetStatusFromIndex(int in);

    /**
     * \brief Used to get lowest cost int in the open node. 
     * \param graph Take in a graph object that contains all the graph data.
     * \param openNodeHash Takes in an open node unordered map.
     * \return Returns the lowest cost int in the open node.
     */
    static int FindLowest(Graph* graph, std::unordered_map<int, int> openNodeHash);

    /**
     * \brief Used to get the lowest total node out of all nodes in open node.
     * \param graph Takes in a graph object.
     * \param openNode Takes in a open node hash map.
     * \return Returns the lowest total cost node out of all the open nodes.
     */
    static int MinNode(Graph* graph, const std::unordered_map<int, int>& openNode);

    /**
     * \brief Used to get the distance between coordinates.
     * \param graph Takes in a graph object.
     * \param node_1 Takes in the current node.
     * \param node_2 Takes in the last node.
     * \return Returns a double between the distance.
     */
    static double Heuristic(Graph* graph, int node_1, int node_2);

    /**
     * \brief Used to get all the nodes that is less than target. At least that how I read R which works,
     *        and it produces results so it works.
     * \param graph Takes in a graph object.
     * \param openNode Takes in a open node hash map.
     * \param target Takes in a target and find all the indices (nodes) that is less than the target.
     * \return Returns vector<int>.
     */
    static std::vector<int>* which(Graph* graph, std::unordered_map<int, int>& openNode, int target);

    /**
     * \brief Take in all the indices from which and processes them to find the lowest index.
     * \param indexes Takes in a vector<int> array to be processed.
     * \return Returns the lowest INT index in the vector array.
     */
    static int min(const std::vector<int>* indexes);

    // GET
    char GetCharacterType() const;
    int GetNodeNumber() const;
    NodeStatus GetStatus() const;
    double GetCostSoFar() const;
    double GetHeuristic() const;
    int GetTotal() const;
    int GetPreviousIndex() const;
    double GetXCoordinate() const;
    double GetZCoordinate() const;
    int GetNumPlotPos() const;
    int GetNamePlotPos() const;
    std::string GetLocation() const;

    // SET
    void SetStatus(NodeStatus newStatus);
    void SetCostSoFar(double cost);
    void SetPrevious(int previous);
    void SetTotal(int inTotal);
    void SetHeuristic(double newHeuristic);
};

class Connection
{
    char nameType;
    int conNum;
    int fromNode;
    int toNode;
    int cost;
    int costPlot;
    int type;
public:
    Connection();
    Connection(char inNameType, int connectionNum, int fromNode, int toNode, int conCost, int coPlot, int inType);

    // STATIC

    /**
     * \brief Used to process data file Connection from text to objects.
     * \return Returns vector connection pointers of instantiated Connection objects.
     */
    static std::vector<Connection*>& CreateConList();
    
    /**
     * \brief Used to find all the connections to a node and returns them.
     * \param graph Takes in graph data object.
     * \param target Takes in a target.
     * \return Vector<int> of all connection nodes that equal target.
     */
    static std::vector<int>* FindConnection(Graph* graph, int target);
    
    
    // Getters
    char GetCharacterType() const;
    int GetConnection() const;
    int GetFromNode() const;
    int GetToNode() const;
    int GetCost() const;
    int GetCostPlot() const;
    int GetType() const;
};