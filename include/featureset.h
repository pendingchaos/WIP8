#ifndef FEATURESET_H
#define FEATURESET_H

//TODO: Find a better name for this.
class FeatureSet
{
    FeatureSet();

    bool mGeometryShaderRequired:1;
    bool mTessellationRequired:1;

    bool mGLSL110Supported:1;
    bool mGLSL120Supported:1;
    bool mGLSL130Supported:1;
    bool mGLSL140Supported:1;
    bool mGLSL150Supported:1;
    bool mGLSL330Supported:1;
    bool mGLSL400Supported:1;
    bool mGLSL410Supported:1;
    bool mGLSL420Supported:1;
    bool mGLSL430Supported:1;
    bool mGLSL440Supported:1;

    bool mNvidiaCardSupported:1;
    bool mAMDCardSupported:1;
    bool mIntelCardSupported:1;
    bool mSoftwareRasterizerSupported:1;

    bool mMesaDriverSupported:1;
    bool mNvidiaDriverSupported:1;
    bool mAMDDriverSupported:1;
    bool mIntelDriverSupported:1;
};

#endif // FEATURESET_H
