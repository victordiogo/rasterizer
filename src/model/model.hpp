#ifndef MODEL_HPP
#define MODEL_HPP

#include "model/mesh.hpp"
#include <vector>
#include <string>

class Model {
public:
  Model(const std::string& filepath);

private:
  std::vector<Mesh> m_meshes;
};

#endif // MODEL_HPP