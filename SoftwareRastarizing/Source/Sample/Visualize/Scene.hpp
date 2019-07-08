#pragma once


#include "tiny_obj_loader.h"


class Scene
{
public:
    Scene();
    virtual ~Scene();

    using Vertex = Vertex_PositionColorNormal;
    struct Triangle
    {
        Vertex Vertex1;
        Vertex Vertex2;
        Vertex Vertex3;
    };

    ResultUtil Load(const std::string& path);

    void ForeachTriangles(std::function<void(const Triangle&)> func);

protected:
    tinyobj::attrib_t                m_Attribute;
    std::vector<tinyobj::shape_t>    m_Shapes;
    std::vector<tinyobj::material_t> m_Materials;
};

