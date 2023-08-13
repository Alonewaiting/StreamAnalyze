#ifndef _TRRLIST_HPP
#define _TRRLIST_HPP
#include <vector>
#include <memory>

template<class TreeData>
class TreeNode {
public:
    TreeData m_element;
    std::string m_treeName;
    std::vector<std::shared_ptr<TreeNode<TreeData>>> m_children;
    std::shared_ptr<TreeNode<TreeData>> m_parent;
    TreeNode(const std::string & treeName) {
        m_parent = nullptr;
        m_children.clear();
        m_children.reserve(4);
        m_treeName = treeName;
    }
};
template<class TreeData>
class TreeList {
    using MNode =std::shared_ptr<TreeNode<TreeData>>;
private:
    MNode m_root;
public:
    TreeList(const std::string& treeName) {
        m_root = std::make_shared<TreeNode<TreeData>>(treeName);
    
    }
    void init(MNode root) {
        this->m_root = root;
    }
    void putChild(MNode node, MNode parent) {
        parent->m_children.push_back(node);
        node->m_parent = parent;    
    }
    MNode getRoot() {
        return m_root;
    }
    void putChildren(std::vector<MNode> nodes, MNode parent) {
        for (int i = 0; i < nodes.size(); ++i) {
            putChild(nodes[i], parent);
        }
    }
    std::vector<MNode> getChildren(MNode root) {
        return root->m_children;
    }
    MNode getParent(MNode root) {
        return root->m_parent;
    }
    void tranversal(MNode root) {
        
    }
    void tranversal() {
        this->tranversal(this->root);
    }
    int getMaxDepth(MNode root, std::vector<MNode> nodes) {

    }

};



#endif // _TRRLIST_HPP
