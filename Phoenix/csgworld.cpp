#include "csgworld.hpp"

OpenCSG::Operation CsgNode::getOpenCsgEquivalent(CsgNode::OPERATION operation)
{
	switch(operation)
	{
		case CsgNode::OPERATION::SUBTRACTION:
			{
				return OpenCSG::Operation::Subtraction;
			}
			break;

		case CsgNode::OPERATION::INTERSECTION:
		default:
			{
				return OpenCSG::Operation::Intersection;
			}
			break;
	}
}

std::basic_ostream<TCHAR> & operator<<(std::basic_ostream<TCHAR> & out, _IN_(CsgNode & node))
{
	FileIO::Export::push(out, _T("operation"));
	out << static_cast<unsigned char>(node.operation) << FileIO::Export::pop;

	FileIO::Export::push(out, _T("object"));
	out << node.object->getObjectId() << FileIO::Export::pop;

	return out;
}

CsgTree::CsgTree():
generate(false)
{
}

CsgTree::~CsgTree()
{
	this->csgNodes.clear();

	this->deleteOpenCsgNodes();
}

void CsgTree::deleteOpenCsgNodes()
{
	for(std::vector<OpenCSG::Primitive*>::iterator i = this->openCsgNodes.begin(), j = this->openCsgNodes.end(); i != j; i++)
	{
		delete *i;
	}

	this->openCsgNodes.clear();
}

std::basic_ostream<TCHAR> & operator<<(std::basic_ostream<TCHAR> & out, _IN_(CsgTree & tree))
{
	FileIO::Export::push(out, _T("nodes"));
	for(std::vector<CsgNode>::const_iterator i = tree.csgNodes.begin(), j = tree.csgNodes.end(); i != j; i++)
	{
		FileIO::Export::push(out, _T("node"));
		out << *i << FileIO::Export::pop;
	}
	FileIO::Export::pop(out);

	return out;
}

CsgWorld::CsgWorld()
{
	OpenCSG::setOption(OpenCSG::AlgorithmSetting, OpenCSG::Algorithm::SCS);
	OpenCSG::setOption(OpenCSG::DepthComplexitySetting, OpenCSG::DepthComplexityAlgorithm::NoDepthComplexitySampling); 
	OpenCSG::setOption(OpenCSG::OffscreenSetting, OpenCSG::OffscreenType::AutomaticOffscreenType);
	OpenCSG::setOption(OpenCSG::DepthBoundsOptimization, OpenCSG::OptimizationOff);
}

CsgWorld::~CsgWorld()
{
	this->csgTrees.clear();
}

bool CsgWorld::addCsgTree(_IN_(std::basic_string<TCHAR> & treeName))
{
	CsgTree tree;

	std::pair<std::map<std::basic_string<TCHAR>, CsgTree>::iterator, bool> inserted = this->csgTrees.insert({treeName, tree});

	return inserted.second;
}

bool CsgWorld::addCsgNode(_IN_(std::basic_string<TCHAR> & treeName), GraphicObject* object)
{
	std::vector<CsgNode>* nodes = this->getTreeCsgNodes(treeName);

	if(nodes != nullptr)
	{
		CsgNode node{CsgNode::OPERATION::UNION, object};

		std::vector<CsgNode>::iterator location = std::find_if(nodes->begin(), nodes->end(), ComparatorCsgNode(object->getObjectId()));

		if(location == nodes->end())
		{
			ExceptionHandler::push_back<CsgNode>(*nodes, node);

			return true;
		}
	}

	return false;
}

bool CsgWorld::removeCsgNode(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position))
{
	std::vector<CsgNode>* nodes = this->getTreeCsgNodes(treeName);

	if(nodes != nullptr)
	{
		nodes->erase(nodes->begin() + position);

		return true;
	}

	return false;
}

const std::vector<CsgNode>* CsgWorld::getCsgNodes(_IN_(std::basic_string<TCHAR> & treeName))
{
	return this->getTreeCsgNodes(treeName);
}

void CsgWorld::setGenerate(_IN_(std::basic_string<TCHAR> & treeName))
{
	std::map<std::basic_string<TCHAR>, CsgTree>::iterator tree = this->csgTrees.find(treeName);

	if(tree != this->csgTrees.end())
	{
		tree->second.generate = true;
	}
}

void CsgWorld::generateOpenCsgTrees()
{
	CsgNode::OPERATION operation;

	for(std::map<std::basic_string<TCHAR>, CsgTree>::iterator i = this->csgTrees.begin(), j = this->csgTrees.end(); i != j; i++)
	{
		CsgTree & tree = i->second;

		if(tree.generate)
		{
			tree.deleteOpenCsgNodes();

			for(std::vector<CsgNode>::const_iterator k = tree.csgNodes.begin(), l = tree.csgNodes.end(); k != l; k++)
			{
				if(k == tree.csgNodes.begin())
				{
					operation = CsgNode::OPERATION::UNION;
				}
				else
				{
					operation = k->operation;
				}

				if(operation == CsgNode::OPERATION::UNION)
				{
					std::vector<CsgNode>::const_iterator next = std::next(k, 1);

					if(next != l)
					{
						if(next->operation != CsgNode::OPERATION::UNION)
						{
							operation = CsgNode::OPERATION::INTERSECTION;
						}
					}
				}

				if(operation != CsgNode::OPERATION::UNION)
				{
					OpenCSG::Primitive* object = new OpenCsgObject(CsgNode::getOpenCsgEquivalent(operation), 1, k->object);

					if(object != nullptr)
					{
						ExceptionHandler::push_back<OpenCSG::Primitive*>(tree.openCsgNodes, object);
					}
				}
			}

			tree.generate = false;
		}
	}
}

bool CsgWorld::isCsgTreeEmpty(_IN_(std::basic_string<TCHAR> & treeName))
{
	std::vector<CsgNode>* nodes = this->getTreeCsgNodes(treeName);

	if(nodes != nullptr)
	{
		return nodes->empty();
	}

	return true;
}

void CsgWorld::setNodeOperation(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position), _IN_(CsgNode::OPERATION & operation))
{
	std::vector<CsgNode>* nodes = this->getTreeCsgNodes(treeName);

	if(nodes != nullptr)
	{
		try
		{
			CsgNode & node = nodes->at(position);

			node.operation = operation;
		}
		catch(const std::out_of_range &)
		{
		}
	}
}

bool CsgWorld::getNodeOperation(_IN_(std::basic_string<TCHAR> & treeName), _IN_(unsigned int & position), _INOUT_(CsgNode::OPERATION & operation))
{
	std::vector<CsgNode>* nodes = this->getTreeCsgNodes(treeName);

	if(nodes != nullptr)
	{
		try
		{
			const CsgNode & node = nodes->at(position);

			operation = node.operation;

			return true;
		}
		catch(const std::out_of_range &)
		{
		}
	}

	return false;
}

bool CsgWorld::isObjectCsgNode(_IN_(GraphicObject* object))
{
	for(std::map<std::basic_string<TCHAR>, CsgTree>::iterator i = this->csgTrees.begin(), j = this->csgTrees.end(); i != j; i++)
	{
		CsgTree & tree = i->second;

		std::vector<CsgNode>::iterator location = std::find_if(tree.csgNodes.begin(), tree.csgNodes.end(), ComparatorCsgNode(object->getObjectId()));

		if(location != tree.csgNodes.end())
		{
			return true;
		}
	}

	return false;
}

void CsgWorld::drawObjects(_IN_(unsigned int & shaderProgram), _IN_(unsigned int & shaderProgramFixed))
{
	CsgNode::OPERATION operation;

	for(std::map<std::basic_string<TCHAR>, CsgTree>::iterator i = this->csgTrees.begin(), j = this->csgTrees.end(); i != j; i++)
	{
		CsgTree & tree = i->second;

		if(!tree.openCsgNodes.empty())
		{
			glUseProgram(0);

			OpenCSG::render(tree.openCsgNodes);

			glUseProgram(shaderProgramFixed);

			glDepthFunc(GL_EQUAL);
				for(std::vector<OpenCSG::Primitive*>::iterator k = tree.openCsgNodes.begin(), l = tree.openCsgNodes.end(); k != l; k++)
				{
					OpenCsgObject* object = dynamic_cast<OpenCsgObject*>(*k);

					if(object != nullptr)
					{
						object->setShaderProgram(shaderProgramFixed);
						object->render();
						object->setShaderProgram(0);
					}
				}
			glDepthFunc(GL_LESS);
		}

		if(!tree.csgNodes.empty())
		{
			glUseProgram(shaderProgram);

			for(std::vector<CsgNode>::const_iterator k = tree.csgNodes.begin(), l = tree.csgNodes.end(); k != l; k++)
			{
				if(k == tree.csgNodes.begin())
				{
					operation = CsgNode::OPERATION::UNION;
				}
				else
				{
					operation = k->operation;
				}

				if(operation == CsgNode::OPERATION::UNION)
				{
					std::vector<CsgNode>::const_iterator next = std::next(k, 1);

					if(next == l || next->operation == CsgNode::OPERATION::UNION)
					{
						k->object->drawObject(shaderProgram, SceneObject::DRAWMODE::CSG);
					}
				}
			}
		}
	}
}

void CsgWorld::save(std::basic_ofstream<TCHAR> & file)
{
	FileIO::Export::push(file, _T("trees"));
	for(std::map<std::basic_string<TCHAR>, CsgTree>::const_iterator i = this->csgTrees.begin(), j = this->csgTrees.end(); i != j; i++)
	{
		FileIO::Export::push(file, _T("tree"));
		FileIO::Export::push(file, _T("name"));
		file << i->first << FileIO::Export::pop << i->second;
		FileIO::Export::pop(file);
	}
	FileIO::Export::pop(file);
}

std::vector<CsgNode>* CsgWorld::getTreeCsgNodes(_IN_(std::basic_string<TCHAR> & treeName))
{
	std::vector<CsgNode>* nodes = nullptr;

	std::map<std::basic_string<TCHAR>, CsgTree>::iterator tree = this->csgTrees.find(treeName);

	if(tree != this->csgTrees.end())
	{
		nodes = &(tree->second.csgNodes);
	}

	return nodes;
}