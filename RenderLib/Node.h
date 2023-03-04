#pragma once
#include "stdafx.h"
#include "UpdateCallback.h"
class Visitor;

// abstract base class for a scene graph node
class Node
{
	std::vector<Node*> parents; // need this for conveniently deleting nodes, but not for traversal
	std::vector<Node*> children; 
	UpdateCallback* callback = nullptr;
public:
	void AddChild(Node* c) { children.push_back(c); c->parents.push_back(this); }
	Node* GetChild(int i) { return children[i]; }
	int GetNumChildren() { return children.size(); }
	Node* GetParents(int i) { return parents[i]; }
	int GetNumParents() { return parents.size(); }

	void RemoveChild( Node *c ) 
	{ 
		auto it = std::find(children.begin(), children.end(), c);
		if (it != children.end())
			children.erase(it);
	}

	void RemoveParent(Node* c)
	{
		auto it = std::find(parents.begin(), parents.end(), c);
		if (it != parents.end())
			parents.erase(it);
	}

	~Node()
	{
		// deal with disconnecting the graph here
		for (Node* parent : parents)
			parent->RemoveChild(this);
		for (Node* child : children)
			child->RemoveParent(this);
	}

	void SetUpdateCallback(UpdateCallback* cb ) { callback = cb; }
	UpdateCallback* GetUpdateCallback() const { return callback;  }
	virtual void Accept(Visitor* v) = 0;
};