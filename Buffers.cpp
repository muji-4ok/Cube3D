#include "Buffers.h"


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

void VAO::enableAttribute(unsigned int index)
{
    glEnableVertexAttribArray(index);
}

void VAO::setAttribPointer(unsigned int index, unsigned int val_per_vertex, bool normalize,
                           unsigned int increment, unsigned int first_index)
{
    glVertexAttribPointer(index, val_per_vertex, GL_FLOAT, normalize, sizeof(float) * increment, (void*)first_index);
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
    std::cout << "VAO destroyed, ID = " << ID << '\n';
    glDeleteVertexArrays(1, &ID);
}


void VAO::generate()
{
    glGenVertexArrays(1, &ID);
}

void EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLenum EBO::getTarget() const
{
    return GL_ELEMENT_ARRAY_BUFFER;
}
