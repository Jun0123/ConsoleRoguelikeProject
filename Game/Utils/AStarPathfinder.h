#pragma once

#include "Math/Vector2.h"
#include <unordered_set>
#include <vector>
#include <memory>

struct Node {
    int x, y;
    int g, h, f;
    std::weak_ptr<Node> parent;

    Node(int x, int y, std::shared_ptr<Node> parent = nullptr);
    ~Node()
    {
    }

    //정렬 기준 오름차순
    bool operator>(const std::shared_ptr<Node> other) const {
        return f > other->f;
    }
};

struct NodePtrHash {
    // std::unordered_set은 C++의 해시 테이블 기반 컨테이너
    // 이미 방문한 노드(닫힌 목록, closedList)를 기록할 때
    // n의 메모리 주소 대신, n이 가리키는 x와 y 좌표를 사용해 해시 값을 만든다.
    // nodeA와 nodeB는 비록 메모리 주소는 달라도, (x, y) 좌표가 같으므로 같은 해시 값을 갖게 됨
    std::size_t operator()(const std::shared_ptr<Node> n) const {
        return std::hash<int>()(n->x) ^ ((std::hash<int>()(n->y)<<1));
    }
};

struct NodePtrEqual {
    // 노드가 같은 좌표를 가르키는지 비교
    bool operator()(const std::shared_ptr<Node> a, const std::shared_ptr<Node> b) const {
        return a->x == b->x && a->y == b->y;
    }
};

class AStarPathfinder
{
public:
    // 맵 포인터, 맵 사이즈, 시작위치, 끝 위치,이동 가능한 문자,구할 경로
    AStarPathfinder(char*& map, int sizeX, int sizeY, Vector2 startPosition, Vector2 endPosition, std::vector<char> road , std::vector < std::shared_ptr<Node>>& outPath);
    ~AStarPathfinder();
private:
    // 최적의 경로를 예측하는 기준 f(n) = g(n) + h(n)
    // h(n)의 의미: 현재 노드 n에서 최종 목적지 노드까지 앞으로 얼마나 걸릴지에 대한 추정치
    // 대각선은 이동하지 않아서 맨해튼 거리로 계산
    // 맨해튼 거리: x좌표 차이의 절댓값 + y좌표 차이의 절댓값
    int heuristic(std::weak_ptr<Node> a, std::weak_ptr<Node> b);

    //node에 인접한 노드를 생성
    std::vector<std::shared_ptr<Node>> getNeighbors(const std::shared_ptr<Node> node);

    //목표지점에 도착했을 때 경로를 전달하는 함수
    std::vector<std::shared_ptr<Node>> reconstructPath(std::shared_ptr<Node> endNode);

public:
    std::vector<std::shared_ptr<Node>> findPath(std::weak_ptr<Node> start, std::weak_ptr<Node> end);

private:
    // 방문한 노드 , 중복값 방지 NodePtrHash
    std::unordered_set<std::shared_ptr<Node>, NodePtrHash, NodePtrEqual> closedList;
    const char* grid = nullptr;
    int gridSizeX = 0;
    int gridSizeY = 0;
    std::shared_ptr<Node> start = nullptr;
    std::shared_ptr<Node> end = nullptr;
    std::vector<char> road;
};
