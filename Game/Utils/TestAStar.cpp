#include "TestAStar.h"
#include <queue>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include <set>

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define new new
#endif


TestNode::TestNode(int x, int y, TestNode* parent2, int i)
    : x(x), y(y), g(0), h(0), f(0), parent2(parent2)
{

}

TestAStar::TestAStar(char*& map, int sizeX, int sizeY, Vector2 startPosition, Vector2 endPosition, std::vector<char> road, std::vector <Vector2>& outPath)
    : gridSizeX(sizeX), gridSizeY(sizeY), road(road)
{
    start2 = new TestNode(startPosition.x, startPosition.y, nullptr, 1);
    end2 = new TestNode(endPosition.x, endPosition.y, nullptr, 1);
    grid = map;

    outPath = findPath(start2, end2);

}
TestAStar::~TestAStar()
{
    closedList2.clear();
}

int TestAStar::heuristic2(TestNode* a, TestNode* b)
{
    return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

std::vector<TestNode*> TestAStar::getNeighbors2(TestNode* node)
{
    int x = node->x;
    int y = node->y;

    //방향 
    int dx[] = { -1, 1, 0, 0 };
    int dy[] = { 0, 0, -1, 1 };

    std::vector<TestNode*> neighbors;

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
                    neighbors.emplace_back(new TestNode(newX, newY, nullptr, 1));

                    break;
                }
            }
        }
    }
    return neighbors;
}

std::vector<Vector2> TestAStar::reconstructPath(TestNode* endNode)
{
    std::vector<Vector2> path;
    TestNode* currentNode = endNode;
    while (currentNode != nullptr)
    {
        path.push_back(Vector2(currentNode->x, currentNode->y));
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

std::vector<Vector2> TestAStar::findPath(TestNode* start, TestNode* end)
{
    // 람다 f(n) 비교
    auto cmp = [](const TestNode* left, const TestNode* right) { return left->f > right->f; };
    // 노드의 가장 작은 f(n) 값을 가져오기 위한 우선순위큐
    // openList 방문할 노드 목록
    std::priority_queue<TestNode*, std::vector<TestNode*>, decltype(cmp)> openList(cmp);


    //어떤 노드를 발견했을 때, "이 노드가 이미 openList에 있는가?" 그리고 "만약 있다면, 기존 경로의 g값보다 지금 경로의 g값이 더 작은가?" 를 빠르게 확인해야 한다.
    //key : TestNode, value `g` 비용 /  중복값 방지 NodePtrHash
    std::unordered_map<TestNode*, int, NodePointerHash2, NodePointerEqual2> openMap;
    std::set<TestNode*> trash;
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
        TestNode* current = openList.top();
        //목표에 도착하면 경로 반환
        if (current->x == end2->x && current->y == end2->y) {
            //목표위치는 이웃노드방문할때 새로 생성되기 때문에 delete

            //openMap 삭제 - 필요한 경로에 포함되지 않음


            //목표를 찾았으므로 openList 비우기

            std::vector<Vector2> v = reconstructPath(current);

            //for (auto& d : trash)
            //{
            //    delete d;
            //}
            ////delete start2;
            delete end2;
            while (!openList.empty())
            {
                TestNode* node = openList.top();
                openList.pop();
                // Todo : 메모리 누수 원인
                // 좌표가 같고 서로 다른 메모리 주소
                // 커스텀 == 문을 쓰고 있기 때문에 좌표가 같은 경우 같은 객체로 취급하여
                // 좌표값은 같고 메모리 주소가 다른 객체를 빼버렸고 그 객체는 접근할 수 없게되어 메모리 해제를 못하여 메모리 누수가 발생했다.
                /*if (closedList2.count(node))
                {
                    closedList2.erase(node);
                }*/
                if (openMap.count(node))
                {
                    openMap.erase(node);
                }
                delete node;
                
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

 
            
            openMap.clear();


            return v;
        }

        openList.pop();
        openMap.erase(current);
        
        //auto result = closedList2.insert(current);
      /*  TestNode* newNode1 = new TestNode(73, 15, nullptr, 1);
        closedList2.insert(newNode1);
        TestNode* newNode2 = new TestNode(72, 15, nullptr, 1);
        closedList2.insert(newNode2);
        TestNode* newNode3 = new TestNode(74, 15, nullptr, 1);
        closedList2.insert(newNode3);*/
      
     
   
        //해쉬 값이 같아도 최종적으로 좌표를 == 판정을 하여 false값이 나옴 다른 것으로 판단하여 추가함

  /*      TestNode* newNode = new TestNode(3,0,nullptr,1);
        TestNode* newNode2 = new TestNode(3, 0, nullptr, 1);
        closedList2.insert(newNode);
        closedList2.insert(newNode2);*/
        //커스텀 == 쓸경우
        // 해쉬 값이 같아서 실패했음을 의미 , == 판정은 관련없음, (같은 좌표일때 해쉬값이 같아서 삽입 실패, 다른좌표이고 해쉬값이 같지만 삽입 성공)이지만 둘다 false판정
        auto result = closedList2.insert(current);
        
        
        std::vector<TestNode*> neighbors = getNeighbors2(current);

        for (TestNode* neighbor : neighbors)
        {
            trash.insert(neighbor);
            //set<>.count() = 내부에 있는 데이터 개수를 리턴
            //중복된 값이 있으면 노드 삭제

            //if (closedList2.count(neighbor))
            //커스텀 == 쓸경우
            //같은 좌표일때는 closedList2에 삽입 실패 후 if (1)
            //다른 좌표일때 삽입 실패 
            if(closedList2.count(neighbor))
            {
                trash.erase(neighbor);
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
                trash.erase(neighbor);
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


