#include <yaml-cpp/yaml_ex.hpp>

namespace YAML
{

    Node convert<glm::vec2>::encode(const glm::vec2& vec) {
        Node node;
        node.push_back(vec.x);
        node.push_back(vec.y);
        return node;
    }
    bool convert<glm::vec2>::decode(const Node& node, glm::vec2& vec) {
        // Check Container Vector Data
        if (!node.IsSequence() || node.size() != 2)
        {
            return false;
        }

        vec.x = node[0].as<float>();
        vec.y = node[1].as<float>();

        return true;
    }

    Node convert<glm::vec3>::encode(const glm::vec3& vec)
    {
        Node node;
        node.push_back(vec.x);
        node.push_back(vec.y);
        node.push_back(vec.z);
        return node;
    }

    bool convert<glm::vec3>::decode(const Node& node, glm::vec3& vec)
    {
        // Check Container Vector Data
        if (!node.IsSequence() || node.size() != 3)
        {
            return false;
        }

        vec.x = node[0].as<float>();
        vec.y = node[1].as<float>();
        vec.z = node[2].as<float>();

        return true;
    }

    Node convert<glm::vec4>::encode(const glm::vec4& vec)
    {
        Node node;
        node.push_back(vec.r);
        node.push_back(vec.g);
        node.push_back(vec.b);
        node.push_back(vec.a);
        return node;
    }

    bool convert<glm::vec4>::decode(const Node& node, glm::vec4& vec)
    {
        // Check Container Vector Data
        if (!node.IsSequence() || node.size() != 4)
        {
            return false;
        }

        vec.r = node[0].as<float>();
        vec.g = node[1].as<float>();
        vec.b = node[2].as<float>();
        vec.a = node[3].as<float>();

        return true;
    }

    Emitter& operator<<(YAML::Emitter& writer, const glm::vec2& vec){
        writer << YAML::Flow;
        writer << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
        return writer;
    }

    Emitter &operator<<(YAML::Emitter& writer, const glm::vec3& vec)
    {
        writer << YAML::Flow;
        writer << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return writer;
    }

    Emitter &operator<<(YAML::Emitter& writer, const glm::vec4& vec)
    {
        writer << YAML::Flow;
        writer << YAML::BeginSeq << vec.r << vec.g << vec.b << vec.a << YAML::EndSeq;
        return writer;
    }

}