#include "carve.hpp"

GraphicObjectC* Carve::calculate(_IN_(std::vector<CsgNode>* nodes))
{
	if(nodes->size() > 0)
	{
		carve::csg::CSG::OP operation;
		carve::mesh::MeshSet<3>* csgObject = nullptr;
		carve::mesh::MeshSet<3>* csgObjectTmp = nullptr;
		carve::mesh::MeshSet<3>* csgObjectLeft = nullptr;
		carve::mesh::MeshSet<3>* csgObjectRight = nullptr;
		carve::interpolate::FaceVertexAttr<TSTRUCT> fv_tex;
		carve::interpolate::FaceAttr<unsigned int> f_tex_num;

		for(std::vector<CsgNode>::const_iterator i = nodes->begin(), j = nodes->end(); i != j; i++)
		{
			const GraphicObjectG* object = dynamic_cast<GraphicObjectG*>(i->object);

			if(object != nullptr)
			{
				csgObjectTmp = Carve::createMesh(object, fv_tex, f_tex_num);
			}

			if(csgObject == nullptr)
			{
				csgObject = csgObjectTmp;
			}
			else
			{
				carve::csg::CSG csg;

				//fv_tex.installHooks(csg);
				//f_tex_num.installHooks(csg);

				csgObjectLeft = csgObject;
				csgObjectRight = csgObjectTmp;

				switch(i->operation)
				{
					case CsgNode::OPERATION::INTERSECTION:
						{
							operation = carve::csg::CSG::OP::INTERSECTION;
						}
						break;

					case CsgNode::OPERATION::SUBTRACTION:
						{
							operation = carve::csg::CSG::OP::A_MINUS_B;
						}
						break;

					case CsgNode::OPERATION::UNION:
						{
							operation = carve::csg::CSG::OP::UNION;
						}
						break;
				}

				try
				{
					csgObject = csg.compute(csgObjectLeft, csgObjectRight, operation, nullptr, carve::csg::CSG::CLASSIFY_NORMAL);
				}
				catch(...)
				{
				}

				delete csgObjectLeft;
				delete csgObjectRight;
			}
		}

		if(csgObject != nullptr)
		{
			return new GraphicObjectC(csgObject, fv_tex, f_tex_num);
		}
	}

	return nullptr;
}

carve::mesh::MeshSet<3>* Carve::createMesh(_IN_(GraphicObjectG* object),
										   _OUT_(carve::interpolate::FaceVertexAttr<TSTRUCT> & fv_tex),
										   _OUT_(carve::interpolate::FaceAttr<unsigned int> & f_tex_num))
{
	glm::mat4 transformation(1.0f);
	object->transformation.multModelMatrix(transformation);
	float* floatArray = glm::value_ptr(transformation);

	carve::math::Matrix carveMatrix;
	std::copy(floatArray, floatArray + 16, carveMatrix.v);

	const std::array<float, 4> parameters = object->parameters;

	switch(object->type)
	{
		case Object::OBJECTTYPE::CUBE:
			{
				return MeshSet::primitiveCube(parameters[0], parameters[1], parameters[2], object->getTexture(), carveMatrix, fv_tex, f_tex_num);
			}
			break;

		case Object::OBJECTTYPE::SPHERE:
			{
				return MeshSet::primitiveSphere(parameters[0], static_cast<unsigned short>(parameters[1]), static_cast<unsigned short>(parameters[2]), object->getTexture(), carveMatrix, fv_tex, f_tex_num);
			}
			break;

		case Object::OBJECTTYPE::CYLINDER:
			{
				return MeshSet::primitiveCylinder(parameters[0], parameters[1], static_cast<GLushort>(parameters[2]), object->getTexture(), carveMatrix, fv_tex, f_tex_num);
			}
			break;
	}

	return nullptr;
}