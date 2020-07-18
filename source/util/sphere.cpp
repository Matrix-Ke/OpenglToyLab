#include "util/sphere.h"


BasicShape::Sphere::Sphere(unsigned int stacks) : Shape((stacks + 1)* (stacks + 1), 2 * stacks *stacks)
{
	//构造球体网格

	m_TexCoordsArr = std::vector<glm::vec2>(m_VertexNum);
	m_IndexArr = std::vector<glm::uvec3>(m_TriangleNum);

	//球体绘制是从上往下，从左到右
	float inc = 1.0f / stacks;
	for (unsigned int i = 0; i <= stacks; i++) {
		float u = inc * i;
		//必须相等否则三角面片衔接不上
		for (unsigned int j = 0; j <= stacks; j++) {
			float v = inc * j;
			//float theta = PI * (1 - u);
			float theta = PI * u;
			float phi = 2 * PI * v;
			// 右手系: 上y, 右x, 垂直屏幕外为 z
			m_positionArr[i*(stacks + 1) + j].x = sinf(theta) * cosf(phi);
			m_positionArr[i*(stacks + 1) + j].y = cosf(theta);
			m_positionArr[i*(stacks + 1) + j].z = sinf(theta) * sinf(phi);
			// u 对应纵轴所以应该是纹理坐标的 t
			// v 对应横轴所以应该是纹理坐标的 s
			m_TexCoordsArr[i*(stacks + 1) + j].x = v;
			m_TexCoordsArr[i*(stacks + 1) + j].y = u;
		}
	}
	//------------
	for (unsigned int i = 0; i < stacks; i++) {
		for (unsigned int j = 0; j < stacks; j++) {
			// 1  ----->  0
			// |		  |
			// V		  V
			// 3  ----->  2
			//---------------0, 1, 2
			m_IndexArr[i * 2 * stacks + 2 * j].x = i * (stacks + 1) + j;
			m_IndexArr[i * 2 * stacks + 2 * j].y = i * (stacks + 1) + j + 1;
			m_IndexArr[i * 2 * stacks + 2 * j].z = (i + 1) * (stacks + 1) + j;
			//1, 3, 2
			m_IndexArr[i * 2 * stacks + 2 * j + 1].x = i * (stacks + 1) + j + 1;
			m_IndexArr[i * 2 * stacks + 2 * j + 1].y = (i + 1) * (stacks + 1) + j + 1;
			m_IndexArr[i * 2 * stacks + 2 * j + 1].z = (i + 1) * (stacks + 1) + j;
		}
	}
	//球体的 normal和pos的值相同
	m_NormalArr = m_positionArr;
}

BasicShape::Sphere::~Sphere()
{

}

std::vector<glm::vec3> BasicShape::Sphere::GetNormalArr()
{
	return m_NormalArr;
}

std::vector<glm::vec2> BasicShape::Sphere::GetTexCoordsArr()
{
	return m_TexCoordsArr;
}

std::vector<glm::uvec3> BasicShape::Sphere::GetIndexArr()
{
	return m_IndexArr;
}

unsigned int BasicShape::Sphere::GetNormalArrSize()
{
	return m_NormalArr.size();
}

unsigned int BasicShape::Sphere::GetTexCoordsArrSize()
{
	return m_TexCoordsArr.size();
}

unsigned int BasicShape::Sphere::GetIndexArrSize()
{
	return m_IndexArr.size();
}
