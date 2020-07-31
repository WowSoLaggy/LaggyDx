#include "stdafx.h"
#include "MeshLoader.h"

#include <LaggySdk/StringUtils.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  namespace
  {
    std::vector<Material> loadMaterials(const std::string& i_filename)
    {
      std::ifstream file(i_filename);
      if (!file)
        return { };

      std::vector<Material> materials;

      std::string line;
      Material material;
      while (std::getline(file, line))
      {
        auto tokens = Sdk::splitString(line, '#', true);
        if (tokens.empty())
          continue;
        line = tokens.front();
        if (line.empty())
          continue;

        tokens = Sdk::splitString(line, ' ', true);
        if (tokens[0] == "newmtl")
        {
          if (material.name != tokens[1])
          {
            if (!material.name.empty())
              materials.push_back(material);
            material = Material();
            material.name = tokens[1];
          }
        }
        else if (tokens[0] == "Ka")
        {
          material.ambientColor = {
            (float)std::atof(tokens[1].c_str()),
            (float)std::atof(tokens[2].c_str()),
            (float)std::atof(tokens[3].c_str()),
            1.0f };
        }
        else if (tokens[0] == "Kd")
        {
          material.diffuseColor = {
            (float)std::atof(tokens[1].c_str()),
            (float)std::atof(tokens[2].c_str()),
            (float)std::atof(tokens[3].c_str()),
            1.0f };
        }
      }

      if (!material.name.empty())
        materials.push_back(material);

      return materials;
    }
  } // anonymous NS


  void MeshLoader::loadInfoFromObjFile(
    const std::string& i_modelPath,
    std::vector<VertexTypePosTexNorm>& o_vertices, std::vector<int>& o_indices,
    MaterialSequence& o_matSequence)
  {
    o_vertices.clear();
    o_indices.clear();
    o_matSequence.clear();

    std::ifstream file(i_modelPath);
    if (!file)
      return;

    std::vector<Sdk::Vector3F> positions;
    std::vector<Sdk::Vector2F> texCoords;
    std::vector<Sdk::Vector3F> normals;

    std::map<std::tuple<int, int, int>, int> trioMap;
    int nextIndex = 0;

    std::string mtllibFileName;
    std::vector<std::pair<int, std::string>> materialNamesMap;

    std::string line;
    while (std::getline(file, line))
    {
      auto tokens = Sdk::splitString(line, '#', true);
      if (tokens.empty())
        continue;
      line = tokens.front();
      if (line.empty())
        continue;

      tokens = Sdk::splitString(line, ' ', true);
      if (tokens[0] == "v")
      {
        positions.push_back({
          (float)std::atof(tokens[1].c_str()),
          (float)std::atof(tokens[2].c_str()),
          (float)std::atof(tokens[3].c_str()) });
      }
      else if (tokens[0] == "vt")
      {
        texCoords.push_back({
          (float)std::atof(tokens[1].c_str()),
          1.0f - (float)std::atof(tokens[2].c_str()) });
      }
      else if (tokens[0] == "vn")
      {
        normals.push_back({
          (float)std::atof(tokens[1].c_str()),
          (float)std::atof(tokens[2].c_str()),
          (float)std::atof(tokens[3].c_str()) });
      }
      else if (tokens[0] == "f")
      {
        for (int trioNum : { 1, 2, 3 })
        {
          auto indicesTrio = Sdk::splitString(tokens[trioNum], '/', true);
          int posIndex = std::atoi(indicesTrio[0].c_str()) - 1;
          int texCoordIndex = !indicesTrio[1].empty() ? std::atoi(indicesTrio[1].c_str()) - 1 : -1;
          int normIndex = std::atoi(indicesTrio[2].c_str()) - 1;
          auto trio = std::tuple<int, int, int>(posIndex, texCoordIndex, normIndex);

          auto it = trioMap.find(trio);
          if (it == trioMap.end())
          {
            Sdk::Vector2F uv = (texCoordIndex == -1) ? Sdk::Vector2F{ 0.0f, 0.0f } : texCoords[texCoordIndex];
            o_vertices.push_back({ positions[posIndex], uv, normals[normIndex] });
            trioMap[trio] = nextIndex;
            o_indices.push_back(nextIndex);
            ++nextIndex;
          }
          else
            o_indices.push_back(it->second);
        }
      }
      else if (tokens[0] == "usemtl")
        materialNamesMap.push_back({ (int)o_indices.size(), tokens[1] });
      else if (tokens[0] == "mtllib")
        mtllibFileName = tokens[1];

    } // while (std::getline(file, line))

    mtllibFileName = fs::path(i_modelPath).parent_path().string() + "\\" + mtllibFileName;
    auto materials = loadMaterials(mtllibFileName);

    for (auto it = materialNamesMap.begin(); it != materialNamesMap.end(); ++it)
    {
      auto itMat = std::find_if(materials.begin(), materials.end(),
        [&](const Material& i_material) { return i_material.name == it->second; });
      Material material = itMat != materials.end() ? *itMat : Material::getDefault();

      if (std::next(it) != materialNamesMap.end())
      {
        // Not the last element
        int count = std::next(it)->first - it->first;
        o_matSequence.add({ material, it->first, count });
      }
      else
      {
        // The last element
        int count = (int)o_indices.size() - it->first;
        o_matSequence.add({ material, it->first, count });
      }
    }
  }

} // ns Dx
