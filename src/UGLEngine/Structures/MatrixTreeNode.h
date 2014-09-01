#pragma once

#include <glm.hpp>
#include <vector>
#include "DenseMap.h"


class MatrixTreeNode
{
public:
    MatrixTreeNode* parent;
    //this is the key we use when removing ourselves from our parent's list of children
    unsigned int indexInParentsList;
    
    DenseMap<MatrixTreeNode> children;
    
    //This node's transform matrix.
    glm::mat4x4 mtx;
    
    //A cached matrix that is supposed to be equal to all of the matricies from root to this multiplied (in that order).
    //It becomes valid when a settle function is called on one of this node's parents,
    //  and ceases validity when this or a node up the parent chain has it's mtx changed.
    glm::mat4x4 cached;
    
    //Unlinks the old parent (if we have one), sets the new parent and adds us to the new parent's list of children.
    void setParent(MatrixTreeNode* newParent);
    //If we have a parent, remove ourselves from its list of children. Sets parent to null.
    void clearParent();
    //Sets the child node's parent to this, and puts child in the list.
    void addChild(MatrixTreeNode* newChild);
    //Removes a child from our list of children and sets its parent to null
    void removeChild(MatrixTreeNode* doomedChild);
    
    //The No Sync functions operate only on this node, and not on the nodes that reference it.
    
    //Sets our parent and parentChildMapIndex to what is passed into the function. The caller is responsible for adding this to the parent's list of children,
    //  and cleaning up if there was a parent already present.
    void setParentNoSync(MatrixTreeNode* newParent, unsigned int indexInParentsList);
    //Sets parent to null. The caller is responsible for removing this node from the list of children.
    void clearParentNoSync();
    //Adds the argument to this node's list of children. The caller is responsible for setting the argument's parent to this, and handling the case of a preexisting parent.
    //Returns the index that the child is given in the vector.
    unsigned int addChildNoSync(MatrixTreeNode* newChild);
    
    //Removes the provided child reference from our list of children and does nothing else.
    void removeChildNoSync(unsigned int indexInOurList);
    void removeChildNoSync(MatrixTreeNode* doomedChild);
    
    
    //Updates our children's cached pre-multiplied matrix.
    //WARNING: THIS WILL SPREAD INVALID DATA IF THIS NODE IS PARENTED TO A NODE WITH AN INVALID CACHE!
    // In practice, this means you must only call settle on the Root node (parented to nothing), or a node which has a known-valid cache.
    void settle();
};