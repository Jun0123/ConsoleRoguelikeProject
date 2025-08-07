#include "AStarPathfinder.h"
#include <queue>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <iostream>


Node::Node(int x, int y, std::shared_ptr<Node> parent)
    : x(x), y(y), g(0), h(0), f(0),parent(parent), parent2(parent2)
{
    
}
Node::Node(int x, int y, Node* parent2,int i)
    : x(x), y(y), g(0), h(0), f(0), parent2(parent2)
{

}

AStarPathfinder::AStarPathfinder(char*& map, int sizeX, int sizeY, Vector2 startPosition, Vector2 endPosition, std::vector<char> road, std::vector < std::shared_ptr<Node>>& outPath)
    : gridSizeX(sizeX), gridSizeY(sizeY), road(road)
{
    start = std::make_shared<Node>(startPosition.x, startPosition.y);
    end = std::make_shared<Node>(endPosition.x, endPosition.y);
    grid = map;

    outPath = findPath(start, end);

}
AStarPathfinder::AStarPathfinder(char*& map, int sizeX, int sizeY, Vector2 startPosition, Vector2 endPosition, std::vector<char> road, std::vector <Vector2>& outPath)
    : gridSizeX(sizeX), gridSizeY(sizeY), road(road)
{
    start2 = new Node(startPosition.x, startPosition.y,nullptr,1);
    end2 = new Node(endPosition.x, endPosition.y, nullptr, 1);
    grid = map;

    outPath = findPath(start2, end2);

}
AStarPathfinder::~AStarPathfinder()
{
    closedList.clear();
}

int AStarPathfinder::heuristic(std::weak_ptr<Node> a, std::weak_ptr<Node> b)
{
    return std::abs(a.lock()->x - b.lock()->x) + std::abs(a.lock()->y - b.lock()->y);
}

int AStarPathfinder::heuristic2(Node* a, Node* b)
{
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

std::vector<std::shared_ptr<Node>> AStarPathfinder::getNeighbors(const std::shared_ptr<Node> node)
{
    int x = node->x;
    int y = node->y;

    //방향 
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    std::vector<std::shared_ptr<Node>> neighbors;

    for (int i = 0; i < 4; ++i)
    {
        int newX = x + dx[i];
        int newY = y + dy[i];
        //좌표가 갈 수 있는 길이고 맵의 최대/최소 길이를 벗어나지 않는다면 이웃에 추가
        if (newX >= 0 && newX < gridSizeX && newY >= 0 && newY < gridSizeY)//grid[newX][newY] == 1)
        {
            for (const char way : road)
            {
                if (grid[newY * gridSizeX + newX] == way)
                {
                    neighbors.emplace_back(std::make_shared<Node>(newX, newY));
                    break;
                }
            }
        }
    }
    return neighbors;
}

std::vector<Node*> AStarPathfinder::getNeighbors2(Node* node)
{
    int x = node->x;
    int y = node->y;

    //방향 
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    std::vector<Node*> neighbors;

    for (int i = 0; i < 4; ++i)
    {
        int newX = x + dx[i];
        int newY = y + dy[i];
        //좌표가 갈 수 있는 길이고 맵의 최대/최소 길이를 벗어나지 않는다면 이웃에 추가
        if (newX >= 0 && newX < gridSizeX && newY >= 0 && newY < gridSizeY)//grid[newX][newY] == 1)
        {
            for (const char way : road)
            {
                if (grid[newY * gridSizeX + newX] == way)
                {
                    neighbors.emplace_back(new Node(newX, newY,nullptr, 1));
                    break;
                }
            }
        }
    }
    return neighbors;
}


std::vector<Vector2> AStarPathfinder::reconstructPath(Node* endNode)
{
    std::vector<Vector2> path;
    Node* currentNode = endNode;
    while (currentNode != nullptr)
    {
        path.push_back(Vector2(currentNode->x,currentNode->y));
        currentNode = currentNode->parent2;
    }
    //시작위치->도착위치로 정렬
    std::reverse(path.begin(), path.end());
    //도착지점 좌표 제거
    path.pop_back();
    //for (auto* node : closedList)
    //{
    //    delete node;
    //}


    return path;
}

std::vector<std::shared_ptr<Node>> AStarPathfinder::reconstructPath(std::shared_ptr<Node> endNode)
{
    std::vector<std::shared_ptr<Node>> path;
    std::shared_ptr<Node> currentNode = endNode;
    while (currentNode != nullptr)
    {
        path.push_back(currentNode);
        currentNode = currentNode->parent.lock();
    }

    //시작위치->도착위치로 정렬
    std::reverse(path.begin(), path.end());

    //for (auto* node : closedList)
    //{
    //    delete node;
    //}
    

    return path;
}

std::vector<std::shared_ptr<Node>> AStarPathfinder::findPath(std::weak_ptr<Node> start, std::weak_ptr<Node> end)
{
    // 람다 f(n) 비교
    auto cmp = [](const std::shared_ptr<Node> left, const std::shared_ptr<Node> right) { return left->f > right->f; };
    // 노드의 가장 작은 f(n) 값을 가져오기 위한 우선순위큐
    // openList 방문할 노드 목록
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> openList(cmp);
    

    //어떤 노드를 발견했을 때, "이 노드가 이미 openList에 있는가?" 그리고 "만약 있다면, 기존 경로의 g값보다 지금 경로의 g값이 더 작은가?" 를 빠르게 확인해야 한다.
    //key : Node, value `g` 비용 /  중복값 방지 NodePtrHash
    std::unordered_map<std::shared_ptr<Node>, int, NodePtrHash, NodePtrEqual> openMap;

    //시작지점에서 현재거리까지
    start.lock()->g = 0;
    //현재 노드 n에서 최종 목적지 노드까지 앞으로 얼마나 걸릴지에 대한 추정치
    start.lock()->h = heuristic(start.lock(), end.lock());
    //종합 비용
    start.lock()->f = start.lock()->g + start.lock()->h;
    openList.push(start.lock());
    openMap[start.lock()] = start.lock()->g;


    //갈 수 있을 때까지 반복
    while (!openList.empty())
    {
        //f(n) 비용이 적은 값을 꺼내고 꺼낸 값 삭제
        std::shared_ptr<Node> current = openList.top();

        openList.pop();
        openMap.erase(current);

        //목표에 도착하면 경로 반환
        if (current->x == end.lock()->x && current->y == end.lock()->y) {
            //목표위치는 이웃노드방문할때 새로 생성되기 때문에 delete

            //openMap 삭제 - 필요한 경로에 포함되지 않음

            
            //목표를 찾았으므로 openList 비우기

            return reconstructPath(current);
        }

        //방문한 목록에 넣기
        closedList.insert(current);

        std::vector<std::shared_ptr<Node>> neighbors = getNeighbors(current);
        for (std::shared_ptr<Node>& neighbor : neighbors)
        {
            //set<>.count() = 내부에 있는 데이터 개수를 리턴
            //중복된 값이 있으면 노드 삭제
            if (closedList.count(neighbor))
            {
                continue;
            }

            //한칸 이동하므로 g+1
            //이동할 노드의 g값
            int newGCost = current->g + 1;

            //이웃노드가 방문할 노드목록에 없거나 이전에 알고 있던 경로보다 비용이 더 저렴하다면 정보 갱신   
            if (openMap.find(neighbor) == openMap.end() || newGCost < openMap[neighbor])
            {
                //거슬러 올라갈 부모 지정
                neighbor->parent = current;
                neighbor->g = newGCost;
                neighbor->h = heuristic(neighbor, end);
                neighbor->f = neighbor->g + neighbor->h;

                // openList에 들어가지만 pop() 할 수도 있어서, openMap에는 안들어가서 ??
                if (openMap.find(neighbor) != openMap.end())
                {
                    openMap.erase(neighbor);
                }
                //여기에 갱신한 정보
                openList.push(neighbor);
                //여기에 g변화값 갱신
                openMap[neighbor] = neighbor->g;
                
                
               
            }
            
        }
        neighbors.clear();
    }

    //경로를 찾을 수 없으므로 노드 전부 제거


    while (!openList.empty())
    {
        openList.pop();
    }
    openMap.clear();

    return {}; // No path found
}

std::vector<Vector2> AStarPathfinder::findPath(Node* start, Node* end)
{
    // 람다 f(n) 비교
    auto cmp = [](const Node* left, const Node* right) { return left->f > right->f; };
    // 노드의 가장 작은 f(n) 값을 가져오기 위한 우선순위큐
    // openList 방문할 노드 목록
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openList(cmp);


    //어떤 노드를 발견했을 때, "이 노드가 이미 openList에 있는가?" 그리고 "만약 있다면, 기존 경로의 g값보다 지금 경로의 g값이 더 작은가?" 를 빠르게 확인해야 한다.
    //key : Node, value `g` 비용 /  중복값 방지 NodePtrHash
    std::unordered_map<Node*, int, NodePointerHash, NodePointerEqual> openMap;
    std::vector<Node*> trash;
    //시작지점에서 현재거리까지
    start2->g = 0;
    //현재 노드 n에서 최종 목적지 노드까지 앞으로 얼마나 걸릴지에 대한 추정치
    start2->h = heuristic2(start2, end2);
    //종합 비용
    start2->f = start2->g + start2->h;
    openList.push(start2);
    openMap[start2] = start2->g;


    //갈 수 있을 때까지 반복
    while (!openList.empty())
    {
        //f(n) 비용이 적은 값을 꺼내고 꺼낸 값 삭제
        Node* current = openList.top();

        openList.pop();
        //openMap.erase(current);

        //목표에 도착하면 경로 반환
        if (current->x == end2->x && current->y == end2->y) {
            //목표위치는 이웃노드방문할때 새로 생성되기 때문에 delete

            //openMap 삭제 - 필요한 경로에 포함되지 않음


            //목표를 찾았으므로 openList 비우기

            std::vector<Vector2> v = reconstructPath(current);

            for (auto& d : trash)
            {
                delete d;
            }
            delete start2;
            delete end2;
            /*while (!openList.empty())
            {
                Node* node = openList.top();
                if (closedList2.count(node))
                {
                    closedList2.erase(node);
                }
                if (openMap.count(node))
                {
                    openMap.erase(node);
                }
                delete node;
                openList.pop();
            }


            for (auto& d : openMap)
            {
                if (closedList2.count(d.first))
                {
                    closedList2.erase(d.first);
                }
                delete d.first;
            }

            for (auto& d : closedList2)
            {
                delete d;
            }

            
            openMap.clear();*/

            
            return v;
        }

        //방문한 목록에 넣기
        closedList2.insert(current);

        std::vector<Node*> neighbors = getNeighbors2(current);
        
        for (Node*& neighbor : neighbors)
        {
            trash.push_back(neighbor);
            //set<>.count() = 내부에 있는 데이터 개수를 리턴
            //중복된 값이 있으면 노드 삭제
            if (closedList2.count(neighbor))
            {
                trash.pop_back();
                delete neighbor;
                continue;
            }

            //한칸 이동하므로 g+1
            //이동할 노드의 g값
            int newGCost = current->g + 1;

            //이웃노드가 방문할 노드목록에 없거나 이전에 알고 있던 경로보다 비용이 더 저렴하다면 정보 갱신   
            if (openMap.find(neighbor) == openMap.end() || newGCost < openMap[neighbor])
            {
                //거슬러 올라갈 부모 지정
                neighbor->parent2 = current;
                neighbor->g = newGCost;
                neighbor->h = heuristic2(neighbor, end2);
                neighbor->f = neighbor->g + neighbor->h;

                //여기에 갱신한 정보
                openList.push(neighbor);
                //여기에 g변화값 갱신
                openMap[neighbor] = neighbor->g;
            }
            else
            {
                trash.pop_back();
                delete neighbor;
            }
                

        }
        neighbors.clear();
    }

    //경로를 찾을 수 없으므로 노드 전부 제거
    for (auto& d : trash)
    {
        delete d;
    }
    delete start2;
    delete end2;

    return {}; // No path found
}


