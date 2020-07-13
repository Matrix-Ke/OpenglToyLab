#pragma once

namespace BasicShape
{
	class Shape
	{
	public:
		Shape(unsigned int vertexNum = 0, unsigned int triangleNum  = 0);

		float * GetVertexArr();
		size_t GetTriNum();
		size_t GetVertexArrSize();


		virtual ~Shape();


	protected:
		static const float PI;



	private:

	};
}