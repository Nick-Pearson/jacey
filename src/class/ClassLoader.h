#pragma once

#include <istream>

class JClass;

class ClassLoader
{
public:
 
  JClass* loadClass(const char* classPath);

private:

void newMethod();

  JClass* parseClass(std::istream& stream);

  bool parseConstant(std::istream& stream, struct Constant& outConstant);

  unsigned int readUInt(std::istream& stream);
  unsigned short readUShort(std::istream& stream);
};
