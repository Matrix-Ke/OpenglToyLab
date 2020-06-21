#pragma once
#include<vector>

namespace OpenGL {
	class VAO {
	public:
		struct VBO_DataPatch{
			float const * data;
			unsigned int dataSize;
			unsigned int attrLen;
		};

		VAO(float const * data, unsigned int dataSize, const std::vector<unsigned int> & attrLen);
		VAO(float const * data, unsigned int dataSize, const std::vector<unsigned int> & attrLen, unsigned int const * index, unsigned int indexSize);
		VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch, const std::vector<unsigned int> & divisors = std::vector<unsigned int>());
		VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch, unsigned int const * index, unsigned int indexSize, const std::vector<unsigned int> & divisors = std::vector<unsigned int>());
		VAO(const VAO& rhs);
		~VAO();


		unsigned int GetID() const;
		unsigned int GetAttrNum() const;
		unsigned int Size() const;

		bool IsValid() const;
		bool Use() const;
		bool Draw() const;
		bool indexValid() const;


	private:
		bool GenBindEBO(unsigned int const * index, unsigned int indexSize);
		//------------
		unsigned int attrNum;
		unsigned int ID;
		std::vector<unsigned int>  VBOs;
		unsigned int pointNum;
		bool hasIndex;
		bool isValid;
	};
}
