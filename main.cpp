#include <QCoreApplication>
#include <QVector>
#include <QPair>
#include <iostream>
#include <QRandomGenerator>
#include <QMap>

class Edge
{
public:
    quint64 vertex1, vertex2;
    quint64 depth_from;
    Edge(quint64 v, quint64 n, quint64 d)
    {
        vertex1 = v;
        vertex2 = n;
        depth_from = d;
    }
    bool operator == (const Edge& e2) const
    {
        return (((this->vertex1 == e2.vertex1) && (this->vertex2 == e2.vertex2)) || ((this->vertex1 == e2.vertex2) && (this->vertex2 == e2.vertex1)));
    }
    bool operator != (const Edge& e2) const
    {
        return !(*this == e2);
    }
};

class Graph
{
public:
    quint64 depth = 0;
    quint64 graph_size = 0;
    QVector<Edge> Edges_to_delete;
    Graph(){};
    void DFS_depth(quint64 v, quint64 l);
    void addEdge(quint64, quint64);
    void addNode();
    QVector<QVector<quint64>> AdjacencyList;
    QVector<QVector<quint64>> AdjacencyListCopy;
    void deleteEdge(Edge e);
    void deleteEdges(Edge e, quint64 k);
    Edge getRandomEdge();
    bool isEmpty();
    QVector<Edge> resEdges;
    QVector<quint64> resVertices;
private:
    QVector<bool> used;
};


void Graph::addEdge(quint64 u, quint64 v)
{
    if (!AdjacencyList[u].contains(v))
    {
        AdjacencyList[u].push_back(v);
        AdjacencyList[v].push_back(u);
        AdjacencyListCopy[u].push_back(v);
        AdjacencyListCopy[v].push_back(u);
    }
}
void Graph::addNode()
{
    graph_size++;
    AdjacencyList.append(QVector<quint64>());
    AdjacencyListCopy.append(QVector<quint64>());
    used.push_back(false);
}

void Graph::DFS_depth(quint64 v, quint64 l)
{
    if (depth > l)
    {
        return;
    }
    used[v] = true;
    QVector<quint64>::iterator i;
    for (i = AdjacencyList[v].begin(); i != AdjacencyList[v].end(); ++i)
        if (!used[*i])
        {
            if (!(Edges_to_delete.contains(Edge(*i, v, depth))))
            {
              Edges_to_delete.append(Edge(v, *i, depth));
            }
            depth++;
            DFS_depth(*i, l);
            depth--;
        }
}

void Graph::deleteEdge(Edge e)
{
    AdjacencyListCopy[e.vertex1].removeAll(e.vertex2);
    AdjacencyListCopy[e.vertex2].removeAll(e.vertex1);
}
void Graph::deleteEdges(Edge e, quint64 k)
{
    Edges_to_delete.clear();
    depth = 0;
    this->DFS_depth(e.vertex1, k);
    Edges_to_delete.append(e);
    for (auto&& a : Edges_to_delete)
    {
        deleteEdge(a);
    }
    Edges_to_delete.clear();
    depth = 0;
    this->DFS_depth(e.vertex2, k);
    Edges_to_delete.append(e);
    for (auto&& a : Edges_to_delete)
    {
        deleteEdge(a);
    }
    resEdges.append(e);
}

Edge Graph::getRandomEdge()
{
    quint64 i;
    for (i = 0; i < graph_size; i++)
    {
        if(!AdjacencyListCopy[i].isEmpty())
        {
            break;
        }
    }
    return Edge(i, AdjacencyListCopy[i][0], 0);

}
bool Graph::isEmpty()
{
    for(qint64 i = 0; i < AdjacencyListCopy.size(); i++)
    {
        if (!AdjacencyListCopy[i].isEmpty())
        {
            return false;
        }
    }
    return true;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    uint n;
    std::cout << "Введите число вершин графа (граф будет заполнен случайно): ";
    std::cin >> n;
    std::cin.clear();
    while (n <= 0)
    {
        std::cout << "Введите корректное значение!" << std::endl;
        std::cout << "Введите число вершин графа: ";
        std::cin >> n;
        std::cin.clear();
    }
    int t;
    quint64 l;
    std::cout << "Введите расстояние: ";
    std::cin >> t;
    std::cin.clear();
    while (t < 0)
    {
        std::cout << "Введите корректное значение!" << std::endl;
        std::cout << "Введите расстояние: ";
        std::cin >> t;
        std::cin.clear();
    }
    l = t;
    Graph g;
    for (quint64 i = 0; i < n; i++)
    {
        g.addNode();
    }
    quint64 edge_number = QRandomGenerator::global()->bounded(1U, n * (n - 1) / 2);
    for (quint64 i = 0; i < edge_number; i++)
    {
        quint64 v1 = QRandomGenerator::global()->bounded(n);
        quint64 v2 = QRandomGenerator::global()->bounded(n);
        if (v1 != v2)
        {
            g.addEdge(v1, v2);
        }
    }

    std::cout << "Список смежности графа\n";
    quint64 c = 1;
    for (auto&& a : g.AdjacencyList)
    {
        std::cout << c << "| ";
        for (quint64 b : a)
            std::cout << b + 1 << " ";
        c++;
        std::cout << std::endl;
    }
    if (a.arguments().at(0) == "-debug")
    {
        std::cout << "Матрица смежности графа\n";
        for (quint64 i = 0; i < n; i++)
        {
            for (quint64 j = 0; j < n; j++)
            {
                if (g.AdjacencyList[i].contains(j))
                {
                    std::cout << "1, ";
                }
                else
                {
                    std::cout << "0, ";
                }
            }
            std::cout << std::endl;
        }
    }
    quint64 k = l / 2;
    k *= 2;
    k++;
    while (!g.isEmpty())
    {
        Edge re = g.getRandomEdge();
        g.deleteEdges(re, k);
    }
    if (l % 2 == 0)
    {
        for (auto&& a : g.resEdges)
        {
            if (!g.resVertices.contains(a.vertex1))
            {
                g.resVertices.append(a.vertex1);
            }
        }
    }
    else
    {
        for (auto&& a : g.resEdges)
        {
            if (!g.resVertices.contains(a.vertex1))
            {
                g.resVertices.append(a.vertex1);
            }
            if (!g.resVertices.contains(a.vertex2))
            {
                g.resVertices.append(a.vertex2);
            }
        }
    }
    std::cout << "N-МВП данного графа: ";
    for (auto&& a : g.resVertices)
    {
        std::cout << a + 1 << " ";
    }

    QCoreApplication::exit(0);
    return 0;
}
