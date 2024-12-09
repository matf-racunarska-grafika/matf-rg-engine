//
// Created by spaske on 12/3/24.
//

#ifndef OPENGL_HPP
#define OPENGL_HPP
#include <cstdint>
#include <filesystem>
#include <engine/resources/Shader.hpp>

// TODO(mspasic): Add GL_ASSERT(...)

namespace engine::resources {
    class Skybox;
}

namespace engine::graphics {
    /**
    * @class OpenGL
    * @brief This class serves as the OpenGL interface for your app, since the engine doesn't directly link OpenGL to the app executable.
    *
    * Any OpenGL additional direct OpenGL calls you need should be added here.
    */
    class OpenGL {
    public:
        using ShaderProgramId = uint32_t;

        /**
        * @brief Converts @ref resources::ShaderType to the OpenGL shader type enum.
        * @returns GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
        */
        static int32_t shader_type_to_opengl_type(resources::ShaderType type);

        /**
        * @brief Loads the texture from `path` into the OpenGL context.
        *
        * @param path path to a texture file.
        * @param flip_uvs flip_uvs on load.
        * @returns OpenGL id of a texture object.
        */
        static uint32_t generate_texture(const std::filesystem::path &path, bool flip_uvs);

        /**
        * @brief Get texture format for a `number_of_channels`.
        * @param number_of_channels that the texture has.
        * @returns GL_RED, GL_RGB, GL_RGBA for the number_of_channels=[1,3,4] respectively.
        */
        static int32_t texture_format(int32_t number_of_channels);

        /**
        * @brief Initializes the cube Vertex Array Object used for skybox drawing. Caches the vao result.
        * @returns VAO of the cube used for skybox drawing.
        */
        static uint32_t init_skybox_cube();

        /**
        * @brief Check if the shader with the `shader_id` compiled successfully.
        * @returns true if the shader compilation succeeded, false otherwise.
        */
        static bool shader_compiled_successfully(uint32_t shader_id);

        /**
        * @brief Compiles the shader from source.
        * @param shader_source source code for the shader
        * @param shader_type the type of the shader to create1
        * @returns OpenGL context object of the shader.
        */
        static uint32_t compile_shader(const std::string &shader_source,
                                       resources::ShaderType shader_type);

        /**
        * @brief Loads the skybox textures from the `path`.
        * Make sure that images are named: front.jpg, back.jpg, up.jpg, down.jpg, left.jpg, down.jpg.
        * They can be of the other extension as well, but the function will assign each texture to the appropriate
        * side of the cubemap based on the texture file name.
        * @param path directory in which cubemap textures are located.
        * @param flip_uvs wheater to flip_uvs on texture loading.
        * @returns OpenGL id to the cubemap texture
        */
        static uint32_t load_skybox_textures(const std::filesystem::path &path, bool flip_uvs = false);

        /**
        * @brief Enables depth testing.
        */
        static void enable_depth_testing();

        /**
        * @brief Disables depth testing.
        */
        static void disable_depth_testing();

        /**
        * @brief Clears GL_DEPTH_BUFFER_BIT, GL_COLOR_BUFFER_BIT, and GL_STENCIL_BUFFER_BIT.
        */
        static void clear_buffers();

        /**
        * @brief Retrieve the shader compilation error log message.
        * @param shader_id Shader id for which the compilation failed.
        * @returns shader compilation error message.
        */
        static std::string get_compilation_error_message(uint32_t shader_id);
    };
}
#endif //OPENGL_HPP