#include "Texture.h"


Texture::~Texture()
{
    glDeleteTextures(1, &ID);
}

Texture::Texture(Texture && t)
{
    ID = t.ID;
    t.ID = 0;
}

Texture & Texture::operator=(Texture && t)
{
    if (&t != this)
    {
        ID = t.ID;
        t.ID = 0;
    }

    return *this;
}

unsigned int Texture::getID() const
{
    return ID;
}

void Texture::generate()
{
    glGenTextures(1, &ID);
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texImage2D(int internalFormat, int width, int height, GLenum format, GLenum type, const void * data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
}
