#include "MatrixTreeNode.h"

//Unlinks the old parent (if we have one), sets the new parent and adds us to the new parent's list of children.
void MatrixTreeNode::setParent(MatrixTreeNode* newParent)
{
    //if we are already parented to something, unlink from it.
    if (this->parent) clearParent();
    
    this->indexInParentsList = newParent->addChildNoSync(this);
    this->parent = newParent;
}

//If we have a parent, remove ourselves from its list of children. Sets parent to null.
void MatrixTreeNode::clearParent()
{
    if (this->parent)
    {
        this->parent->removeChildNoSync(this->indexInParentsList);
        this->parent = nullptr;
    }
}

//Sets the child node's parent to this, and puts child in the list.
void MatrixTreeNode::addChild(MatrixTreeNode* newChild)
{
    if (newChild->parent) newChild->clearParent();
    
    unsigned int indexInOurList = children.add(newChild);
    newChild->setParentNoSync(this, indexInOurList);
}

//Removes a child from our list of children and sets its parent to null
void MatrixTreeNode::removeChild(MatrixTreeNode* doomedChild)
{
    children.remove(doomedChild->indexInParentsList);
    doomedChild->clearParentNoSync();
}

//The No Sync functions operate only on this node, and not on the nodes that reference it.

//Sets our parent and parentChildMapIndex to what is passed into the function. The caller is responsible for adding this to the parent's list of children,
//  and cleaning up if there was a parent already present.
void MatrixTreeNode::setParentNoSync(MatrixTreeNode* newParent, unsigned int indexInParentsList)
{
    this->parent = newParent;
    this->indexInParentsList = indexInParentsList;
}

//Sets parent to null. The caller is responsible for removing this node from the list of children.
void MatrixTreeNode::clearParentNoSync()
{
    this->parent = nullptr;
}

//Adds the argument to this node's list of children. The caller is responsible for setting the argument's parent to this, and handling the case of a preexisting parent.
//Returns the index that the child is given in the vector.
unsigned int MatrixTreeNode::addChildNoSync(MatrixTreeNode* newChild)
{
    return this->children.add(newChild);
}

//Removes the provided child reference from our list of children and does nothing else.
void MatrixTreeNode::removeChildNoSync(unsigned int indexInOurList)
{
    this->children.remove(indexInOurList);
}
void MatrixTreeNode::removeChildNoSync(MatrixTreeNode* doomedChild)
{
    this->children.remove(doomedChild->indexInParentsList);
}


//Updates our children's cached pre-multiplied matrix.
//WARNING: THIS WILL SPREAD INVALID DATA IF THIS NODE IS PARENTED TO A NODE WITH AN INVALID CACHE!
// In practice, this means you must only call settle on the Root node (parented to nothing), or a node which has a known-valid cache.
void MatrixTreeNode::settle()
{
    glm::mat4x4 root;
    
    //if we have no parent, we are a root node.
    //because a root node's internal value is the first in the multiplication chain,
    // its internal "local" value is equal to its "absolute" value.
    if (!this->parent)
        root = this->mtx;
    //if we have a parent, assume our cached premultipled matrix is valid
    else
        root = this->cached;
    
    //iterate through all our children.
    //DenseMap does not have a function for this, so we have to break encapsulation and do it ourselves.
    std::vector<MatrixTreeNode*> vec = this->children.vector;
    
    for (unsigned int i = 0; i < vec.size();i++)
    {
        MatrixTreeNode* child = vec[i];
        //DenseMaps will have holes.
        if (child != nullptr)
        {
            //first, give the child a vaild cached value.
            child->cached = root * child->mtx;
            
            //then, settle() on all of the child's children.
            //a very minor speed boost could be obtained by not doing this recursively
            
            //we can call settle on this non-root node because we know it's cached value is good - we just set it!
            child->settle();
        }
    }
}