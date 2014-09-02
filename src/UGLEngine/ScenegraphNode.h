#pragma once 

#include "DenseMap.h"
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <vector>

/*
* A node in the tree of objects in the world.
* Objects are rendered by traversing the tree.
* Each node has a relative transform matrix, that is multiplied with
* the matrices of all its parents to get the absolute modelview matrix,
* that is then passed to whatever rendering function wants it (if any)
*/


class ScenegraphNode
{
public:
	ScenegraphNode* parent;
	//this is the key we use when removing ourselves from our parent's list of children
	unsigned int indexInParentsList;

	DenseMap<ScenegraphNode> children;

	//This node's (relative) transform matrix.
	glm::mat4x4 relativeMtx;

	ScenegraphNode();
	~ScenegraphNode();

	//Unlinks the old parent (if we have one), sets the new parent and adds us to the new parent's list of children.
	void setParent(ScenegraphNode* newParent);
	//If we have a parent, remove ourselves from its list of children. Sets parent to null.
	void clearParent();
	//Sets the child node's parent to this, and puts child in the list.
	void addChild(ScenegraphNode* newChild);
	//Removes a child from our list of children and sets its parent to null
	void removeChild(ScenegraphNode* doomedChild);

	//The No Sync functions operate only on this node, and not on the nodes that reference it.

	//Sets our parent and parentChildMapIndex to what is passed into the function. The caller is responsible for adding this to the parent's list of children,
	//  and cleaning up if there was a parent already present.
	void setParentNoSync(ScenegraphNode* newParent, unsigned int indexInParentsList);
	//Sets parent to null. The caller is responsible for removing this node from the list of children.
	void clearParentNoSync();
	//Adds the argument to this node's list of children. The caller is responsible for setting the argument's parent to this, and handling the case of a preexisting parent.
	//Returns the index that the child is given in the vector.
	unsigned int addChildNoSync(ScenegraphNode* newChild);

	//Removes the provided child reference from our list of children and does nothing else.
	void removeChildNoSync(unsigned int indexInOurList);
	void removeChildNoSync(ScenegraphNode* doomedChild);

	//Render this node's model (if it has one), and its children.
	//(This is a recursive tree traversal function)
	void render(glm::mat4x4 parentsAbsoluteMtx);

	//Draw the actual contents of this node, not it's children.
	//called by render, provided with this node's absolute matrix.
	//until overriden, does nothing.
	virtual void drawSelf(glm::mat4x4 absoluteMtx);
};