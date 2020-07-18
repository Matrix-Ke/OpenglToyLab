#include "util/basicShape.h"


using namespace BasicShape;

const float Shape::PI = 3.1415926;


BasicShape::Shape::Shape(unsigned int vertexNum /*= 0*/, unsigned int triNum /*= 0*/) : m_VertexNum(vertexNum), m_TriangleNum(triNum)
{
		m_positionArr = std::vector<glm::vec3>(vertexNum);
}

std::vector<glm::vec3> BasicShape::Shape::GetVertexArr()
{
	return m_positionArr;
}

unsigned int BasicShape::Shape::GetTriNum()
{
	return m_TriangleNum;
}

unsigned int BasicShape::Shape::GetVertexArrSize()
{
	return m_positionArr.size();
}

BasicShape::Shape::~Shape()
{

}

