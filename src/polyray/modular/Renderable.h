#ifndef RENDERABLE_H_INCLUDED
#define RENDERABLE_H_INCLUDED

class Renderable {
public:
    virtual ~Renderable() = default;
    virtual void render() = 0;
};

#endif
