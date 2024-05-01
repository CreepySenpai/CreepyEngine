rem Vulkan

glslc -std=460core --target-env=vulkan1.2 -O CircleVertexShader.vert -o cache/vulkan/CircleVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O CircleFragmentShader.frag -o cache/vulkan/CircleFragmentShader.vulkan.spv

glslc -std=460core --target-env=vulkan1.2 -O LineVertexShader.vert -o cache/vulkan/LineVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O LineFragmentShader.frag -o cache/vulkan/LineFragmentShader.vulkan.spv

glslc -std=460core --target-env=vulkan1.2 -O PrimitiveVertexShader.vert -o cache/vulkan/PrimitiveVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O PrimitiveFragmentShader.frag -o cache/vulkan/PrimitiveFragmentShader.vulkan.spv

glslc -std=460core --target-env=vulkan1.2 -O RectVertexShader.vert -o cache/vulkan/RectVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O RectFragmentShader.frag -o cache/vulkan/RectFragmentShader.vulkan.spv


rem OpenGL

glslc -std=460core --target-env=opengl4.5 -O CircleVertexShader.vert -o cache/opengl/CircleVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O CircleFragmentShader.frag -o cache/opengl/CircleFragmentShader.opengl.spv

glslc -std=460core --target-env=opengl4.5 -O LineVertexShader.vert -o cache/opengl/LineVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O LineFragmentShader.frag -o cache/opengl/LineFragmentShader.opengl.spv

glslc -std=460core --target-env=opengl4.5 -O PrimitiveVertexShader.vert -o cache/opengl/PrimitiveVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O PrimitiveFragmentShader.frag -o cache/opengl/PrimitiveFragmentShader.opengl.spv

glslc -std=460core --target-env=opengl4.5 -O RectVertexShader.vert -o cache/opengl/RectVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O RectFragmentShader.frag -o cache/opengl/RectFragmentShader.opengl.spv