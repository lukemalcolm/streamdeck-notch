//==============================================================================
/**
@file       NotchStreamDeckPlugin.cpp

@brief      Notch plugin

@copyright  (c) 2018, Corsair Memory, Inc.
			This source code is licensed under the MIT-style license found in the LICENSE file.

**/
//==============================================================================

#include "NotchStreamDeckPlugin.h"
#include "Common/EPLJSONUtils.h"
#include <atomic>

#include "Common/ESDConnectionManager.h"

class CallBackTimer
{
public:
    CallBackTimer() :_execute(false) { }

    ~CallBackTimer()
    {
        if( _execute.load(std::memory_order_acquire) )
        {
            stop();
        };
    }

    void stop()
    {
        _execute.store(false, std::memory_order_release);
        if(_thd.joinable())
            _thd.join();
    }

    void start(int interval, std::function<void(void)> func)
    {
        if(_execute.load(std::memory_order_acquire))
        {
            stop();
        };
        _execute.store(true, std::memory_order_release);
        _thd = std::thread([this, interval, func]()
        {
            while (_execute.load(std::memory_order_acquire))
            {
                func();
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }
        });
    }

    bool is_running() const noexcept
    {
        return (_execute.load(std::memory_order_acquire) && _thd.joinable());
    }

private:
    std::atomic<bool> _execute;
    std::thread _thd;
};

NotchStreamDeckPlugin::NotchStreamDeckPlugin()
{
	mCpuUsageHelper = new CpuUsageHelper();
	mTimer = new CallBackTimer();
	mTimer->start(1000, [this]()
	{
		this->UpdateTimer();
	});
}

NotchStreamDeckPlugin::~NotchStreamDeckPlugin()
{
	if(mTimer != nullptr)
	{
		mTimer->stop();
		
		delete mTimer;
		mTimer = nullptr;
	}
	
	if(mCpuUsageHelper != nullptr)
	{
		delete mCpuUsageHelper;
		mCpuUsageHelper = nullptr;
	}
}

void NotchStreamDeckPlugin::UpdateTimer()
{
	//
	// Warning: UpdateTimer() is running in the timer thread
	//
	if(mConnectionManager != nullptr)
	{
		mVisibleContextsMutex.lock();
		int currentValue = mCpuUsageHelper->GetCurrentCPUValue();
		for (const std::string& context : mVisibleContexts)
		{
			//mConnectionManager->SetTitle(std::to_string(currentValue) + "%", context, kESDSDKTarget_HardwareAndSoftware);
		}
		mVisibleContextsMutex.unlock();
	}
}



std::string getButtonNumber(std::string inAction) {
	int underscorePos = inAction.find("_") + 1;
	return inAction.substr(underscorePos, inAction.length() - underscorePos);
}

int NotchStreamDeckPlugin::toggleAndReturn(std::string context) {
	if (this->m_buttonToggleStates.find(context) != this->m_buttonToggleStates.end()) {
		m_buttonToggleStates[context] = !m_buttonToggleStates[context];
	}
	else {
		m_buttonToggleStates[context] = 1;
	}
	return m_buttonToggleStates[context];
	
}

void NotchStreamDeckPlugin::KeyDownForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	if (inAction == "com.elgato.notch.momentaryButton" || inAction == "com.elgato.notch.toggleButton") {
		std::string button_address = inPayload["settings"]["button_address"];
		float button_value = inPayload["settings"]["button_value"];

		if (inAction == "com.elgato.notch.momentaryButton") {
			m_notchOSCActions.sendButtonState(button_address, button_value);
		}

		if (inAction == "com.elgato.notch.toggleButton") {
			int button_state = inPayload["state"];
			m_notchOSCActions.sendButtonState(button_address, (float)button_state * button_value);
		}
	}

	if (inAction == "com.elgato.notch.switchLayer") {
		int layerIndex = inPayload["settings"]["layerIndex"];
		m_notchOSCActions.sendSwitchLayer(layerIndex);
	}

	if (inAction == "com.elgato.notch.play") {
		m_notchOSCActions.sendPlay();
	}

	if (inAction == "com.elgato.notch.pause") {
		m_notchOSCActions.sendPause();
	}

	if (inAction == "com.elgato.notch.resetTime") {
		m_notchOSCActions.sendReset();
	}

	if (inAction == "com.elgato.notch.goToTime") {
		float time = inPayload["settings"]["goToTime"];
		m_notchOSCActions.sendGoToTime(time);
	}


}

void NotchStreamDeckPlugin::KeyUpForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	if (inAction == "com.elgato.notch.momentaryButton") {
		std::string button_address = inPayload["settings"]["button_address"];
		m_notchOSCActions.sendButtonState(button_address, 0.0f);
	}
}

void NotchStreamDeckPlugin::WillAppearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// Remember the context
	mVisibleContextsMutex.lock();
	mVisibleContexts.insert(inContext);
	mVisibleContextsMutex.unlock();
}

void NotchStreamDeckPlugin::WillDisappearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// Remove the context
	mVisibleContextsMutex.lock();
	mVisibleContexts.erase(inContext);
	mVisibleContextsMutex.unlock();
}

void NotchStreamDeckPlugin::DeviceDidConnect(const std::string& inDeviceID, const json &inDeviceInfo)
{
	// Nothing to do
}

void NotchStreamDeckPlugin::DeviceDidDisconnect(const std::string& inDeviceID)
{
	// Nothing to do
}

void NotchStreamDeckPlugin::SendToPlugin(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID)
{
	// Nothing to do
}
