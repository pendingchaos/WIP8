#include "featureset.h"

FeatureSet::FeatureSet() : mGeometryShaderRequired(false),
                           mTessellationRequired(false),

                           mGLSL110Supported(false),
                           mGLSL120Supported(false),
                           mGLSL130Supported(false),
                           mGLSL140Supported(false),
                           mGLSL150Supported(false),
                           mGLSL330Supported(false),
                           mGLSL400Supported(false),
                           mGLSL410Supported(false),
                           mGLSL420Supported(false),
                           mGLSL430Supported(false),
                           mGLSL440Supported(false),

                           mNvidiaCardSupported(true),
                           mAMDCardSupported(true),
                           mIntelCardSupported(true),
                           mSoftwareRasterizerSupported(true),

                           mMesaDriverSupported(true),
                           mNvidiaDriverSupported(true),
                           mAMDDriverSupported(true),
                           mIntelDriverSupported(true) {}
