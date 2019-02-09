#pragma once
#include <glad/glad.h>
#include <vector>
#include <iostream>


template <typename T>
class BufferObject
{
public:
    BufferObject() : ID(0) {};

    BufferObject(BufferObject<T>&& bo);
    BufferObject<T>& operator= (BufferObject<T>&& bo);
    BufferObject(const BufferObject<T>&) = delete;
    BufferObject<T>& operator= (const BufferObject<T>&) = delete;

    ~BufferObject();

    void generate();
    void bind();
    void setStaticData(const std::vector<T> &data);
    unsigned int getID() const;

protected:
    virtual GLenum getTarget() const = 0;
    unsigned int ID;

};


class VBO : public BufferObject<float>
{
    using BufferObject<float>::BufferObject;
    using BufferObject<float>::operator=;

public:
    static void unbind();

protected:
    GLenum getTarget() const override;
};


class EBO : public BufferObject<unsigned int>
{
    using BufferObject<unsigned int>::BufferObject;
    using BufferObject<unsigned int>::operator=;

public:
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
    void enableAttribute(unsigned int index);
    void setAttribPointer(unsigned int index, unsigned int val_per_vertex, bool normalize,
                          unsigned int increment, unsigned int first_index);

private:
    unsigned int ID;
};

template<typename T>
inline BufferObject<T>::BufferObject(BufferObject<T>&& bo)
{
    ID = bo.ID;
    bo.ID = 0;
}

template<typename T>
inline BufferObject<T>& BufferObject<T>::operator=(BufferObject<T>&& bo)
{
    if (&bo != this)
    {
        this.ID = bo.ID;
        bo.ID = 0;
    }

    return *this;
}

template<typename T>
inline BufferObject<T>::~BufferObject()
{
    // std::cerr << "BufferObject destroyed, ID = " << ID << '\n';
    glDeleteBuffers(1, &ID);
}

template<typename T>
inline void BufferObject<T>::generate()
{
    glGenBuffers(1, &ID);
}

template<typename T>
inline void BufferObject<T>::bind()
{
    glBindBuffer(getTarget(), ID);
}

template<typename T>
inline void BufferObject<T>::setStaticData(const std::vector<T>& data)
{
    glBufferData(getTarget(), sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW);
}

template<typename T>
inline unsigned int BufferObject<T>::getID() const
{
    return ID;
}
