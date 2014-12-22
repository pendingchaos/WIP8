#include "texture.h"

Texture::Texture(Type type) : Resource(typeid(Texture)),
                              mType(type) {}

Texture::~Texture() {}
