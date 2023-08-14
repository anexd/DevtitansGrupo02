#include "smartlamp_service.h"

namespace aidl::devtitans::smartlamp {
    ndk::ScopedAStatus SmartlampService::connect(int32_t* _aidl_return) {
        *_aidl_return = this->smartlamp.connect();
        LOG(INFO) << "connect(): " << *_aidl_return;
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus SmartlampService::getLed(int32_t* _aidl_return) {
        *_aidl_return = this->smartlamp.getLed();
        LOG(INFO) << "getLed(): " << *_aidl_return;
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus SmartlampService::setLed(int32_t in_ledValue, bool* _aidl_return) {
        *_aidl_return = this->smartlamp.setLed(in_ledValue);
        LOG(INFO) << "setLed( " << in_ledValue << "): " << (*_aidl_return ? "true" : "false");
        return ndk::ScopedAStatus::ok();
    }

    ndk::ScopedAStatus SmartlampService::getLuminosity(std::string* _aidl_return) {
    std::string luminosity = this->smartlamp.getLuminosity();
    *_aidl_return = luminosity; // Correção aqui
    LOG(INFO) << "getLuminosity(): " << luminosity;
    return ndk::ScopedAStatus::ok();
}



}