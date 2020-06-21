#pragma once
#include "util/filesystem.h"

#include <cstdlib>
#include <cstdarg>

using namespace std;



std::string FileSystem::getPath(const std::string& path)
{
	static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
	return (pathBuilder)(path);
}

std::string const FileSystem::getRoot()
{
	std::string OpenGL_root(getCurrentDir());
	OpenGL_root += "/../";
	//static char const * envRoot = _dupenv_s("OpenGL_ROOT_PATH");
	//static char const * givenRoot = (envRoot != nullptr ? envRoot : OpenGL_root.c_str());
	//static std::string root = (givenRoot != nullptr ? givenRoot : "");
	return OpenGL_root;
}

FileSystem::Builder FileSystem::getPathBuilder()
{
	if (getRoot() != "")
		return &FileSystem::getPathRelativeRoot;
	else
		return &FileSystem::getPathRelativeBinary;
}

std::string FileSystem::getPathRelativeRoot(const std::string& path)
{
	return getRoot() + std::string("/") + path;
}

std::string FileSystem::getPathRelativeBinary(const std::string& path)
{
	return "../../../" + path;
}




FileSystem::FileSystem(const std::string & fileName, Mode mode) {
	this->fileName = fileName;
	this->mode = mode;
	fopen_s(&pF, fileName.c_str(), ModeToStr(mode).c_str());
	if (pF == NULL)
		fprintf(stderr, "%s open error\n", fileName.c_str());
}

FileSystem::~FileSystem() {
	if (pF != NULL) {
		fclose(pF);
		pF = NULL;
	}
}

bool FileSystem::Check(Mode mode) {
	if (mode == this->mode && IsValid())
		return true;
	else
		return false;
}

int FileSystem::Printf(const std::string & format, ...) {
	if (!Check(WRITE)) {
		fprintf(stderr, "%s can't not write\n", fileName.c_str());
		return -1;
	}
	va_list args;
	const char * cstr_format = format.c_str();
	va_start(args, cstr_format);
	int rst = vfprintf(pF, format.c_str(), args);
	va_end(args);
	return rst;
}

int FileSystem::Scanf(const std::string & format, ...) {
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return -1;
	}
	va_list args;
	const char * cstr_format = format.c_str();
	va_start(args, cstr_format);
	int rst = vfscanf(pF, format.c_str(), args);
	va_end(args);
	return rst;
}

string FileSystem::ReadLine() {
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return "";
	}
	char buffer[1024] = "";
	fgets(buffer, 1024, pF);
	return buffer;
}

string FileSystem::ReadAll() {
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return "";
	}
	string rst;
	while (!IsEnd())
		rst += ReadLine();
	return rst;
}

bool FileSystem::IsEnd() {
	return feof(pF);
}

bool FileSystem::IsValid() {
	return pF != NULL;
}

string FileSystem::ModeToStr(Mode mode) {
	switch (mode)
	{
	case READ:
		return "r";
		break;
	case WRITE:
		return "w";
		break;
	default:
		return "";
		break;
	}
}
