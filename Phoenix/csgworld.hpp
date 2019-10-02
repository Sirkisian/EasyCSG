#pragma once
//-----
#include <map>
#include "opencsgobject.hpp"
#include "graphicworld.hpp"
//----
//---
//--
class CsgNode
{
	public:
		enum class OPERATION : unsigned char {INTERSECTION, SUBTRACTION, UNION};

		static OpenCSG::Operation getOpenCsgEquivalent(CsgNode::OPERATION operation);

		CsgNode::OPERATION operation;

		GraphicObject* object;
};

class ComparatorCsgNode
{
	public:
		ComparatorCsgNode(unsigned int id):
		id(id)
		{
		}

		bool operator()(_IN_(CsgNode & node))
		{
			return node.object->getObjectId() == this->id;
		}

	private:
		unsigned int id;
};

class CsgTree
{
	public:
		CsgTree();
		~CsgTree();

		void deleteOpenCsgNodes();

		std::vector<CsgNode> csgNodes;
		std::vector<OpenCSG::Primitive*> openCsgNodes;

		bool generate;
};

class ComparatorCsgTree
{
	public:
		inline bool operator()(_IN_(std::basic_string<TCHAR> & str1), _IN_(std::basic_string<TCHAR> & str2)) const
		{
			return str1.compare(str2) < 0;
		}
};

class CsgWorld
{
	public:
		CsgWorld();
		~CsgWorld();

		bool addCsgTree(_IN_(std::basic_string<TCHAR> & treeName));

		bool addCsgNode(_IN_(std::basic_string<TCHAR> & treeName), GraphicObject* object);
		bool removeCsgNode(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position));
		const std::vector<CsgNode>* getCsgNodes(_IN_(std::basic_string<TCHAR> & treeName));

		void setGenerate(_IN_(std::basic_string<TCHAR> & treeName));

		void generateOpenCsgTrees();

		bool isCsgTreeEmpty(_IN_(std::basic_string<TCHAR> & treeName));

		void setNodeOperation(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position), _IN_(CsgNode::OPERATION & operation));
		bool getNodeOperation(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position), _OUT_(CsgNode::OPERATION & operation));

		void drawObjects(_IN_(unsigned int & shaderProgram), _IN_(unsigned int & shaderProgramFixed));

	private:
		std::vector<CsgNode>* getTreeCsgNodes(_IN_(std::basic_string<TCHAR> & treeName));

		std::map<std::basic_string<TCHAR>, CsgTree, ComparatorCsgTree> csgTrees;
};