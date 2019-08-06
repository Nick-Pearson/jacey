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
