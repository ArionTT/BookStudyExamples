#pragma once


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)																					 \
	{																										\
		HRESULT hr = (x);																	   \
		if(FAILED(hr))																			  \
		{																								  \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}																							   \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 


//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = 0; }


