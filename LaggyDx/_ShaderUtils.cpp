#include "stdafx.h"
#include "_ShaderUtils.h"


namespace Dx
{
  void compileShader(
    const fs::path& i_fileName,
    ID3D10Blob** i_buffer,
    const char* i_entryPoint,
    const char* i_target)
  {
    ID3D10Blob* errorBuffer = nullptr;

    HRESULT hres = D3DCompileFromFile(
      i_fileName.wstring().c_str(), // pFileName
      NULL, // *pDefines
      NULL, // *pInclude
      i_entryPoint, // pEntryPoint
      i_target, // pTarget
      D3D10_SHADER_ENABLE_STRICTNESS, // Flags1
      0, // Flags2
      i_buffer, // **ppCode
      &errorBuffer // **ppErrorMsgs
    );

    if (FAILED(hres))
    {
      if (errorBuffer)
      {
        // Get a pointer to the error message text buffer.
        [[maybe_unused]] const char* const errorText = (const char*)(errorBuffer->GetBufferPointer());
        errorBuffer->Release();

        // Inspect 'errorText' for details
        CONTRACT_ASSERT(false);
      }

      // Most likely is that the given file is not found
      CONTRACT_ASSERT(false);
    }
  }

} // ns Dx
