#pragma once

#include "util/basicShape.h"

namespace BasicShape
{
	class Sphere : public Shape 
	{
	public:
		Sphere(unsigned int num);
		virtual ~Sphere();
		std::vector<glm::vec3> GetNormalArr();
		std::vector<glm::vec2> GetTexCoordsArr();
		std::vector<glm::ivec3> GetIndexArr();
		unsigned int  GetNormalArrSize();
		unsigned int  GetTexCoordsArrSize();
		unsigned int  GetIndexArrSize(); 
	protected:
		std::vector<glm::vec3> m_NormalArr;
		std::vector<glm::vec2> m_TexCoordsArr;
		std::vector<glm::ivec3> m_IndexArr;
	private:
	};
}




