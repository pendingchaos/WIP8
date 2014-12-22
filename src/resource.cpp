#include "resource.h"

Resource::Resource(std::type_index type) : mRefCount(0), mType(type) {}

Resource::~Resource() {}
