#pragma once

#include "decoders/monounpacked.h"
extern "C" {
  #include <arv.h>
}

namespace QArv
{

class Mono8SignedFormat : public QObject, public QArvPixelFormat {
    Q_OBJECT
    Q_INTERFACES(QArvPixelFormat)
    Q_PLUGIN_METADATA(IID "si.ad-vega.qarv.Mono8SignedFormat")
public:
    ArvPixelFormat pixelFormat() { return ARV_PIXEL_FORMAT_MONO_8_SIGNED; }
    QArvDecoder* makeDecoder(QSize size) {
        return new MonoUnpackedDecoder<int8_t, 8,
                                       ARV_PIXEL_FORMAT_MONO_8_SIGNED>(size);
    }
};

}