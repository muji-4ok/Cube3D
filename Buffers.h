#ifndef OPENGL_LEARNING_VBO_H
#define OPENGL_LEARNING_VBO_H

#include <glad/glad.h>
#include <vector>


class BufferObject
{
public:
    BufferObject() : ID(0) {};

    BufferObject(BufferObject&& bo);
    BufferObject& operator= (BufferObject&& bo);

    BufferObject& operator= (const BufferObject&) = delete;
    BufferObject(const BufferObject&) = delete;

    void generate();
    void bind();
    template <typename T>
    void setStaticData(std::vector<T> data);

    unsigned int getID() const;

    ~BufferObject();

protected:
    template <typename T>
    void setDataSuper(const std::vector<T> &data);

    virtual GLenum getTarget() const = 0;
    unsigned int ID;
};


class VBO : public BufferObject
{
    using BufferObject::BufferObject;
    using BufferObject::operator=;

public:
    void setData(const std::vector<float> &vertices);
    static void unbind();

protected:
    GLenum getTarget() const override;
};

class EBO : public BufferObject
{
    using BufferObject::BufferObject;
    using BufferObject::operator=;

public:
    void setData(const std::vector<unsigned int> &indices);
    static void unbind();

protected:
    GLenum getTarget() const override;
};

class VAO
{
public:
    VAO() : ID(0) {};

    VAO(VAO&& vao);
    VAO& operator= (VAO&& vao);

    VAO& operator= (const VAO&) = delete;
    VAO(const VAO&) = delete;

    ~VAO();

    void generate();
    void bind();
    static void unbind();
    void setAttribPointer(unsigned int index, unsigned int components, GLenum type, GLboolean normalize,
                          unsigned int stride, const void *offset);

private:
    unsigned int ID;
};


#endif //OPENGL_LEARNING_VBO_H
