#include "RoomNode.h"
#include <iostream>
RoomNode::RoomNode()
{
	currentRatio = 0;
}

RoomNode::RoomNode(Vector2 inPosition, int inWidth, int inHeight, float inRatio)
	: rect(inPosition, inWidth, inHeight), currentRatio(inRatio)
{

}

RoomNode::~RoomNode()
{
	DeleteNode(leftNode);
	DeleteNode(rightNode);
}

void RoomNode::DeleteNode(RoomNode*& nodeToDelete)
{
	if (nullptr == nodeToDelete)
	{
		return;
	}

	if (nullptr != nodeToDelete->leftNode)
	{
		DeleteNode(nodeToDelete->leftNode);
		nodeToDelete->leftNode = nullptr;
	}
	if (nullptr != nodeToDelete->rightNode)
	{
		DeleteNode(nodeToDelete->rightNode);
		nodeToDelete->rightNode = nullptr;
	}

	delete nodeToDelete;
	nodeToDelete = nullptr;
}


void RoomNode::SetActors()
{
}

Rect& Rect::operator=(const Rect& inRect)
{
	position = inRect.position;

	width = inRect.width;
	height = inRect.height;

	return *this;
}


