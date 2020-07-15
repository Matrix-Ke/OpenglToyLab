#include "util/sphere.h"


BasicShape::Sphere::Sphere(unsigned int num) : Shape( (num +1)* (num +1), 2 *num *num)
{
	//构造球体网格
	//球体的 normal和pos的值相同
	m_NormalArr = m_VertexArr;
	m_TexCoordsArr = std::vector<glm::vec2>(m_VertexNum);
	m_IndexArr = std::vector<glm::ivec3>(m_TriangleNum);

	float inc = 1.0f / num;
	for (unsigned int i = 0; i <= num; i++) {
		float u = inc * i;
		for (unsigned int j = 0; j <= num; j++) {
			float v = inc * j;
			float theta = PI * (1 - u);
			float phi = 2 * PI * v;
			// 右手系: 上y, 右x, 垂直屏幕外为 z
			m_VertexArr[i*(num + 1) + j].x = sinf(theta) * sinf(phi);
			m_VertexArr[i*(num + 1) + j].y = cosf(theta);
			m_VertexArr[i*(num + 1) + j].z = sinf(theta) * cosf(phi);
			// u 对应纵轴所以应该是纹理坐标的 t
			// v 对应横轴所以应该是纹理坐标的 s
			m_TexCoordsArr[i*(num + 1) + j].x = v;
			m_TexCoordsArr[i*(num + 1) + j].y = u;
		}
	}
	//------------
	for (unsigned int i = 0; i < num; i++) {
		for (unsigned int j = 0; j < num; j++) {
			// 左下 右下 左上
			m_IndexArr[2 * (i*num + j)].x = i * (num + 1) + j;
			m_IndexArr[2 * (i*num + j)].y = i * (num + 1) + j + 1;
			m_IndexArr[2 * (i*num + j)].z = (i + 1) * (num + 1) + j;
			// 右上 右下 左上
			m_IndexArr[2 * (i*num + j) + 1].x = (i + 1) * (num + 1) + j + 1;
			m_IndexArr[2 * (i*num + j) + 1].y = i * (num + 1) + j + 1;
			m_IndexArr[2 * (i*num + j) + 1].z = (i + 1) * (num + 1) + j;
		}
	}
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

std::vector<glm::ivec3> BasicShape::Sphere::GetIndexArr()
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
