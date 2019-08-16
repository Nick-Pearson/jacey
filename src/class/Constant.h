#pragma once

enum class ConstantType : unsigned char
{
  Utf8	= 1,
  Integer = 3,
  Float = 4,
  Long = 5,
  Double = 6,
  Class = 7,
  String = 8,
  Fieldref = 9,
  Methodref = 10,
  InterfaceMethodref = 11,
  NameAndType = 12,
  MethodHandle = 15,
  MethodType = 16,
  InvokeDynamic = 18,

  INVALID = 99,
};

struct Constant
{
  ConstantType type = ConstantType::INVALID;
  unsigned char* data = nullptr;
};

const char* constantTypeToString(ConstantType type)
{
  switch(type)
  {
  case ConstantType::Utf8:
    return "UTF-8  ";
  case ConstantType::Integer:
    return "Integer";
  case ConstantType::Float:
    return "Float";
  case ConstantType::Long:
    return "Long";
  case ConstantType::Double:
    return "Double";
  case ConstantType::Class:
    return "Class";
  case ConstantType::String:
    return "String";
  case ConstantType::Fieldref:
    return "Fieldref";
  case ConstantType::Methodref:
    return "Methodref";
  case ConstantType::InterfaceMethodref:
    return "IfaceMethodRef";
  case ConstantType::NameAndType:
    return "NameAndType";
  case ConstantType::MethodHandle:
    return "MethodHandle";
  case ConstantType::MethodType:
    return "MethodType";
  case ConstantType::InvokeDynamic:
    return "InvokeDynamic";
  default:
    return "INVALID";
  }
}
