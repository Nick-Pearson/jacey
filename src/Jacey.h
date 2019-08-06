#pragma once

#include "IJacey.h"

extern class IJacey* JVM;

class Jacey : public IJacey
{
public:
  Jacey();
  ~Jacey();

  void run(int argc, const char** argv);

  inline int getExitCode() const { return exitCode; }

private:
  class ClassLoader* classLoader = nullptr;

  int exitCode = 0;
};
