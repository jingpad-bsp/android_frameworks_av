#ifndef VIDEO_POWER_HINT_H_

#define VIDEO_POWER_HINT_H_

#include <stdint.h>
#include <android/native_window.h>
#include <media/hardware/MetadataBufferType.h>
#include <media/IOMX.h>
#include <media/stagefright/AHierarchicalStateMachine.h>
#include <media/stagefright/CodecBase.h>
#include <media/stagefright/FrameRenderTracker.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/SkipCutBuffer.h>
#include <utils/NativeHandle.h>
#include <OMX_Audio.h>
#include <hardware/gralloc.h>
#include "cutils/properties.h"

#include <binder/IServiceManager.h>
#include <powermanager/IPowerManager.h>
#include <powermanager/PowerManager.h>
//


namespace android {

struct VideoPowerHint {
    VideoPowerHint()
    : mPowerHintAcquired(false),
      mHeightWidth(0),
      mFrameRate(-1),
      mEstimatedFrameRate(-1),
      mIsWFDRunning(false) {
    };
    virtual ~VideoPowerHint() {};
    virtual void acquire() = 0;
    virtual void release() = 0;
    void SetSize(int heightwidth) {mHeightWidth = heightwidth;}
    void SetFrameRate(int rate) {mFrameRate = rate;}
    void SetEstimatedFrameRate(int rate) {mEstimatedFrameRate = rate;}
    void SetIsWFDRunning(bool wfd) {mIsWFDRunning = wfd;}
protected:
    bool mPowerHintAcquired;
    int mHeightWidth;
    int mFrameRate;
    int mEstimatedFrameRate;
    bool mIsWFDRunning;

    enum {
        VENDOR_VIDEO_1 = 0x7f000017,
        VENDOR_VIDEO_2 = 0x7f000018,
        VENDOR_VIDEO_3 = 0x7f000019,
        VENDOR_VIDEO_4 = 0x7f00001a,
        VENDOR_VIDEO_60FPS = 0x7f000003,
    };
};

struct VideoPowerHintCPU : public VideoPowerHint {
    VideoPowerHintCPU() {mPowerHintID = 0;}
    ~VideoPowerHintCPU() {}
    virtual void acquire();
    virtual void release();
protected:
    int mPowerHintID;
    static int mAcquiredCount;
    static Mutex lock;
private:
    static ::android::sp<IPowerManager> mPowerManager;
};

struct VideoPowerHintCPULowPower2Core : public VideoPowerHintCPU {
    VideoPowerHintCPULowPower2Core()
    {
        mPowerHintID = VENDOR_VIDEO_1;
    }
    ~VideoPowerHintCPULowPower2Core() {}
    virtual void acquire()
    {
        Mutex::Autolock autoLock(lock);
        ALOGI("LowPower2Core size %d, frame rate %d, estimated frame rate %d", mHeightWidth, mFrameRate, mEstimatedFrameRate);
        if(!mIsWFDRunning && mHeightWidth <= 1920 * 1080 && ((mFrameRate > 0 && mFrameRate <= 30) ||
                        (mFrameRate == -1 && mEstimatedFrameRate > 0 && mEstimatedFrameRate < 34)))
        {
            if(mPowerHintAcquired)
            {
                return;
            }
            mPowerHintAcquired = true;
            ALOGI("LowPower2Core acquire count = %d acquire", mAcquiredCount);
            if (mAcquiredCount == 0)
            {
                VideoPowerHintCPU::acquire();
            }
            mAcquiredCount ++;
        }
    };
    virtual void release()
    {
        Mutex::Autolock autoLock(lock);
        if(!mPowerHintAcquired)
        {
            return;
        }
        mPowerHintAcquired = false;
        ALOGI("LowPower2Core acquire count = %d release", mAcquiredCount);
        mAcquiredCount --;
        if (mAcquiredCount == 0)
        {
            VideoPowerHintCPU::release();
        }
    };
};

struct VideoPowerHintCPULowPower3Core : public VideoPowerHintCPU {
    VideoPowerHintCPULowPower3Core()
    {
        mPowerHintID = VENDOR_VIDEO_2;
    }
    ~VideoPowerHintCPULowPower3Core() {}
    virtual void acquire()
    {
        Mutex::Autolock autoLock(lock);
        ALOGI("LowPower3Core size %d, frame rate %d, estimated frame rate %d", mHeightWidth, mFrameRate, mEstimatedFrameRate);
        if(!mIsWFDRunning && mHeightWidth > 1920 * 1080 && ((mFrameRate > 0 && mFrameRate <= 30) ||
                        (mFrameRate == -1 && mEstimatedFrameRate > 0 && mEstimatedFrameRate < 34)))
        {
            if(mPowerHintAcquired)
            {
                return;
            }
            mPowerHintAcquired = true;
            ALOGI("LowPower3Core acquire count = %d acquire", mAcquiredCount);
            if (mAcquiredCount == 0)
            {
                VideoPowerHintCPU::acquire();
            }
            mAcquiredCount ++;
        }
    };
    virtual void release()
    {
        Mutex::Autolock autoLock(lock);
        if(!mPowerHintAcquired)
        {
            return;
        }
        mPowerHintAcquired = false;
        ALOGI("LowPower3Core acquire count = %d release", mAcquiredCount);
        mAcquiredCount --;
        if (mAcquiredCount == 0)
        {
            VideoPowerHintCPU::release();
        }
    };
};


struct VideoPowerHintCPULowPower4Core : public VideoPowerHintCPU {
    VideoPowerHintCPULowPower4Core()
    {
        mPowerHintID = VENDOR_VIDEO_3;
    }
    ~VideoPowerHintCPULowPower4Core() {}
    virtual void acquire()
    {
        Mutex::Autolock autoLock(lock);
        ALOGI("LowPower4Core size %d, frame rate %d, estimated frame rate %d", mHeightWidth, mFrameRate, mEstimatedFrameRate);
        if(!mIsWFDRunning && mHeightWidth > 1920 * 1080 && (mFrameRate > 30 ||
                        (mFrameRate == -1 && mEstimatedFrameRate > 34 && mEstimatedFrameRate < 64)))
        {
            if(mPowerHintAcquired)
            {
                return;
            }
            mPowerHintAcquired = true;
            ALOGI("LowPower4Core acquire count = %d acquire", mAcquiredCount);
            if (mAcquiredCount == 0)
            {
                VideoPowerHintCPU::acquire();
            }
            mAcquiredCount ++;
        }
    };
    virtual void release()
    {
        Mutex::Autolock autoLock(lock);
        if(!mPowerHintAcquired)
        {
            return;
        }
        mPowerHintAcquired = false;
        ALOGI("LowPower4Core acquire count = %d release", mAcquiredCount);
        mAcquiredCount --;
        if (mAcquiredCount == 0)
        {
            VideoPowerHintCPU::release();
        }
    };
};


struct VideoPowerHintCPUHightPerformance : public VideoPowerHintCPU {
    VideoPowerHintCPUHightPerformance()
    {
        mPowerHintID = VENDOR_VIDEO_60FPS;
    }
    ~VideoPowerHintCPUHightPerformance() {}
    virtual void acquire()
    {
        Mutex::Autolock autoLock(lock);
        ALOGI("HightPerformance size %d, mIsWFDRunning %d", mHeightWidth, mIsWFDRunning );
        if(mIsWFDRunning && mHeightWidth >= 1920 * 1080)
        {
            if(mPowerHintAcquired)
            {
                return;
            }
            mPowerHintAcquired = true;
            ALOGI("HightPerformance acquire count = %d acquire", mAcquiredCount);
            if (mAcquiredCount == 0)
            {
                VideoPowerHintCPU::acquire();
            }
            mAcquiredCount ++;
        }
    };
    virtual void release()
    {
        Mutex::Autolock autoLock(lock);
        if(!mPowerHintAcquired)
        {
            return;
        }
        mPowerHintAcquired = false;
        ALOGI("HightPerformance acquire count = %d release", mAcquiredCount);
        mAcquiredCount --;
        if (mAcquiredCount == 0)
        {
            VideoPowerHintCPU::release();
        }
    };
};

struct VideoPowerHintInstance {
    VideoPowerHintInstance(bool isWFDRunning)
    {
        ALOGI("HightPerformance isWFDRunning  %d ", isWFDRunning);
        mPowerHintList.push_back(new VideoPowerHintCPUHightPerformance());
    };
    ~VideoPowerHintInstance() {};
    void acquire(int size, int frameRate, bool isWFDRunning)
    {
        List< VideoPowerHint*>::iterator iter;
        for (iter = mPowerHintList.begin(); iter != mPowerHintList.end(); iter++)
        {
            (*iter)->SetSize(size);
            (*iter)->SetFrameRate(frameRate);
            (*iter)->SetIsWFDRunning(isWFDRunning);
            (*iter)->acquire();
        }
    };
    void acquire(int estimatedFrameRate)
    {
        List< VideoPowerHint*>::iterator iter;
        for (iter = mPowerHintList.begin(); iter != mPowerHintList.end(); iter++)
        {
            (*iter)->SetEstimatedFrameRate(estimatedFrameRate);
            (*iter)->acquire();
        }
    };
    void release()
    {
        while(!mPowerHintList.empty())
        {
            VideoPowerHint* hint = *mPowerHintList.begin();
            mPowerHintList.erase(mPowerHintList.begin());
            hint->release();
            delete hint;
        }
    };
private:
    List<VideoPowerHint*> mPowerHintList;
};

}

#endif
