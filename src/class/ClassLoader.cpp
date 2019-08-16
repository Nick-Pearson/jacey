#include "ClassLoader.h"
#include "JClass.h"
#include "Constant.h"

#include <iostream>
#include <fstream>

namespace
{
  unsigned int MAGIC_NUMBER = 0xCAFEBABE;
}

JClass* ClassLoader::loadClass(const char* classPath)
{
  std::string filePath;

  const char* cp = classPath;
  while(*cp != '\0')
  {
    if (*cp == '.')
      filePath.push_back('/');
    else
      filePath.push_back(*cp);

    cp++;
  }

  filePath.append(".class");

  std::cout << "Loading class from file " << filePath << std::endl;

  std::ifstream stream(filePath);

  if (!stream.is_open())
  {
    std::cout << "Cannot open file " << filePath << std::endl;
    return nullptr;
  }

  JClass* c = parseClass(stream);
  stream.close();
  return c;
}

JClass* ClassLoader::parseClass(std::istream& stream)
{
  unsigned int magicNumber = readUInt(stream);

  if (magicNumber != MAGIC_NUMBER)
  {
    std::cout << "Corrupted .class file" << std::endl;
    return nullptr;
  }

  unsigned short minor = readUShort(stream);
  unsigned short major = readUShort(stream);

  if (major != 52)
  {
    std::cout << "Unsupported .class file version " << major << "." << minor << std::endl;
    return nullptr;
  }

  unsigned short constantPoolCount = readUShort(stream);
  std::cout << "Got " << constantPoolCount << " constants" << std::endl;

  JClass* c = new JClass{};

  for (unsigned short i = 0; i < constantPoolCount; ++i)
  {
    Constant constant;
    parseConstant(stream, constant);
  }

  return c;
}

bool ClassLoader::parseConstant(std::istream& stream, Constant& outConstant)
{
  stream.read((char*)&outConstant.type, 1);

  printf("%.20s", constantTypeToString(outConstant.type));

  switch (outConstant.type)
  {
  case ConstantType::Utf8:
  {
    unsigned short length = readUShort(stream);
    char str[length + 2];
    stream.read(str, length);
    str[length] = '*';
    str[length + 1] = 0;
    std::cout << "= (" << length << ") " << str;
    break;
  }
  case ConstantType::Integer:
  case ConstantType::Float:
    stream.ignore(4);
    break;
  case ConstantType::Long:
  case ConstantType::Double:
    stream.ignore(8);
    break;
  case ConstantType::Class:
  {
    unsigned short idx = readUShort(stream);
    std::cout << idx;
    break;
  }
  case ConstantType::String:
    stream.ignore(2);
    break;
  case ConstantType::Fieldref:
  case ConstantType::InterfaceMethodref:
  case ConstantType::Methodref:
    stream.ignore(4);
    break;
  case ConstantType::NameAndType:
    stream.ignore(4);
    break;
  case ConstantType::MethodHandle:
    stream.ignore(4);
    break;
  case ConstantType::MethodType:
    stream.ignore(2);
    break;
  case ConstantType::InvokeDynamic:
    stream.ignore(4);
    break;
  }

  std::cout << std::endl;
  return true;
}

unsigned int ClassLoader::readUInt(std::istream& stream)
{
  unsigned int val = 0;
  stream.read((char*)&val + 3, 1);
  stream.read((char*)&val + 2, 1);
  stream.read((char*)&val + 1, 1);
  stream.read((char*)&val, 1);
  return val;
}

unsigned short ClassLoader::readUShort(std::istream& stream)
{
  unsigned short val = 0;
  stream.read((char*)&val + 1, 1);
  stream.read((char*)&val, 1);
  return val;
}
