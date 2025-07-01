#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glad/glad.h>

#include "stb_image/stb_image.h"

class Texture {
public:
    unsigned int ID = {};

    Texture(const char* path, const GLint internalFormat, const GLenum format, const GLenum type, const bool flip) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (flip) {
            stbi_set_flip_vertically_on_load(true);s
        }

        int width, height, nrChannels;
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void bind(const GLenum textureUnit = GL_TEXTURE0) const {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    static void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void destroy() const {
        glDeleteTextures(1, &ID);
    }
};

#endif //TEXTURE_H
