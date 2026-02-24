#ifndef MODEL_HPP
#define MODEL_HPP

#include "model/texture.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <fstream>
#include <filesystem>
#include <sstream>

struct Material {
  Vec3f ambient{0.1f};
  Vec3f diffuse{1.0f};
  Vec3f specular{1.0f};
  float shininess{32.0f};
  std::optional<std::string> diffuse_texture;
};

struct Vertex {
  Vec3f position;
  Vec3f normal;
  Vec2f uv;
};

struct Mesh {
  Material material;
  std::vector<Vertex> vertices;
};

struct Index {
  std::uint32_t position;
  std::optional<std::uint32_t> uv;
  std::optional<std::uint32_t> normal;
};

using Face = std::vector<Index>;

/// @param line the face line without prefix "f"
inline auto parse_face(const std::string& line) -> Face {
  auto sstream = std::istringstream{line};
  auto face = Face{};
  
  auto token = std::string{};
  while (sstream >> token) {
    auto first_slash = token.find('/');
    auto second_slash = token.find('/', first_slash + 1);
    auto index = Index{};

    if (first_slash == std::string::npos) {
      // only position index
      auto pos = std::stol(token);
      if (pos < 0) {
        throw std::runtime_error{"Negative indices are not supported"};
      }
      index.position = (std::uint32_t)pos - 1;
    }
    else if (second_slash == std::string::npos) {
      // position and uv indices
      auto pos = std::stol(token.substr(0, first_slash));
      auto uv = std::stol(token.substr(first_slash + 1));
      if (pos < 0 || uv < 0) {
        throw std::runtime_error{"Negative indices are not supported"};
      }

      index.position = (std::uint32_t)pos - 1;
      index.uv = (std::uint32_t)uv - 1;
    }
    else {
      // position, uv, and normal indices
      auto pos = std::stol(token.substr(0, first_slash));
      if (pos < 0)
        throw std::runtime_error{"Negative indices are not supported"};
      
      index.position = (std::uint32_t)pos - 1;
      if (second_slash > first_slash + 1) {
        auto uv = std::stol(token.substr(first_slash + 1, second_slash - first_slash - 1));
        if (uv < 0)
          throw std::runtime_error{"Negative indices are not supported"};
        index.uv = (std::uint32_t)uv - 1;
      }
      if (second_slash < token.size() - 1) {
        auto nor = std::stoul(token.substr(second_slash + 1));
        if (nor < 0)
          throw std::runtime_error{"Negative indices are not supported"};
        index.normal = (std::uint32_t)nor - 1;
      }
    }

    face.push_back(index);
  }

  if (face.size() < 3 || face.size() > 4)
    throw std::runtime_error{"Only triangular and quadrilateral faces are supported"};

  return face;
}

using material_lib = std::unordered_map<std::string, Material>;

inline auto parse_mtl(const std::string& filepath) -> material_lib {
  auto file = std::ifstream{filepath};
  if (!file)
    throw std::runtime_error{"Could not open material file: " + filepath};

  auto materials = material_lib{};
  auto current_material = std::string{};

  while (file) {
    auto token = std::string{};
    file >> token;

    if (token == "newmtl") {
      file >> current_material;
      materials.emplace(current_material, Material{});
    }
    else if (token == "Ka") {
      file >> materials.at(current_material).ambient.x
          >> materials.at(current_material).ambient.y
          >> materials.at(current_material).ambient.z;
    }
    else if (token == "Kd") {
      file >> materials.at(current_material).diffuse.x
          >> materials.at(current_material).diffuse.y
          >> materials.at(current_material).diffuse.z;
    }
    else if (token == "Ks") {
      file >> materials.at(current_material).specular.x
          >> materials.at(current_material).specular.y
          >> materials.at(current_material).specular.z;
    }
    else if (token == "Ns") {
      file >> materials.at(current_material).shininess;
    }
    else if (token == "map_Kd") {
      auto texture_path = std::string{};
      file >> texture_path;
      materials.at(current_material).diffuse_texture = texture_path;
    }
  }

  return materials;
}

class Model {
public:
  Model(const std::string& filepath) {
    auto file = std::ifstream{filepath};
    if (!file)
      throw std::runtime_error{"Could not open model file: " + filepath};

    auto dir = std::filesystem::path{filepath}.parent_path();

    auto positions = std::vector<Vec3f>{};
    auto normals = std::vector<Vec3f>{};
    auto uvs = std::vector<Vec2f>{};

    auto materials = std::optional<material_lib>{};

    while (file) {
      auto token = std::string{};
      file >> token;
      if (token == "v") {
        auto position = Vec3f{};
        file >> position.x >> position.y >> position.z;
        positions.push_back(position);
      } 
      else if (token == "vn") {
        auto normal = Vec3f{};
        file >> normal.x >> normal.y >> normal.z;
        normals.push_back(normal);
      }
      else if (token == "vt") {
        auto uv = Vec2f{};
        file >> uv.x >> uv.y;
        uvs.push_back(uv);
      }
      else if (token == "f") {
        if (m_meshes.empty()) {
          m_meshes.emplace_back();
        }

        std::getline(file, token);
        auto face = parse_face(token);

        auto normal = Vec3f{};
        if (!face[0].normal) {
          auto ab = positions[face[1].position] - positions[face[0].position];
          auto ac = positions[face[2].position] - positions[face[0].position];
          normal = normalize(cross(ab, ac));
        }

        auto v0 = Vertex{positions[face[0].position], face[0].normal ? normals[*face[0].normal] : normal, face[0].uv ? uvs[*face[0].uv] : Vec2f{}};
        auto v1 = Vertex{positions[face[1].position], face[1].normal ? normals[*face[1].normal] : normal, face[1].uv ? uvs[*face[1].uv] : Vec2f{}};
        auto v2 = Vertex{positions[face[2].position], face[2].normal ? normals[*face[2].normal] : normal, face[2].uv ? uvs[*face[2].uv] : Vec2f{}};

        m_meshes.back().vertices.push_back(v0);
        m_meshes.back().vertices.push_back(v1);
        m_meshes.back().vertices.push_back(v2);

        if (face.size() == 4) {
          auto v3 = Vertex{positions[face[3].position], face[3].normal ? normals[*face[3].normal] : normal, face[3].uv ? uvs[*face[3].uv] : Vec2f{}};
          m_meshes.back().vertices.push_back(v2);
          m_meshes.back().vertices.push_back(v3);
          m_meshes.back().vertices.push_back(v0);
        }
      }
      else if (token == "usemtl" && materials) {
        auto material_name = std::string{};
        file >> material_name;
        if (!materials->contains(material_name))
          throw std::runtime_error{"material not found: " + material_name};

        m_meshes.emplace_back();
        m_meshes.back().material = materials->at(material_name);
      }
      else if (token == "mtllib") {
        auto mtl_filename = std::string{};
        file >> mtl_filename;
        auto mtl_filepath = (dir / mtl_filename).string();
        materials = parse_mtl(mtl_filepath);

        // Load textures for materials
        for (auto& [_, material] : *materials) {
          if (!material.diffuse_texture) continue;

          if (!m_textures.contains(*material.diffuse_texture)) {
            auto texture_filepath = (dir / *material.diffuse_texture).string();
            m_textures.emplace(*material.diffuse_texture, Texture{texture_filepath});
          }
        }
      }
    }
  }

  auto meshes() const -> const std::vector<Mesh>& {
    return m_meshes;
  }

  auto texture(const std::string& name) const -> const Texture& {
    if (!m_textures.contains(name))
      throw std::runtime_error{"Texture not found: " + name};
    return m_textures.at(name);
  }

private:
  std::vector<Mesh> m_meshes;
  std::unordered_map<std::string, Texture> m_textures;
};

#endif // MODEL_HPP