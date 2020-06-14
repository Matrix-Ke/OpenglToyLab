#pragma once
#include "util/filesystem.h"
#include "util/LStorage.h"

namespace Oper
{
	class Config
	{
	public:
		Config();

		Config(const std::string &filename);

		bool Load(const std::string & filename);

		std::string *  GetStrPtr(const std::string &id);

		float *  GetFloatPtr(const std::string & id);

		int * GetIntPtr(const std::string & id);
		bool IsValid();
	private:
		bool DecodeLine(const std::string & data);
		//------------
		LStorage<std::string> strDirectory;
		LStorage<float> floatDirectory;
		LStorage<int> intDirectory;
		bool valid;

	};
}
