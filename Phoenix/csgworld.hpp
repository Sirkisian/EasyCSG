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
		enum class OPERATION : unsigned char {NOOPERATION, INTERSECTION, SUBTRACTION, UNION};

		static inline CsgNode::OPERATION convertOperation(unsigned char operation)
		{
			return EnumConverter::convert<CsgNode::OPERATION>(operation, CsgNode::OPERATION::INTERSECTION, CsgNode::OPERATION::UNION);
		}

		static OpenCSG::Operation getOpenCsgEquivalent(CsgNode::OPERATION operation);

		friend std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(CsgNode & node));

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

		friend std::basic_ostream<TCHAR> & operator<<(_INOUT_(std::basic_ostream<TCHAR> & out), _IN_(CsgTree & tree));

		std::vector<CsgNode> csgNodes;
		std::vector<OpenCSG::Primitive*> openCsgNodes;

		bool generate;
};

class CsgWorld
{
	public:
		CsgWorld();
		~CsgWorld();

		bool addCsgTree(_IN_(std::basic_string<TCHAR> & treeName));

		void getCsgTreeNames(_OUT_(std::vector<std::basic_string<TCHAR>> & treeNames));

		bool addCsgNode(_IN_(std::basic_string<TCHAR> & treeName), GraphicObject* object, CsgNode::OPERATION operation = CsgNode::OPERATION::UNION);
		bool removeCsgNode(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position));
		const std::vector<CsgNode>* getCsgNodes(_IN_(std::basic_string<TCHAR> & treeName));

		void setGenerate(_IN_(std::basic_string<TCHAR> & treeName));

		void generateOpenCsgTrees();

		bool isCsgTreeEmpty(_IN_(std::basic_string<TCHAR> & treeName));

		void setNodeOperation(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position), _IN_(CsgNode::OPERATION & operation));
		bool getNodeOperation(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position), _OUT_(CsgNode::OPERATION & operation));

		bool isObjectCsgNode(_IN_(GraphicObject* object));

		void drawObjects(_IN_(unsigned int & shaderProgram), _IN_(unsigned int & shaderProgramFixed));

		void save(_INOUT_(std::basic_ofstream<TCHAR> & file));
		void load(_IN_(rapidxml::xml_node<TCHAR>* parentNode), _IN_(GraphicWorld & graphicWorld));

		inline void clear()
		{
			this->csgTrees.clear();
		}

	private:
		std::vector<CsgNode>* getTreeCsgNodes(_IN_(std::basic_string<TCHAR> & treeName));

		std::map<std::basic_string<TCHAR>, CsgTree> csgTrees;
};