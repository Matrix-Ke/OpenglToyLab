#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace BasicShape
{
	class Shape
	{
	public:
		Shape(unsigned int vertexNum = 0, unsigned int triNum = 0);

		std::vector<glm::vec3> GetVertexArr();
		unsigned int  GetTriNum();
		unsigned int GetVertexArrSize();


		virtual ~Shape();

	protected:
		static const float PI;
		unsigned int	m_TriangleNum;
		unsigned int	m_VertexNum;
		std::vector<glm::vec3>  m_VertexArr;


	private:

	};
}