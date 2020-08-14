//==============================================================================
/**
@file       NotchStreamDeckPlugin.h

@brief      Notch plugin

@copyright  (c) 2018, Corsair Memory, Inc.
			This source code is licensed under the MIT-style license found in the LICENSE file.

**/
//==============================================================================

#include "Common/ESDBasePlugin.h"
#include "NotchOSCActions.h"
#include <mutex>


class CpuUsageHelper;
class CallBackTimer;

class NotchStreamDeckPlugin : public ESDBasePlugin
{
public:
	
	NotchStreamDeckPlugin();
	virtual ~NotchStreamDeckPlugin();

	int toggleAndReturn(std::string context);
	
	void KeyDownForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
	void KeyUpForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
	
	void WillAppearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
	void WillDisappearForAction(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;
	
	void DeviceDidConnect(const std::string& inDeviceID, const json &inDeviceInfo) override;
	void DeviceDidDisconnect(const std::string& inDeviceID) override;
	
	void SendToPlugin(const std::string& inAction, const std::string& inContext, const json &inPayload, const std::string& inDeviceID) override;

	void ReceiveGlobalSettings(const std::string& inAction, const std::string& inContext, const json& inPayload, const std::string& inDeviceID) override;

private:
	
	void UpdateTimer();
	
	std::mutex mReadSettingsMutex;
	bool mRecvdSettings;

	CallBackTimer *mTimer;


	NotchOSCActions m_notchOSCActions;

	std::map<std::string, int> m_buttonToggleStates;

};
