#define LOG_TAG "VideoPowerHint"

#include <inttypes.h>
#include <utils/Trace.h>

#include <gui/Surface.h>

#include <media/stagefright/ACodec.h>

#include <binder/MemoryDealer.h>

#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AUtils.h>

#include <media/stagefright/BufferProducerWrapper.h>
#include <media/stagefright/MediaCodec.h>
#include <media/stagefright/MediaCodecList.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/PersistentSurface.h>
#include <media/stagefright/SurfaceUtils.h>
#include <media/hardware/HardwareAPI.h>
#include <media/OMXBuffer.h>
#include <media/omx/1.0/WOmxNode.h>

#include <hidlmemory/mapping.h>

#include <OMX_AudioExt.h>
#include <OMX_VideoExt.h>
#include <OMX_Component.h>
#include <OMX_IndexExt.h>
#include <OMX_AsString.h>

#include "include/ACodecBufferChannel.h"
#include "include/DataConverter.h"
#include "include/SecureBuffer.h"
#include "include/SharedMemoryBuffer.h"
#include "include/VideoPowerHint.h"

#include <android/hidl/allocator/1.0/IAllocator.h>
#include <android/hidl/memory/1.0/IMemory.h>
#include <media/stagefright/foundation/avc_utils.h>

#include "include/VideoPowerHint.h"


namespace android {


int VideoPowerHintCPU::mAcquiredCount = 0;
sp<IPowerManager> VideoPowerHintCPU::mPowerManager = NULL;
Mutex VideoPowerHintCPU::lock;

void VideoPowerHintCPU::acquire()
{

    if (mPowerManager == NULL) {
       const String16 serviceName("power");
       sp<IBinder> bs = defaultServiceManager()->checkService(serviceName);
       if (bs == NULL) {
           ALOGI("power service failed");
           return;
       }
       mPowerManager = interface_cast<IPowerManager>(bs);
    }

    if (mPowerManager != NULL) {
        mPowerManager->powerHint(mPowerHintID, 1);
        ALOGD("VideoPowerHintCPU video play with acquire");
    }
}

void VideoPowerHintCPU::release()
{
    if (mPowerManager != NULL) {
        mPowerManager->powerHint(mPowerHintID, 0);
        mPowerManager = NULL;
        ALOGD("VideoPowerHintCPU exit, release mPowerManager");
    }
}

}
