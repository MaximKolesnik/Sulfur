#include <d3dcompiler.h>
#include <iostream>
#include <fstream>


struct CompilerOptions
{
  bool Debug;
  bool NoOptimizations;
  int OptimizationLevel;
};

static CompilerOptions compilerOptions = { false, false, 3 };

static void Usage();
static void CompileShaders(const std::string& dir, const std::string& outDir);
static void CompileShaderD3D(const std::string& in, const std::string& out, const std::string& target);
static std::string ExtractExtension(std::string& str);

#define USAGE() { Usage(); return 0; }

int main(int argc, char **argv)
{
  if (argc < 3)
    USAGE();
  
  for (int i = 1; i < argc - 2; ++i)
  {
    if (argv[i][0] != '-')
      USAGE();

    switch (argv[i][1])
    {
    case 'd': compilerOptions.Debug = true; break;
    case 'O':
      if (argv[i][2] == 's')
        compilerOptions.NoOptimizations = true;
      else if (argv[i][2] >= '0' && argv[i][2] <= '3')
        compilerOptions.OptimizationLevel = argv[i][2] - '0';
      else
        USAGE();
      break;
    default:
      USAGE();
    }
  }

  CompileShaders(argv[argc - 2], argv[argc - 1]);

  return 0;

}

static void Usage()
{
  std::cout << "usage: ShaderCompiler [options] src_dir out_dir" << std::endl;
  std::cout << "  options:" << std::endl;
  std::cout << "    -d    Debug compilation" << std::endl;
  std::cout << "    -On   Optimization level (n: 0-3, lower is less optimization)" << std::endl;
  std::cout << "    -Os   Skip optimizations" << std::endl;
}

static void CompileShaders(const std::string& dir, const std::string& outDir)
{
  HANDLE hFind;
  WIN32_FIND_DATA data;

  std::string dirWC = dir + "\\*";
  hFind = FindFirstFile((dir + "\\*").c_str(), &data);
  if (hFind != INVALID_HANDLE_VALUE) {
    do 
    {
      if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
      else
      {
        std::string fileName = data.cFileName;
        std::string ext = ExtractExtension(fileName);

        if (ext == "hlsl")
        {
          switch (data.cFileName[0])
          {
          case 'V': CompileShaderD3D(dir + "\\" + data.cFileName, outDir + "\\" + fileName + ".sbin", "vs_5_0"); break;
          case 'C': CompileShaderD3D(dir + "\\" + data.cFileName, outDir + "\\" + fileName + ".sbin", "cs_5_0"); break;
          case 'D': CompileShaderD3D(dir + "\\" + data.cFileName, outDir + "\\" + fileName + ".sbin", "ds_5_0"); break;
          case 'G': CompileShaderD3D(dir + "\\" + data.cFileName, outDir + "\\" + fileName + ".sbin", "gs_5_0"); break;
          case 'H': CompileShaderD3D(dir + "\\" + data.cFileName, outDir + "\\" + fileName + ".sbin", "hs_5_0"); break;
          case 'P': CompileShaderD3D(dir + "\\" + data.cFileName, outDir + "\\" + fileName + ".sbin", "ps_5_0"); break;
          }
        }
      }

    } while (FindNextFile(hFind, &data));
    FindClose(hFind);
  }
}

static void CompileShaderD3D(const std::string& in, const std::string& out, const std::string& target)
{
  std::ifstream istr(in.c_str(), std::ios_base::binary);

  istr.seekg(0, std::ios_base::end);
  unsigned size = (unsigned)istr.tellg();
  istr.seekg(0, std::ios_base::beg);

  char *buffer = new char[size];
  istr.read(buffer, size);

  UINT32 flags = 0;

  if (compilerOptions.Debug)
    flags |= D3DCOMPILE_DEBUG;
  
  if (compilerOptions.NoOptimizations)
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
  else
    switch (compilerOptions.OptimizationLevel)
    {
    case 0: flags |= D3DCOMPILE_OPTIMIZATION_LEVEL0; break;
    case 1: flags |= D3DCOMPILE_OPTIMIZATION_LEVEL1; break;
    case 2: flags |= D3DCOMPILE_OPTIMIZATION_LEVEL2; break;
    case 3: flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3; break;
    }

  ID3DBlob *blob = nullptr, *error = nullptr;
  if (FAILED(D3DCompile(buffer, size, in.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", target.c_str(), flags, 0, &blob, &error)))
  {
    std::cout << "Shader failed to compile: " << in.c_str() << std::endl;
    std::cout << (char*)error->GetBufferPointer() << std::endl;
    return;
  }

  std::cout << "Shader compiled: " << in.c_str() << std::endl;
  std::ofstream ostr(out.c_str(), std::ios_base::binary);
  ostr.write((char*)blob->GetBufferPointer(), blob->GetBufferSize());
}

static std::string ExtractExtension(std::string& str)
{
  INT32 dot = (INT32)str.find_last_of('.');
  if (dot == -1) return "";

  std::string ext = str.substr(dot + 1);
  str = str.substr(0, dot);
  return ext;
}