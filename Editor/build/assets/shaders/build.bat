rem Vulkan

glslc -std=460core --target-env=vulkan1.2 -O CircleVertexShader.vert -o cache/CircleVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O CircleFragmentShader.frag -o cache/CircleFragmentShader.vulkan.spv

glslc -std=460core --target-env=vulkan1.2 -O LineVertexShader.vert -o cache/LineVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O LineFragmentShader.frag -o cache/LineFragmentShader.vulkan.spv

glslc -std=460core --target-env=vulkan1.2 -O PrimitiveVertexShader.vert -o cache/PrimitiveVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O PrimitiveFragmentShader.frag -o cache/PrimitiveFragmentShader.vulkan.spv

glslc -std=460core --target-env=vulkan1.2 -O RectVertexShader.vert -o cache/RectVertexShader.vulkan.spv
glslc -std=460core --target-env=vulkan1.2 -O RectFragmentShader.frag -o cache/RectFragmentShader.vulkan.spv


rem OpenGL

glslc -std=460core --target-env=opengl4.5 -O CircleVertexShader.vert -o cache/CircleVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O CircleFragmentShader.frag -o cache/CircleFragmentShader.opengl.spv

glslc -std=460core --target-env=opengl4.5 -O LineVertexShader.vert -o cache/LineVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O LineFragmentShader.frag -o cache/LineFragmentShader.opengl.spv

glslc -std=460core --target-env=opengl4.5 -O PrimitiveVertexShader.vert -o cache/PrimitiveVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O PrimitiveFragmentShader.frag -o cache/PrimitiveFragmentShader.opengl.spv

glslc -std=460core --target-env=opengl4.5 -O RectVertexShader.vert -o cache/RectVertexShader.opengl.spv
glslc -std=460core --target-env=opengl4.5 -O RectFragmentShader.frag -o cache/RectFragmentShader.opengl.spv