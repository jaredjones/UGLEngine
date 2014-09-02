#include "ScenegraphNode.h"


ScenegraphNode::ScenegraphNode(){
	this->parent = nullptr;
}

ScenegraphNode::~ScenegraphNode(){
	clearParent();

	//iterate through all our children.
	//DenseMap does not have a function for this, so we have to break encapsulation and do it ourselves.
	std::vector<ScenegraphNode*> vec = this->children.vector;
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		ScenegraphNode* child = vec[i];
		//DenseMaps will have holes.
		if (child != nullptr)
		{
			child->clearParentNoSync(); //remove the child's pointer to us
		}
	}
}

//Unlinks the old parent (if we have one), sets the new parent and adds us to the new parent's list of children.
void ScenegraphNode::setParent(ScenegraphNode* newParent)
{
	//if we are already parented to something, unlink from it.
	if (this->parent) clearParent();

	this->indexInParentsList = newParent->addChildNoSync(this);
	this->parent = newParent;
}

//If we have a parent, remove ourselves from its list of children. Sets parent to null.
void ScenegraphNode::clearParent()
{
	if (this->parent)
	{
		this->parent->removeChildNoSync(this->indexInParentsList);
		this->parent = nullptr;
	}
}

//Sets the child node's parent to this, and puts child in the list.
void ScenegraphNode::addChild(ScenegraphNode* newChild)
{
	if (newChild->parent) newChild->clearParent();

	unsigned int indexInOurList = children.add(newChild);
	newChild->setParentNoSync(this, indexInOurList);
}

//Removes a child from our list of children and sets its parent to null
void ScenegraphNode::removeChild(ScenegraphNode* doomedChild)
{
	children.remove(doomedChild->indexInParentsList);
	doomedChild->clearParentNoSync();
}

//The No Sync functions operate only on this node, and not on the nodes that reference it.

//Sets our parent and parentChildMapIndex to what is passed into the function. The caller is responsible for adding this to the parent's list of children,
//  and cleaning up if there was a parent already present.
void ScenegraphNode::setParentNoSync(ScenegraphNode* newParent, unsigned int indexInParentsList)
{
	this->parent = newParent;
	this->indexInParentsList = indexInParentsList;
}

//Sets parent to null. The caller is responsible for removing this node from the list of children.
void ScenegraphNode::clearParentNoSync()
{
	this->parent = nullptr;
}

//Adds the argument to this node's list of children. The caller is responsible for setting the argument's parent to this, and handling the case of a preexisting parent.
//Returns the index that the child is given in the vector.
unsigned int ScenegraphNode::addChildNoSync(ScenegraphNode* newChild)
{
	return this->children.add(newChild);
}

//Removes the provided child reference from our list of children and does nothing else.
void ScenegraphNode::removeChildNoSync(unsigned int indexInOurList)
{
	this->children.remove(indexInOurList);
}
void ScenegraphNode::removeChildNoSync(ScenegraphNode* doomedChild)
{
	this->children.remove(doomedChild->indexInParentsList);
	this->relativeMtx = glm::mat4x4(1.0f);
}

//Render this node's model (if it has one), and its children.
//(This is a recursive tree traversal function)
void ScenegraphNode::render(glm::mat4x4 parentsAbsoluteMtx){
	glm::mat4x4 absoluteMtx = parentsAbsoluteMtx * this->relativeMtx;
	
	drawSelf(absoluteMtx);

	//iterate through all our children.
	//DenseMap does not have a function for this, so we have to break encapsulation and do it ourselves.
	std::vector<ScenegraphNode*> vec = this->children.vector;
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		ScenegraphNode* child = vec[i];
		//DenseMaps will have holes.
		if (child != nullptr)
		{
			child->render(absoluteMtx); //render all of our children, providing our matrix for them to base themselves off of
		}
	}
}

//unless this is overriden in a subclass, do nothing.
// a pure (non-subclassed) ScenegraphNode has no model, it's just an abstract matrix-having tree node.
void ScenegraphNode::drawSelf(glm::mat4x4 absoluteMtx){

}