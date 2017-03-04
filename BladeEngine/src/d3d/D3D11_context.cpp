#include "d3d/D3D11_context.h"
#include <iostream>
#include <array>

namespace Blade
{
	// Private Methods -----------------------------------------------------------------
	bool D3D11Context::CreateD3D11DeviceAndContext()
	{
		unsigned int device_flags{ 0 };

#if defined(DEBUG) || defined(_DEBUG)
		device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


		std::array<D3D_FEATURE_LEVEL, 3> feature_levels = {
			{ D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0 }
		};

		D3D_FEATURE_LEVEL feature_level;

		if (!m_Device)
		{
			HRESULT h_result{ 0 };
			h_result = D3D11CreateDevice(nullptr,
			                             D3D_DRIVER_TYPE_HARDWARE,
			                             nullptr,
			                             device_flags,
			                             &feature_levels[0],
			                             feature_levels.size(),
			                             D3D11_SDK_VERSION,
			                             m_Device.ReleaseAndGetAddressOf(),
			                             &feature_level,
			                             m_DeviceContext.ReleaseAndGetAddressOf());

			if (FAILED(h_result))
			{
				std::cerr << "D3D11CreateDevice Failed." << std::endl;
				return false;
			}

			if (feature_level != D3D_FEATURE_LEVEL_11_0)
			{
				std::cerr << "Direct3D Feature Level 11 unsupported." << std::endl;
				return false;
			}

#if defined(DEBUG) || defined(_DEBUG)
			h_result = m_Device.As(&m_DebugInterface);
			if (FAILED(h_result))
			{
				std::cerr << "Failed to acquire ID3D11Debug interface!" << std::endl;
				return false;
			}
#endif
		}

		return true;
	}

	// ---------------------------------------------------------------------------------

	bool D3D11Context::Create()
	{
		return CreateD3D11DeviceAndContext();
	}

	ID3D11Device* D3D11Context::GetDevice() const
	{
		return m_Device.Get();
	}

	ID3D11DeviceContext* D3D11Context::GetDeviceContext() const
	{
		return m_DeviceContext.Get();
	}

	unsigned int D3D11Context::GetMSAAQuality(int sample_count) const
	{
		unsigned int quality;
		m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		                                        sample_count,
		                                        &quality);
		assert(quality > 0);

		return quality;
	}
}
