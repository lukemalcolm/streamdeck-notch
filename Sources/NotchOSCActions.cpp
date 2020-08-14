//==============================================================================
/**
@file       NotchOSCActions.cpp

@brief		Send OSC actions to Notch

@copyright  Luke Malcolm

**/
//==============================================================================

#include "pch.h"

#include "Vendor/oscpp/client.hpp"
#include "Vendor/oscpp/util.hpp"

#include "NotchOSCActions.h"
#include "Vendor/asio/include/asio.hpp"
#include <stdexcept>  

using namespace asio;

CpuUsageHelper::CpuUsageHelper() 
{
	PdhOpenQuery(nullptr, NULL, &mCpuQuery);
	PdhAddEnglishCounter(mCpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &mCpuTotal);
	PdhCollectQueryData(mCpuQuery);
	GetCurrentCPUValue();
}

int CpuUsageHelper::GetCurrentCPUValue()
{
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(mCpuQuery);
	PdhGetFormattedCounterValue(mCpuTotal, PDH_FMT_LONG, NULL, &counterVal);
	return (int)counterVal.longValue;
}


size_t makePacket(void* buffer, size_t size)
{
	// Construct a packet
	OSCPP::Client::Packet packet(buffer, size);
	packet
		// Open a bundle with a timetag
		.openBundle(1234ULL)
		// Add a message with two arguments and an array with 6 elements;
		// for efficiency this needs to be known in advance.
		.openMessage("/s_new", 1)
		// Write the arguments
		.float32(330.0f)
		// Every `open` needs a corresponding `close`
		.closeMessage()
		.closeBundle();
	return packet.size();
}

NotchOSCActions::NotchOSCActions() {
	m_targetIP = "127.0.0.1";
	m_targetPort = 9001;
}

NotchOSCActions::~NotchOSCActions() {

}

bool NotchOSCActions::setTargetIP(std::string ip) {
	std::string ValidIpAddressRegex = "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$";
	std::string ValidHostnameRegex = "^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$";

	m_targetIP = ip;
	return true;
}

bool NotchOSCActions::setTargetPort(std::string port) {
	try
	{
		int safePort = std::stoi(port);
		m_targetPort = safePort;
		return true;

	}
	catch (const std::invalid_argument& e)
	{
		(void)e;
		return false;
	}
	catch (const std::out_of_range& e)
	{
		(void)e;
		return false;
	}	
}

bool NotchOSCActions::sendPacket(std::string ipAddress, int port, char* buffer, int packetSize) {
	io_service m_io_service;
	ip::udp::socket m_socket(m_io_service);
	ip::udp::endpoint remote_endpoint;

	m_socket.open(ip::udp::v4());

	remote_endpoint = ip::udp::endpoint(ip::address::from_string(ipAddress), port);

	mutable_buffer asioBuffer = asio::buffer(buffer, packetSize);

	m_socket.send_to(asioBuffer, remote_endpoint);

	m_socket.close();
	return true;
}

bool NotchOSCActions::sendSingleFloat(std::string oscAddress, float value) {
	const int bufferSize = 1024;
	char myBuffer[bufferSize];

	OSCPP::Client::Packet packet(&myBuffer[0], bufferSize);
	packet
		.openBundle(1234ULL)
		.openMessage(oscAddress.c_str(), 1)
		.float32(value)
		.closeMessage()
		.closeBundle();

	this->sendPacket(m_targetIP, m_targetPort, &myBuffer[0], packet.size());

	return true;
}

bool NotchOSCActions::sendSingleInt(std::string oscAddress, int value) {
	const int bufferSize = 1024;
	char myBuffer[bufferSize];

	OSCPP::Client::Packet packet(&myBuffer[0], bufferSize);
	packet
		.openBundle(1234ULL)
		.openMessage(oscAddress.c_str(), 1)
		.int32(value)
		.closeMessage()
		.closeBundle();

	this->sendPacket(m_targetIP, m_targetPort, &myBuffer[0], packet.size());

	return true;
}

bool NotchOSCActions::sendString(std::string oscAddress, std::string value) {
	const int bufferSize = 4096;
	char myBuffer[bufferSize];

	OSCPP::Client::Packet packet(&myBuffer[0], bufferSize);
	packet
		.openBundle(1234ULL)
		.openMessage(oscAddress.c_str(), 1)
		.string(value.c_str())
		.closeMessage()
		.closeBundle();

	this->sendPacket(m_targetIP, m_targetPort, &myBuffer[0], packet.size());

	return true;
}

bool NotchOSCActions::sendNoValue(std::string oscAddress) {
	const int bufferSize = 1024;
	char myBuffer[bufferSize];

	OSCPP::Client::Packet packet(&myBuffer[0], bufferSize);
	packet
		.openBundle(1234ULL)
		.openMessage(oscAddress.c_str(), 1)
		.closeMessage()
		.closeBundle();

	this->sendPacket(m_targetIP, m_targetPort, &myBuffer[0], packet.size());

	return true;
}

bool NotchOSCActions::sendButtonState(std::string buttonAddress, float value) {
	return sendSingleFloat(buttonAddress, value);
}

bool NotchOSCActions::sendSwitchLayer(int layerIndex) {
	std::string oscAddress = "/notch/transport/layer";
	return sendSingleInt(oscAddress, layerIndex);
}

bool NotchOSCActions::sendGoToTime(float time) {
	std::string oscAddress = "/notch/transport/time";
	return sendSingleFloat(oscAddress, time);
}

bool NotchOSCActions::sendPlay() {
	std::string oscAddress = "/notch/transport/play";
	return sendNoValue(oscAddress);
}

bool NotchOSCActions::sendPause() {
	std::string oscAddress = "/notch/transport/pause";
	return sendNoValue(oscAddress);
}

bool NotchOSCActions::sendReset() {
	std::string oscAddress = "/notch/transport/reset";
	return sendNoValue(oscAddress);
}
