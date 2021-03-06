#include "xinput_device.h"

namespace Blade
{
	XInputDevice::XInputDevice(int device_id, DeviceType devType)
	{

		// Set the device id
		SetDeviceID(device_id);

		// Set the device type
		SetDeviceType(devType);

	}

	void XInputDevice::Update(float deltaTime)
	{
		// Update the input states
		XINPUT_STATE tmpState;
		ZeroMemory(&tmpState, sizeof(XINPUT_STATE));

		// Get the state
		DWORD result{ XInputGetState(GetDeviceID(), &tmpState) };

		// Create an input state from the raw XINPUT data
		InputState rawState(tmpState);

		// filter the state into another
		InputState filteredState;
		FilterStateData(rawState, filteredState);

		// Store the filtered state (move current newest to previous, store newState as current)
		SetInputState(filteredState);

	}

	bool XInputDevice::SetVibration(float leftMotor, float rightMotor) const
	{
		XINPUT_VIBRATION vibParams;
		ZeroMemory(&vibParams, sizeof(XINPUT_VIBRATION));

		// scale to xinput range [0..1]-->[0..65536]
		vibParams.wLeftMotorSpeed = static_cast<int>(leftMotor * VIBRATION_THRESHOLD);
		vibParams.wRightMotorSpeed = static_cast<int>(rightMotor * VIBRATION_THRESHOLD);

		DWORD result = XInputSetState(GetDeviceID(), &vibParams);

		return result == ERROR_SUCCESS;
	}

	bool XInputDevice::Initialize()
	{
		// XInput devices are already initialised

		return true;
	}

	bool XInputDevice::IsConnected() const
	{
		// Prepare a state
		XINPUT_STATE tmpState;
		ZeroMemory(&tmpState, sizeof(XINPUT_STATE));

		// Query the state
		DWORD result{ XInputGetState(GetDeviceID(), &tmpState) };

		// The input manager will poll this function and act accordingly if the device is disconnected
		return result != ERROR_DEVICE_NOT_CONNECTED;
	}
}
