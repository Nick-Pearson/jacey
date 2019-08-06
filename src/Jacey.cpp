#include "Jacey.h"

#include "class/ClassLoader.h"
#include "class/JClass.h"

#include <iostream>

IJacey* JVM = nullptr;

int main(int argc, const char** argv)
{
  Jacey* jvm = new Jacey;
  JVM = jvm;

  jvm->run(argc, argv);

  int exitCode = jvm->getExitCode();

  JVM = nullptr;
  delete jvm;

  return exitCode;
}

Jacey::Jacey()
{
  classLoader = new ClassLoader{};
}

Jacey::~Jacey()
{
  delete classLoader;
}

void Jacey::run(int argc, const char** argv)
{
  if (argc < 2)
  {
    std::cout << "Not enough arguments" << std::endl;
    exitCode = 1;
    return;
  }
  const char* classpath = argv[1];

  std::cout << "Running \"" << classpath << "\"" << std::endl;
  JClass* mainClass = classLoader->loadClass(classpath);
  std::string msg = mainClass->toString();
  std::cout << msg << std::endl;
}
