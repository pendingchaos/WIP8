#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform
{
    public:
        Transform();
        virtual ~Transform();

        inline void reset() {mMatrix = glm::mat4();}

        inline void translate(glm::vec3 translation) {mMatrix *= glm::translate(translation);}

        inline void rotate(float angle, glm::vec3 axis)
        {
            mMatrix *= glm::rotate(angle, axis);
        }

        inline void scale(glm::vec3 scale) {mMatrix *= glm::scale(scale);}

        inline void lookAt(glm::vec3 eye,
                           glm::vec3 center,
                           glm::vec3 up) {mMatrix *= glm::lookAt(eye, center, up);}

        inline void perspective(float fovy,
                                float aspect,
                                float near,
                                float far) {mMatrix *= glm::perspective(fovy, aspect, near, far);}

        inline void setMatrix(glm::mat4 matrix) {mMatrix = matrix;}
        inline void multiplyMatrix(glm::mat4 matrix) {mMatrix *= matrix;}

        glm::mat4 getMatrix() const;

        inline Transform *getParent() const {return mParent;}
        void setParent(Transform *parent);

        inline const std::vector<Transform *>& getChildren() const {return mChildren;}
        void removeChild(std::vector<Transform *>::iterator pos);
        inline void addChild(Transform *child) {mChildren.push_back(child);}
    private:
        glm::mat4 mMatrix;

        Transform *mParent;
        std::vector<Transform *> mChildren;
};

#endif // TRANSFORMCOMPONENT_H
