#include "Buffers.h"


void VBO::setData(const std::vector<float> &vertices)
{
    setDataSuper(vertices);
}

void VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLenum VBO::getTarget() const
{
    return GL_ARRAY_BUFFER;
}

void VAO::bind()
{
    glBindVertexArray(ID);
}

void VAO::unbind()
{
    glBindVertexArray(0);
}

void VAO::setAttribPointer(unsigned int index, unsigned int components, GLenum type, GLboolean normalize,
                           unsigned int stride, const void *offset)
{
    glVertexAttribPointer(index, components, type, normalize, stride, offset);
    glEnableVertexAttribArray(index);
}

VAO::VAO(VAO && vao)
{
    ID = vao.ID;
    vao.ID = 0;
}

VAO & VAO::operator=(VAO && vao)
{
    if (this == &vao)
        return *this;

    ID = vao.ID;
    vao.ID = 0;

    return *this;
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &ID);
}

void VAO::generate()
{
    glGenVertexArrays(1, &ID);
}

BufferObject::BufferObject(BufferObject && bo)
{
    ID = bo.ID;
    bo.ID = 0;
}

BufferObject & BufferObject::operator=(BufferObject && bo)
{
    if (this == &bo)
        return *this;

    ID = bo.ID;
    bo.ID = 0;

    return *this;
}

void BufferObject::generate()
{
    glGenBuffers(1, &ID);
}

void BufferObject::bind()
{
    glBindBuffer(getTarget(), ID);
}

unsigned int BufferObject::getID() const
{
    return ID;
}

BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &ID);
}

template<typename T>
void BufferObject::setStaticData(std::vector<T> data)
{
    glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW);
}

template<typename T>
void BufferObject::setDataSuper(const std::vector<T> &data)
{
    //glBufferData(target, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
}

void EBO::setData(const std::vector<unsigned int> &indices)
{
    setDataSuper(indices);
}

void EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLenum EBO::getTarget() const
{
    return GL_ELEMENT_ARRAY_BUFFER;
}
