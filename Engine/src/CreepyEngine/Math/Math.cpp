#include <CreepyEngine/Math/Math.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Creepy {
    
    bool Math::DecomposeTransform(const glm::mat4& transform, glm::vec3& outPosition, glm::vec3& outRotation, glm::vec3& outScale) noexcept {
        glm::mat4 localMatrix{transform};
        
        if(glm::epsilonEqual(localMatrix[3][3], 0.0f, glm::epsilon<float>())){
            return false;
        }

        if(
			glm::epsilonNotEqual(localMatrix[0][3], 0.0f, glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[1][3], 0.0f, glm::epsilon<float>()) ||
			glm::epsilonNotEqual(localMatrix[2][3], 0.0f, glm::epsilon<float>()))
		{
            // Clear perspective
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0.0f;
            localMatrix[3][3] = 1.0f;

        }

        outPosition = glm::vec3{localMatrix[3]};
        localMatrix[3] = glm::vec4{0.0f, 0.0f, 0.0f, localMatrix[3].w};

        glm::vec3 row[3];

        for(int i{}; i < 3; ++i){
            for(int j{}; j < 3; ++j){
                row[i][j] = localMatrix[i][j];
            }
        }
        
        outScale.x = glm::length(row[0]);
        row[0] = glm::detail::scale(row[0], 1.0f);

        outScale.y = glm::length(row[1]);
        row[1] = glm::detail::scale(row[1], 1.0f);

        outScale.z = glm::length(row[2]);
        row[2] = glm::detail::scale(row[2], 1.0f);

        outRotation.y = std::asin(-row[0][2]);

        if(std::is_neq(std::cos(outRotation.y) <=> 0.0f)){
            outRotation.x = std::atan2(row[1][2], row[2][2]);
            outRotation.z = std::atan2(row[0][1], row[0][0]);
        } else {
            outRotation.x = std::atan2(-row[2][0], row[1][1]);
            outRotation.z = 0.0f;
        }

        return true;

    }
}