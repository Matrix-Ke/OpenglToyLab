#pragma once
#include "util/VAO.h"


#include <glad/glad.h>
#include <iostream>

using namespace std;
using namespace OpenGL;



VAO::VAO(float const * data, unsigned int dataSize, const std::vector<unsigned int> & attrLen) {
	if (data == NULL || dataSize == 0 || attrLen.size() == 0) {
		isValid = false;
		ID = 0;
		return;
	}

	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	VBOs.push_back(VBO);

	unsigned int patchLen = 0;
	for (auto & len : attrLen)
		patchLen += len;
	for (unsigned int i = 0, accu = 0; i < attrLen.size(); i++) 
	{
		if (attrLen[i] == 0)
			continue;

		glVertexAttribPointer(i, attrLen[i], GL_FLOAT, GL_FALSE, patchLen * sizeof(float), (void*)(accu * sizeof(float)));
		glEnableVertexAttribArray(i);
		accu += attrLen[i];
	}
	attrNum = attrLen.size();
	//按照没有索引的情况设置 pointNum
	this->pointNum = dataSize / (sizeof(float) * patchLen);
	isValid = true;
	hasIndex = false;
}

VAO::VAO(float const * data, unsigned int dataSize, const std::vector<unsigned int> & attrLen, unsigned int const * index, unsigned int indexSize)
	: VAO(data, dataSize, attrLen) {
	if (IsValid()) {
		pointNum = indexSize / sizeof(unsigned int);
		hasIndex = true;
		isValid = GenBindEBO(index, indexSize);
	}
}

VAO::VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch, const std::vector<unsigned int> & divisors) {
	if (vec_VBO_DataPatch.size() == 0
		|| (divisors.size() > 0 && vec_VBO_DataPatch.size() != divisors.size())) {
		isValid = false;
		return;
	}

	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);

	for (unsigned int i = 0; i < vec_VBO_DataPatch.size(); i++) {
		auto & dataPatch = vec_VBO_DataPatch[i];
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, dataPatch.dataSize, dataPatch.data, GL_STATIC_DRAW);
		glVertexAttribPointer(i, dataPatch.attrLen, GL_FLOAT, GL_FALSE, dataPatch.attrLen * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(i);
		if (divisors.size() != 0 && divisors[i] > 0)
			glVertexAttribDivisor(i, divisors[i]);

		VBOs.push_back(VBO);
	}
	//按照没有索引的情况设置 pointNum
	this->pointNum = vec_VBO_DataPatch[0].dataSize / (sizeof(float) * vec_VBO_DataPatch[0].attrLen);
	hasIndex = false;
	isValid = true;
}

VAO::VAO(const std::vector<VBO_DataPatch> & vec_VBO_DataPatch, unsigned int const * index, unsigned int indexSize, const std::vector<unsigned int> & divisors)
	: VAO(vec_VBO_DataPatch, divisors) {
	if (IsValid()) {
		pointNum = indexSize / sizeof(unsigned int);
		hasIndex = true;
		isValid = GenBindEBO(index, indexSize);
	}
}

//------------
//unsigned int attrNum;
//unsigned int ID;
//unsigned int pointNum;
//bool hasIndex;
//bool isValid;
OpenGL::VAO::VAO(const VAO& rhs)
{
	this->attrNum = rhs.attrNum;
	this->ID = rhs.ID;
	this->pointNum = rhs.pointNum;
	this->hasIndex = rhs.hasIndex;
	this->isValid = rhs.isValid;
}

OpenGL::VAO::~VAO()
{
	if (isValid && ID)
	{
		glDeleteVertexArrays(1, &ID);
		for (int i = 0; i < VBOs.size(); i++ )
		{
			unsigned int bIndex = VBOs[i];
			glDeleteBuffers(1, &bIndex);
		}
	}
}

unsigned int OpenGL::VAO::Size() const
{
	return this->pointNum;
}

bool OpenGL::VAO::indexValid() const
{
	return this->hasIndex;
}

bool VAO::GenBindEBO(unsigned int const * index, unsigned int indexSize)
{
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, index, GL_STATIC_DRAW);
	return true;
}

unsigned int VAO::GetID() const 
{
	return ID;
}

unsigned int VAO::GetAttrNum() const
{
	return ID;
}

bool VAO::IsValid() const
{
	return isValid;
}

bool VAO::Use() const
{
	if (!IsValid())
		return false;

	glBindVertexArray(ID);
	return true;
}

bool VAO::Draw() const 
{
	this->Use();

	if (hasIndex)
		glDrawElements(GL_TRIANGLES, pointNum, GL_UNSIGNED_INT, NULL);
	else
		glDrawArrays(GL_TRIANGLES, 0, pointNum);

	return true;
}