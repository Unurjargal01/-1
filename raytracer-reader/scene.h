#pragma once

#include <material.h>
#include <vector.h>
#include <object.h>
#include <light.h>

#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
// Modified Material albedo to {1, 0, 0};
// Object struct return GetNormal;
// Scene class return attributes;
// So far we have not cleared or reduced copying
class Scene {
public:
    Scene(std::vector<Object>& obj, std::vector<SphereObject>& sobj, std::vector<Light>& light,
          std::unordered_map<std::string, Material>& materials, std::vector<Vector>& vertices,
          std::vector<Vector>& vertices_normal)
        : obj_(obj),
          sobj_(sobj),
          light_(light),
          materials_(materials),
          vertices_(vertices),
          vertices_normal_(vertices_normal) {
        // Maybe after receiveing materials_ we can assign the materials;
        for (auto& elem : obj_) {
            elem.material = &materials_[elem.name];
        }
        for (auto& elem : sobj_) {
            elem.material = &materials_[elem.name];
        }
    };
    const std::vector<Object>& GetObjects() const {
        return obj_;
    };
    const std::vector<SphereObject>& GetSphereObjects() const {
        return sobj_;
    };
    const std::vector<Light>& GetLights() const {
        return light_;
    };
    const std::unordered_map<std::string, Material>& GetMaterials() const {
        return materials_;
    };

private:
    std::vector<Object> obj_;
    std::vector<SphereObject> sobj_;
    std::vector<Light> light_;
    std::unordered_map<std::string, Material> materials_;
    // Needs this
    std::vector<Vector> vertices_, vertices_normal_;
};
std::unordered_map<std::string, Material> ReadMaterials(const std::filesystem::path& path) {
    std::unordered_map<std::string, Material> materials;
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        std::string token;
        std::string name;
        Material material;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            iss >> token;
            double x, y, z;
            if (token == "newmtl") {
                iss >> name;
                if (!material.name.empty()) {
                    materials.insert({material.name, material});
                    // delete material;
                }
                material = Material();
                material.name = name;
            } else if (token == "Ka") {
                iss >> x >> y >> z;
                Vector v{x, y, z};
                material.ambient_color = v;
            } else if (token == "Kd") {
                iss >> x >> y >> z;
                Vector v{x, y, z};
                material.diffuse_color = v;
            } else if (token == "Ks") {
                iss >> x >> y >> z;
                Vector v{x, y, z};
                material.specular_color = v;
            } else if (token == "Ke") {
                iss >> x >> y >> z;
                Vector v{x, y, z};
                material.intensity = v;
            } else if (token == "Ns") {
                iss >> x;
                material.specular_exponent = x;
            } else if (token == "Ni") {
                iss >> x;
                material.refraction_index = x;
            } else if (token == "al") {
                iss >> x >> y >> z;
                Vector v{x, y, z};
                material.albedo = v;
            }
        }
        // Maybe add insert function;
        if (!material.name.empty()) {
            materials.insert({material.name, material});
            // delete material;
        }
    }
    return materials;
};

int Index(int at, int vector_size) {
    return (at >= 0) ? at - 1 : vector_size + at;
}

// For now safety, but later sobj.pushback(SphereObj)
Scene ReadScene(const std::filesystem::path& path) {
    // Required elements to initialize Scene;
    std::vector<Object> objects;
    std::vector<SphereObject> sobj;
    std::vector<Light> light;
    std::unordered_map<std::string, Material> materials;
    std::vector<Vector> vertices, vertices_normal, vertices_texture;

    std::ifstream file(path);
    if (file.is_open()) {
        std::string line, name;
        while (getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            iss >> token;
            // std::cout << line << std::endl;
            // std::cout << token << std::endl;
            double x, y, z, r, g, b;
            if (token == "mtllib") {
                std::string path_material;
                iss >> path_material;
                // std::cout<<path_material<<std::endl;
                std::filesystem::path material_path = path.parent_path() / path_material;
                std::unordered_map<std::string, Material> materials1 = ReadMaterials(material_path);
                materials.insert(materials1.begin(),
                                 materials1.end());  // why it implements two times?
                // std::cout << material_path <<std::endl;
            } else if (token == "usemtl") {
                iss >> name;  // add to element;
            } else if (token == "P") {
                iss >> x >> y >> z >> r >> g >> b;
                Vector pos{x, y, z}, intens{r, g, b};
                light.push_back(Light{pos, intens});  // safety
            } else if (token == "S") {
                iss >> x >> y >> z >> r;
                // std::cout << "Name : " << name << std::endl;
                // std::cout << x << ' ' << y << ' ' << z << ' ' << r <<std::endl;
                Vector pos{x, y, z};
                // Material m;
                // m.name = name;
                SphereObject sph;
                sph.sphere = Sphere{pos, r};
                sph.name = name;
                sobj.push_back(sph);
            } else if (token == "v") {
                iss >> x >> y >> z;
                vertices.push_back(Vector{x, y, z});
            } else if (token == "vn") {
                iss >> x >> y >> z;
                vertices_normal.push_back(Vector{x, y, z});
            } else if (token == "vt") {
                iss >> x >> y >> z;
                vertices_texture.push_back(Vector{x, y, z});
            } else if (token == "f") {
                std::vector<int> vertices_ind, normal_ind, textures_ind;
                while (iss >> token) {
                    // divided by spaces.
                    size_t pos = token.find("//");
                    if (pos != std::string::npos) {
                        int vertex_ind1 = std::stoi(token.substr(0, pos));
                        int normal_ind1 = std::stoi(token.substr(pos + 2));

                        vertices_ind.push_back(vertex_ind1);
                        normal_ind.push_back(normal_ind1);
                    } else {
                        size_t num_slashes = std::count(token.begin(), token.end(), '/');
                        if (num_slashes == 2) {
                            // f v1/vt1/vn1 v2/vt1/vn2
                            pos = token.find("/");
                            size_t pos1 = (token.substr(pos + 1)).find("/");
                            int vertex_ind1 = std::stoi(token.substr(0, pos));
                            int texture_ind1 = std::stoi(token.substr(pos + 1, pos + pos1 + 1));
                            int normal_ind1 = std::stoi(token.substr(pos + pos1 + 2));
                            vertices_ind.push_back(vertex_ind1);
                            normal_ind.push_back(normal_ind1);
                            textures_ind.push_back(texture_ind1);
                        } else if (num_slashes == 1) {
                            // f v1/vt1 v2/vt2
                            pos = token.find("/");
                            int vertex_ind1 = std::stoi(token.substr(0, pos));
                            int texture_ind1 = std::stoi(token.substr(pos + 1));
                            vertices_ind.push_back(vertex_ind1);
                            textures_ind.push_back(texture_ind1);
                        } else if (num_slashes == 0) {
                            // f v1 v2 v3
                            int vertex_ind1 = std::stoi(token);
                            vertices_ind.push_back(vertex_ind1);
                        }
                    }
                }
                Vector p0 = vertices[Index(vertices_ind[0], vertices.size())];
                for (size_t i = 1; i < vertices_ind.size() - 1; ++i) {
                    Object obj;
                    // or we can directly adds material, but I wonder what if the name of mtl
                    // appaers later;
                    // Material m;
                    // m.name = name;
                    // obj.material = &materials[name];
                    // obj.material = &m;
                    obj.name = name;
                    Vector pi = vertices[Index(vertices_ind[i], vertices.size())];
                    Vector pi1 = vertices[Index(vertices_ind[i + 1], vertices.size())];
                    obj.polygon = Triangle{p0, pi, pi1};
                    if (!normal_ind.empty()) {
                        Vector p0_n = vertices_normal[Index(normal_ind[0], vertices_normal.size())];
                        Vector pi_n = vertices_normal[Index(normal_ind[i], vertices_normal.size())];
                        Vector pi1_n =
                            vertices_normal[Index(normal_ind[i + 1], vertices_normal.size())];
                        obj.norm_polygon = Triangle{p0_n, pi_n, pi1_n};
                    }
                    objects.push_back(obj);
                }
            }
        }
    }

    return Scene{objects, sobj, light, materials, vertices, vertices_normal};
};