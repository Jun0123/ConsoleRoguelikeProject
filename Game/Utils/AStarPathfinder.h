#pragma once

#include "Math/Vector2.h"

#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <map>

struct Node {
    int x, y;
    double g, h, f;
    Node * parent;

    Node(int x, int y, Node * parent = nullptr)
        : x(x), y(y), g(0), h(0), f(0), parent(parent) {}

    //정렬 기준 오름차순
    bool operator>(const Node& other) const {
        return f > other.f;
    }
};

struct NodePtrHash {
    // std::unordered_set은 C++의 해시 테이블 기반 컨테이너
    // 이미 방문한 노드(닫힌 목록, closedList)를 기록할 때
    // n의 메모리 주소 대신, n이 가리키는 x와 y 좌표를 사용해 해시 값을 만든다.
    // nodeA와 nodeB는 비록 메모리 주소는 달라도, (x, y) 좌표가 같으므로 같은 해시 값을 갖게 됨
    std::size_t operator()(const Node * n) const {
        return std::hash<int>()(n->x) ^ std::hash<int>()(n->y);
    }
};

struct NodePtrEqual {
    // 노드가 같은 좌표를 가르키는지 비교
    bool operator()(const Node * a, const Node * b) const {
        return a->x == b->x && a->y == b->y;
    }
};

 // 최적의 경로를 예측하는 기준 f(n) = g(n) + h(n)
 // h(n)의 의미: 현재 노드 n에서 최종 목적지 노드까지 앞으로 얼마나 걸릴지에 대한 추정치
 // 대각선은 이동하지 않아서 맨해튼 거리로 계산
 // 맨해튼 거리: x좌표 차이의 절댓값 + y좌표 차이의 절댓값
int heuristic(const Node * a, const Node * b) {
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

//이웃한 노드 방문
std::vector<Node*> getNeighbors(const Node* node, const std::vector<std::vector<int>>& grid) 
{
    std::vector<Node*> neighbors;
    int x = node->x;
    int y = node->y;
    
    //방향 
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };
    
    for (int i = 0; i < 4; ++i) 
    {
        int newX = x + dx[i];
        int newY = y + dy[i];
        //좌표를 방문한 적이 없고 최대/최소 길이를 벗어나지 않는다면 이웃에 추가
        if (newX >= 0 && newX < grid.size() && newY >= 0 && newY < grid[0].size() && grid[newX][newY] == 0)
        {
            neighbors.push_back(new Node(newX, newY));
        }
    }
    return neighbors;
}

//목표지점에 도착했을 때 경로를 전달하는 함수
std::vector<Node*> reconstructPath(Node * endNode) {
    std::vector<Node*> path;
    Node * currentNode = endNode;
    while (currentNode != nullptr) 
    {
         path.push_back(currentNode);
         currentNode = currentNode->parent;
    }
    //배열 역순으로 뒤집기
    std::reverse(path.begin(), path.end());
    return path;
}

// A* algorithm implementation
std::vector<Node*> astar(const std::vector<std::vector<int>>&grid, Node * start, Node * end) 
{
    // 람다 f(n) 비교
    auto cmp = [](const Node* left, const Node* right) { return left->f >right->f; };
    // 노드의 가장 작은 f(n) 값을 가져오기 위한 우선순위큐
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> openList(cmp);
    // 방문한 노드 , 중복값 방지 NodePtrHash
    std::unordered_set<Node*, NodePtrHash, NodePtrEqual> closedList;
    //어떤 노드를 발견했을 때, "이 노드가 이미 openList에 있는가?" 그리고 "만약 있다면, 기존 경로의 g값보다 지금 경로의 g값이 더 작은가?" 를 빠르게 확인해야 한다.
    //key : Node, value `g` 비용 /  map을 정렬
    std::map<Node*, int , bool(*)(const Node*, const Node*)> openMap([](const Node* a, const Node* b) { return a->x < b->x || (a->x == b->x && a->y < b->y); });
    /*
    `std::unordered_map<Node*, double, NodePtrHash, NodePtrEqual>`
       * 동작 방식: NodePtrHash를 이용해 Node의 (x, y) 좌표로 해시 값을 만들어 g값을
         저장합니다. unordered_set과 거의 동일하지만, g값(Value)을 함께 저장할 수 있다는
         장점이 있습니다.
       * 장점:
           * 가장 빠름: 평균적으로 O(1)의 시간 복잡도로 노드를 찾고, 추가하고, 삭제할 수
             있습니다. map의 O(log N)보다 이론적으로 더 빠릅니다.
           * 의도가 명확함: "정렬"이 아니라 "빠른 조회"가 목적이라는 것을 코드가 명확히
             보여줍니다.
       * 단점:
           * 커스텀 해시 함수(NodePtrHash)와 비교 함수(NodePtrEqual)를 직접 만들어 제공해야
             합니다. (이미 코드에 있긴 합니다.)
    */

    start->g = 0;
    start->h = heuristic(start, end);
    start->f = start->g + start->h;
    openList.push(start);
    openMap[start] = start->g;


    while (!openList.empty()) 
    {
        Node * current = openList.top();
        openList.pop();
        openMap.erase(current);

        if (current->x == end->x && current->y == end->y) {
             return reconstructPath(current);

        }

       closedList.insert(current);

       for (Node* neighbor : getNeighbors(current, grid)) 
       {
            if (closedList.count(neighbor)) 
            {
                delete neighbor;
                continue;
            }
            
            double tentativeG = current->g + 1;
            
            if (openMap.find(neighbor) == openMap.end() || tentativeG < openMap[neighbor]) 
            {
                neighbor->parent = current;
                neighbor->g = tentativeG;
                neighbor->h = heuristic(neighbor, end);
                neighbor->f = neighbor->g + neighbor->h;

                if (openMap.find(neighbor) == openMap.end()) 
                {
                    openList.push(neighbor);
                    openMap[neighbor] = neighbor->g;
                }
            }
            else 
            {
                delete neighbor;
            }
        }
    }
    return {}; // No path found
}