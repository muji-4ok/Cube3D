#pragma once
#include <glad/glad.h>


class Texture
{
public:
    Texture() : ID(0) {}
    ~Texture();

    Texture(Texture&& t);
    Texture& operator=(Texture&& t);

    Texture(const Texture&) = delete;
    Texture& operator= (const Texture&) = delete;

    unsigned int getID() const;

    void generate();
    void bind();
    static void unbind();
    void texImage2D(int internalFormat, int width, int height, GLenum format, GLenum type, const void* data);

private:
    unsigned int ID;

};

