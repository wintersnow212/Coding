#include <iostream>
using namespace std;
/*
一个Rope 数据结构用来高效的处理字符串的拼接、查询、删除、及随机访问。
Rope的一个典型应用场景是：在一个文本编辑程序里，用来保存较长的文本字符串。如下图，字符串“Hello_my_name_is_Simon”的表示 
One of the most common operations on strings is appending or concatenation. 
Appending to the end of a string when the string is stored in the traditional manner 
(i.e. an array of characters) would take a minimum of O(n) time (where n is the length of the original string).

We can reduce time taken by append using Ropes Data Structure.

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



Index

Figure 2.1: Example of index lookup on a rope.
Definition: Index(i): return the character at position i
Time complexity: {\displaystyle O(\log N)} O(\log N)
To retrieve the i-th character, we begin a recursive search from the root node:

function index(RopeNode node, integer i)
    if node.weight < i and exists(node.right) then
        return index(node.right, i - node.weight)
    end
    
    if exists(node.left) then
        return index(node.left, i)
    end
    
    return node.[i]string
end
For example, to find the character at i=10 in Figure 2.1 shown on the right, start at the root node (A), find that 22 is greater than 10 and there is a left child, so go to the left child (B). 9 is less than 10, so subtract 9 from 10 (leaving i=1) and go to the right child (D). Then because 6 is greater than 1 and there's a left child, go to the left child (G). 2 is greater than 1 and there's a left child, so go to the left child again (J). Finally 2 is greater than 1 but there is no left child, so the character at index 1 of the short string "na", is the answer.

Concat

Figure 2.2: Concatenating two child ropes into a single rope.
Definition: Concat(S1, S2): concatenate two ropes, S1 and S2, into a single rope.
Time complexity: {\displaystyle O(1)} O(1) (or {\displaystyle O(\log N)} O(\log N) time to compute the root weight)
A concatenation can be performed simply by creating a new root node with left = S1 and right = S2, which is constant time. The weight of the parent node is set to the length of the left child S1, which would take {\displaystyle O(\log N)} O(\log N) time, if the tree is balanced.

As most rope operations require balanced trees, the tree may need to be re-balanced after concatenation.

Split

Figure 2.3: Splitting a rope in half.
Definition: Split (i, S): split the string S into two new strings S1 and S2, S1 = C1, …, Ci and S2 = Ci + 1, …, Cm.
Time complexity: {\displaystyle O(\log N)} O(\log N)
There are two cases that must be dealt with:

The split point is at the end of a string (i.e. after the last character of a leaf node)
The split point is in the middle of a string.
The second case reduces to the first by splitting the string at the split point to create two new leaf nodes, then creating a new node that is the parent of the two component strings.

For example, to split the 22-character rope pictured in Figure 2.3 into two equal component ropes of length 11, query the 12th character to locate the node K at the bottom level. Remove the link between K and G. Go to the parent of G and subtract the weight of K from the weight of D. Travel up the tree and remove any right links to subtrees covering characters past position 11, subtracting the weight of K from their parent nodes (only node D and A, in this case). Finally, build up the newly orphaned nodes K and H by concatenating them together and creating a new parent P with weight equal to the length of the left node K.

As most rope operations require balanced trees, the tree may need to be re-balanced after splitting.

Insert
Definition: Insert(i, S’): insert the string S’ beginning at position i in the string s, to form a new string C1, …, Ci, S', Ci + 1, …, Cm.
Time complexity: {\displaystyle O(\log N)} O(\log N).
This operation can be done by a Split() and two Concat() operations. The cost is the sum of the three.

Delete
Definition: Delete(i, j): delete the substring Ci, …, Ci + j − 1, from s to form a new string C1, …, Ci − 1, Ci + j, …, Cm.
Time complexity: {\displaystyle O(\log N)} O(\log N).
This operation can be done by two Split() and one Concat() operation. First, split the rope in three, divided by i-th and i+j-th character respectively, which extracts the string to delete in a separate node. Then concatenate the other two nodes.

Report
Definition: Report(i, j): output the string Ci, …, Ci + j − 1.
Time complexity: {\displaystyle O(j+\log N)} {\displaystyle O(j+\log N)}
To report the string Ci, …, Ci + j − 1, find the node u that contains Ci and weight(u) >= j, and then traverse T starting at node u. Output Ci, …, Ci + j − 1 by doing an in-order traversal of T starting at node u.



*/
//https://github.com/wroever/rope/tree/master/src/proj


/*

Insert
Definition: Insert(i, S’): insert the string S’ beginning at position i in the string s, to form a new string C1, …, Ci, S', Ci + 1, …, Cm.
Time complexity: {\displaystyle O(\log N)} O(\log N).
This operation can be done by a Split() and two Concat() operations. The cost is the sum of the three.

Delete
Definition: Delete(i, j): delete the substring Ci, …, Ci + j − 1, from s to form a new string C1, …, Ci − 1, Ci + j, …, Cm.
Time complexity: {\displaystyle O(\log N)} O(\log N).
This operation can be done by two Split() and one Concat() operation. First, split the rope in three, divided by i-th and i+j-th character respectively, which extracts the string to delete in a separate node. Then concatenate the other two nodes.

Report
Definition: Report(i, j): output the string Ci, …, Ci + j − 1.
Time complexity: {\displaystyle O(j+\log N)} {\displaystyle O(j+\log N)}
To report the string Ci, …, Ci + j − 1, find the node u that contains Ci and weight(u) >= j, and then traverse T starting at node u. Output Ci, …, Ci + j − 1 by doing an in-order traversal of T starting at node u.

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


namespace proj
{
  using std::make_unique;
  using std::pair;
  
  // out-of-bounds error constant
  std::invalid_argument ERROR_OOB_ROPE = std::invalid_argument("Error: string index out of bounds");

  // Default constructor - produces a rope representing the empty string
  rope::rope(void) : rope("")
  {}
  
  // Construct a rope from the given string
  rope::rope(const string& str) {
    this->root_ = make_unique<rope_node>(str);
  }
  
  // Copy constructor
  rope::rope(const rope& r) {
    rope_node newRoot = rope_node(*r.root_);
    this->root_ = make_unique<rope_node>(newRoot);
  }
  
  // Get the string stored in the rope
  string rope::toString(void) const {
    if(this->root_ == nullptr)
      return "";
    return this->root_->treeToString();
  }
  
  // Get the length of the stored string
  size_t rope::length(void) const {
    if(this->root_ == nullptr)
      return 0;
    return this->root_->getLength();
  }
  
  // Get the character at the given position in the represented string
  char rope::at(size_t index) const {
    if(this->root_ == nullptr)
      throw ERROR_OOB_ROPE;
    return this->root_->getCharByIndex(index);
  }

  // Return the substring of length (len) beginning at the specified index
  string rope::substring(size_t start, size_t len) const {
    size_t actualLength = this->length();
    if (start > actualLength || (start+len) > actualLength) throw ERROR_OOB_ROPE;
    return this->root_->getSubstring(start, len);
  }

  // Insert the given string into the rope, beginning at the specified index (i)
  void rope::insert(size_t i, const string& str) {
    this->insert(i,rope(str));
  }

  // Insert the given rope into the rope, beginning at the specified index (i)
  void rope::insert(size_t i, const rope& r) {
    if (this->length() < i) {
      throw ERROR_OOB_ROPE;
    } else {
      rope tmp = rope(r);
      pair<handle, handle> origRopeSplit = splitAt(move(this->root_),i);
      handle tmpConcat = make_unique<rope_node>(move(origRopeSplit.first), move(tmp.root_));
      this->root_ = make_unique<rope_node>(move(tmpConcat), move(origRopeSplit.second));
    }
  }
  
  // Append the argument to the existing rope
  void rope::append(const string& str) {
    rope tmp = rope(str);
    this->root_ = make_unique<rope_node>(move(this->root_), move(tmp.root_));
  }

  // Append the argument to the existing rope
  void rope::append(const rope& r) {
    rope tmp = rope(r);
    this->root_ = make_unique<rope_node>(move(this->root_), move(tmp.root_));
  }
  
  // Delete the substring of (len) characters beginning at index (start)
  void rope::rdelete(size_t start, size_t len) {
    size_t actualLength = this->length();
    if (start > actualLength || start+len > actualLength) {
      throw ERROR_OOB_ROPE;
    } else {
      pair<handle, handle> firstSplit = splitAt(move(this->root_),start);
      pair<handle, handle> secondSplit = splitAt(move(firstSplit.second),len);
      secondSplit.first.reset();
      this->root_ = make_unique<rope_node>(move(firstSplit.first), move(secondSplit.second));
    }
  }
  
  // Determine if rope is balanced
  //
  // A rope is balanced if and only if its length is greater than or equal to
  //   fib(d+2) where d refers to the depth of the rope and fib(n) represents
  //   the nth fibonacci number i.e. in the set {1,1,2,3,5,8,etc...}
  bool rope::isBalanced(void) const{
    if(this->root_ == nullptr)
      return true;
    size_t d = this->root_->getDepth();
    return this->length() >= fib(d+2);
  }
  
  // Balance a rope
  void rope::balance(void) {
    // initiate rebalancing only if rope is unbalanced
    if(!this->isBalanced()) {
      // build vector representation of Fibonacci intervals
      std::vector<size_t> intervals = buildFibList(this->length());
      std::vector<handle> nodes(intervals.size());
      
      // get leaf nodes
      std::vector<rope_node *> leaves;
      this->root_->getLeaves(leaves);
      
      size_t i;
      size_t max_i = intervals.size()-1;
      size_t currMaxInterval = 0;
      handle acc = nullptr;
      handle tmp = nullptr;

      // attempt to insert each leaf into nodes vector based on length
      for (auto& leaf : leaves) {
        size_t len = leaf->getLength();
        bool inserted = false;
        
        // ignore empty leaf nodes
        if (len > 0) {
          acc = make_unique<rope_node>(*leaf);
          i = 0;

          while(!inserted) {
            // find appropriate slot for the acc node to be inserted,
            // concatenating with nodes encountered along the way
            while(i < max_i && len >= intervals[i+1]) {
              if(nodes[i].get() != nullptr) {
                // concatenate encountered entries with node to be inserted
                tmp = make_unique<rope_node>(*nodes[i].get());
                acc = make_unique<rope_node>(*acc.get());
                acc = make_unique<rope_node>(move(tmp),move(acc));
                
                // update len
                len = acc->getLength();
                
                // if new length is sufficiently great that the node must be
                //   moved to a new slot, we clear out the existing entry
                if(len >= intervals[i+1]) nodes[i] = nullptr;
              }
              i++;
            }
            
            // target slot found -- check if occupied
            if (nodes[i].get() == nullptr) {
              nodes[i].swap(acc);
              inserted = true;
              // update currMaxInterval if necessary
              if(i > currMaxInterval) currMaxInterval = i;
            } else {
              // concatenate encountered entries with node to be inserted
              tmp = make_unique<rope_node>(*nodes[i].get());
              acc = make_unique<rope_node>(*acc.get());
              acc = make_unique<rope_node>(move(tmp),move(acc));
              
              // update len
              len = acc->getLength();
              
              // if new length is sufficiently great that the node must be
              //   moved to a new slot, we clear out the existing entry
              if(len >= intervals[i+1]) nodes[i] = nullptr;
            }
          }
        }
      }
      
      // concatenate remaining entries to produce balanced rope
      acc = move(nodes[currMaxInterval]);
      for(int idx = currMaxInterval; idx >= 0; idx--) {
        if(nodes[idx] != nullptr) {
          tmp = make_unique<rope_node>(*nodes[idx].get());
          acc = make_unique<rope_node>(move(acc),move(tmp));
        }
      }
      this->root_ = move(acc); // reset root
      
    }
  }
  
  // Assignment operator
  rope& rope::operator=(const rope& rhs) {
    // check for self-assignment
    if(this == &rhs) return *this;
    // delete existing rope to recover memory
    this->root_.reset();
    // invoke copy constructor
    this->root_ = make_unique<rope_node>(*(rhs.root_.get()));
    return *this;
  }
  
  // Determine if two ropes contain identical strings
  bool rope::operator ==(const rope& rhs) const {
    return this->toString() == rhs.toString();
  }
  
  // Determine if two ropes contain identical strings
  bool rope::operator !=(const rope& rhs) const {
    return !(*this == rhs);
  }
  
  // Print the rope
  std::ostream& operator<<(std::ostream& out, const rope& r) {
    return out << r.toString();
  }
  
  
  // Compute the nth Fibonacci number, in O(n) time
  size_t fib(size_t n) {
    // initialize first two numbers in sequence
    int a = 0, b = 1, next;
    if(n == 0) return a;
    for (size_t i = 2; i <= n; i++) {
      next = a + b;
      a = b;
      b = next;
    }
    return b;
  };
  
  // Construct a vector where the nth entry represents the interval between the
  //   Fibonacci numbers F(n+2) and F(n+3), and the final entry includes the number
  //   specified in the length parameter
  // e.g. buildFibList(0) -> {}
  //      buildFibList(1) -> {[1,2)}
  //      buildFibList(8) -> {[1,2),[2,3),[3,5),[5,8),[8,13)}
  std::vector<size_t> buildFibList(size_t len) {
    // initialize a and b to the first and second fib numbers respectively
    int a = 0, b = 1, next;
    std::vector<size_t> intervals = std::vector<size_t>();
    while (a <= len) {
      if (a > 0) {
        intervals.push_back(b);
      }
      next = a + b;
      a = b;
      b = next;
    }
    return intervals;
  }
  
}
