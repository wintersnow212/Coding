#include <iostream>
using namespace std;
/*
https://en.wikipedia.org/wiki/Rope_(data_structure)

A node with two children thus divides the whole string into two parts: the left subtree stores the first part of the string, the right subtree stores the second part of the string, and node's weight is the sum of the left child's weight along with all of the nodes contained in its subtree.

A valid rope must be either NULL, a leaf, or a non-leaf. More specifically:
NULL is a valid rope. It represents the empty string.
• A rope is a leaf if it is non-NULL, has a non-empty string data field, has left and right
fields that are both NULL, and has a strictly positive len equal to the length of the
string in the data field (according to the C0 string library function string_length).

• A rope is a non-leaf if it has non-NULL left and right fields, both of which are valid
ropes, and if it has a len field equal to the sum of the len field of its children. The
data field of a non-leaf is unspecificed. We'll call these non-leaves concatenation nodes.

*/


typedef struct rope_node rope;

struct rope_node 
{
    int len;
    rope* left;
    rope* right;
    string data;
};


function index(RopeNode node, integer i)
   if node.weight <= i then
       return index(node.right, i - node.weight)
   end

   if exists(node.left) then
       return index(node.left, i)
   end

   return node.string[i]
end

//
// Copyright Will Roever 2016 - 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace proj
{
  using std::string;
  
  // A rope_node represents a string as a binary tree of string fragments
  //
  // A rope_node consists of:
  //   - a non-negative integer weight
  //   - a pointer to a left child rope_node
  //   - a pointer to a right child rope_node
  //   - a string fragment
  //
  // INVARIANTS:
  //   - a leaf is represented as a rope_node with null child pointers
  //   - a leaf node's weight is equal to the length of the string fragment it contains
  //   - an internal node is represented as a rope_node with non-null children and
  //     an empty string fragment
  //   - an internal node's weight is equal to the length of the string fragment
  //     contained in (the leaf nodes of) its left subtree

  class rope_node {
    
  public:
      // handle这里就是pointer!!!
    using handle = std::unique_ptr<rope_node>;
    
    // CONSTRUCTORS
    // Construct internal node by concatenating the given nodes
    rope_node(handle l, handle r);
    // Construct leaf node from the given string
    rope_node(const string& str);
    // Copy constructor
    rope_node(const rope_node&);
    
    // ACCESSORS
    size_t getLength(void) const;
    char getCharByIndex(size_t) const;
    // Get the substring of (len) chars beginning at index (start)
    string getSubstring(size_t start, size_t len) const;
    // Get string contained in current node and its children
    string treeToString(void) const;
    
    // MUTATORS
    // Split the represented string at the specified index
    friend std::pair<handle, handle> splitAt(handle, size_t);
    
    // HELPERS
    // Functions used in balancing
    size_t getDepth(void) const;
    void getLeaves(std::vector<rope_node *>& v);
    
  private:

    // Determine whether a node is a leaf
    bool isLeaf(void) const;
    
    size_t weight_;
    handle left_;
    handle right_;
    string fragment_;
    
  }; // class rope_node


//https://github.com/wroever/rope/blob/master/src/proj/node.cpp

namespace proj
{
  using std::make_unique;
  using std::pair;
  using handle = rope_node::handle;
  
  // Define out-of-bounds error constant
  std::invalid_argument ERROR_OOB_NODE = std::invalid_argument("Error: string index out of bounds");
  
  // Construct internal node by concatenating the given nodes
  rope_node::rope_node(handle l, handle r)
    : fragment_("")
  {
    this->left_ = move(l);
    this->right_ = move(r);
    this->weight_ = this->left_->getLength();
  }

  // Construct leaf node from the given string
  rope_node::rope_node(const std::string& str)
        : weight_(str.length())
        , left_(nullptr)
        , right_(nullptr)
        , fragment_(str)
  {}
  
  // Copy constructor
  rope_node::rope_node(const rope_node& aNode)
        : weight_(aNode.weight_)
        , fragment_(aNode.fragment_)
  {
    rope_node * tmpLeft = aNode.left_.get();
    rope_node * tmpRight = aNode.right_.get();
    if(tmpLeft == nullptr) this->left_ = nullptr;
    else this->left_ = make_unique<rope_node>(*tmpLeft);
    if(tmpRight == nullptr) this->right_ = nullptr;
    else this->right_ = make_unique<rope_node>(*tmpRight);
  }
  
  // Determine whether a node is a leaf
  bool rope_node::isLeaf(void) const 
  {
    return this->left_ == nullptr && this->right_ == nullptr;
  }
  
  // Get string length by adding the weight of the root and all nodes in
  //   path to rightmost child
  size_t rope_node::getLength() const 
  {
      if(this->isLeaf())
        return this->weight_;
    
      size_t tmp = (this->right_ == nullptr) ? 0 : this->right_->getLength();
    
      return this->weight_ + tmp;
  }
  
  // Get the character at the given index
  char rope_node::getCharByIndex(size_t index) const {
    
      size_t w = this->weight_;
    
      // if node is a leaf, return the character at the specified index
    
      if (this->isLeaf()) 
      {
          if (index >= this->weight_) 
          {
                throw ERROR_OOB_NODE;
      
          } 
          else 
          {
                return this->fragment_[index];
          }
    // else search the appropriate child node
    } 
    else 
    {
        if (index < w) {
            return this->left_->getCharByIndex(index);
        } 
        else {
        return this->right_->getCharByIndex(index - w);
      }
    }
  }

  // Get the substring of (len) chars beginning at index (start)
string rope_node::getSubstring(size_t start, size_t len) const 
{
    size_t w = this->weight_;
    if (this->isLeaf())
    {
        if(len < w) 
        {
            return this->fragment_.substr(start,len);
        } 
        else 
        {
            return this->fragment_;
        }
    } 
    else 
    {
        // check if start index in left subtree
        if (start < w) 
        {
            string lResult = (this->left_ == nullptr) ? "" : this->left_->getSubstring(start,len);
            if ((start + len) > w) 
            {
                // get number of characters in left subtree
                size_t tmp =  w - start;
                string rResult = (this->right_ == nullptr) ? "" : this->right_->getSubstring(w,len-tmp);
                return lResult.append(rResult);
            } 
            else 
            {
                return lResult;
            }
        // if start index is in the right subtree...
        } 
        else 
        {
            return (this->right_ == nullptr) ? "" : this->right_->getSubstring(start-w,len);
        }
    }
}
  
  // Get string contained in current node and its children
  string rope_node::treeToString(void) const {
    if(this->isLeaf()) {
      return this->fragment_;
    }
    string lResult = (this->left_ == nullptr) ? "" : this->left_->treeToString();
    string rResult = (this->right_ == nullptr) ? "" : this->right_->treeToString();
    return lResult.append(rResult);
  }
  
  // Split the represented string at the specified index
  pair<handle, handle> splitAt(handle node, size_t index)
  {
    size_t w = node->weight_;
    // if the given node is a leaf, split the leaf
    if(node->isLeaf()) {
      return pair<handle,handle>{
        make_unique<rope_node>(node->fragment_.substr(0,index)),
        make_unique<rope_node>(node->fragment_.substr(index,w-index))
      };
    }

    // if the given node is a concat (internal) node, compare index to weight and handle
    //   accordingly
    handle oldRight = move(node->right_);
    if (index < w) {
      node->right_ = nullptr;
      node->weight_ = index;
      std::pair<handle, handle> splitLeftResult = splitAt(move(node->left_), index);
      node->left_ = move(splitLeftResult.first);
      return pair<handle,handle>{
        move(node),
        make_unique<rope_node>(move(splitLeftResult.second), move(oldRight))
      };
    } else if (w < index) {
      pair<handle, handle> splitRightResult = splitAt(move(oldRight),index-w);
      node->right_ = move(splitRightResult.first);
      return pair<handle,handle>{
        move(node),
        move(splitRightResult.second)
      };
    } else {
      return pair<handle,handle>{
        move(node->left_), move(oldRight)
      };
    }
  }

  // Get the maximum depth of the rope, where the depth of a leaf is 0 and the
  //   depth of an internal node is 1 plus the max depth of its children
  size_t rope_node::getDepth(void) const {
    if(this->isLeaf()) return 0;
    size_t lResult = (this->left_ == nullptr) ? 0 : this->left_->getDepth();
    size_t rResult = (this->right_ == nullptr) ? 0 : this->right_->getDepth();
    return std::max(++lResult,++rResult);
  }
  
  // Store all leaves in the given vector
  void rope_node::getLeaves(std::vector<rope_node *>& v) {
    if(this->isLeaf()) {
      v.push_back(this);
    } else {
      rope_node * tmpLeft = this->left_.get();
      if (tmpLeft != nullptr) tmpLeft->getLeaves(v);
      rope_node * tmpRight = this->right_.get();
      if (tmpRight != nullptr) tmpRight->getLeaves(v);
    }
  }
  
} // namespace proj




// To execute C++, please define "int main()"
int main() {
  auto words = { "Hello, ", "World!", "\n" };
  for (const string& word : words) {
    cout << word;
  }
  return 0;
}
