//==============================================================================
/**
@file       CpuUsageHelper.h

@brief		Calculate the current CPU percentage

@copyright  (c) 2018, Corsair Memory, Inc.
			This source code is licensed under the MIT-style license found in the LICENSE file.

**/
//==============================================================================

#pragma once
#pragma comment(lib, "Pdh.lib")
#include "pdh.h"

class CpuUsageHelper
{
public:
	CpuUsageHelper();
	~CpuUsageHelper() {};
	
	int GetCurrentCPUValue();

private:
	PDH_HQUERY mCpuQuery;
	PDH_HCOUNTER mCpuTotal;
};

class NotchOSCActions
{
public:
	NotchOSCActions();
	~NotchOSCActions();

	bool setTargetIP(std::string ip);
	bool setTargetPort(std::string port);

	bool sendSingleFloat(std::string oscAddress, float value);
	bool sendSingleInt(std::string oscAddress, int value);
	bool sendString(std::string oscAddress, std::string value);
	bool sendNoValue(std::string oscAddress);

	bool sendButtonState(std::string buttonNumber, float value);
	bool sendSwitchLayer(int layerIndex);
	bool sendGoToTime(float time);
	bool sendPlay();
	bool sendPause();
	bool sendReset();

	std::string m_targetIP;
	int m_targetPort;

private:
	bool sendPacket(std::string ipAddress, int port, char * buffer, int packetSize);

};
