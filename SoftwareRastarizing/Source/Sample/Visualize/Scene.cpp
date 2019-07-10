
Scene::Scene()
{

}

Scene::~Scene()
{

}


ResultUtil Scene::Load(const std::string& path)
{
    std::string warning;
    std::string error;
    if (!tinyobj::LoadObj(&m_Attribute, &m_Shapes, &m_Materials, &warning, &error, path.c_str()))
    {
        std::string message;
        if (warning != "")
        {
            message += "Warning : " + warning + "\n";
        }
        if (error != "")
        {
            message += "Error : " + error + "\n";
        }
        return ResultUtil(false, message);
    }
    return ResultUtil(true, "Scceeded.");
}


void Scene::ForeachTriangles(std::function<void(const Triangle&)> func) const
{
    for (const auto& shape : m_Shapes)
    {
        for (int face_index = 0; face_index < (int)shape.mesh.num_face_vertices.size(); face_index++)
        {
            const auto v = [&](int index) -> Vertex {
                const auto i = shape.mesh.indices[index];
                return {
                    glm::vec4(
                        m_Attribute.vertices[3 * i.vertex_index + 0],
                        m_Attribute.vertices[3 * i.vertex_index + 1],
                        m_Attribute.vertices[3 * i.vertex_index + 2],
                        1.0f
                    ),
                    glm::vec4(
                        m_Attribute.colors[3 * i.vertex_index + 0],
                        m_Attribute.colors[3 * i.vertex_index + 1],
                        m_Attribute.colors[3 * i.vertex_index + 2],
                        1.0f
                    ),
                    glm::normalize(
                        glm::vec4(
                            m_Attribute.normals[3 * i.normal_index + 0],
                            m_Attribute.normals[3 * i.normal_index + 1],
                            m_Attribute.normals[3 * i.normal_index + 2],
                            1.0f
                        )
                    )
                    //glm::vec4(
                    //    0.0f,
                    //    0.0f,
                    //    0.0f,
                    //    1.0f
                    //)
                };
            };
            func({ v(3 * face_index + 0), v(3 * face_index + 1), v(3 * face_index + 2) });
        }
    }
}

