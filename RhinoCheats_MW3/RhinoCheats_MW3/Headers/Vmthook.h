#pragma once

class CVMTHookManager
{
	PDWORD*	m_ppdwClassBase;
	PDWORD	m_pdwNewVMT, m_pdwOldVMT;
	DWORD	m_dwVMTSize;

	DWORD dwGetVMTCount(PDWORD pdwVMT)
	{
		DWORD dwIndex = 0;

		for (dwIndex = 0; pdwVMT[dwIndex]; dwIndex++)
		{
			if (reinterpret_cast<FARPROC>(pdwVMT[dwIndex]) == nullptr)
			{
				break;
			}
		}
		return dwIndex;
	}

public:
	CVMTHookManager()
	{
		memset(this, 0, sizeof(CVMTHookManager));
	}

	~CVMTHookManager()
	{
		UnHook();
	}
	
	bool bInitialize(PDWORD* ppdwClassBase)
	{
		m_ppdwClassBase = ppdwClassBase;
		if (!m_ppdwClassBase)
			return false;
		
		if (!*ppdwClassBase)
			return false;		
		m_pdwOldVMT = *ppdwClassBase;
		
		m_dwVMTSize = dwGetVMTCount(*ppdwClassBase);
		if (!m_dwVMTSize)
			return false;

		m_pdwNewVMT = new DWORD[m_dwVMTSize];
		memcpy(m_pdwNewVMT, m_pdwOldVMT, sizeof(DWORD) * m_dwVMTSize);
		if (!m_pdwNewVMT)
			return false;

		*ppdwClassBase = m_pdwNewVMT;
		
		return true;
	}	
	
	DWORD dwHookMethod(DWORD dwNewFunc, unsigned int iIndex)
	{
		if (m_pdwNewVMT && m_pdwOldVMT && iIndex <= m_dwVMTSize && iIndex >= 0)
		{
			m_pdwNewVMT[iIndex] = dwNewFunc;
			return m_pdwOldVMT[iIndex];
		}

		return NULL;
	}
	
	void UnHook()
	{
		if (m_ppdwClassBase)
		{
			*m_ppdwClassBase = m_pdwOldVMT;
		}
	}	
};

extern CVMTHookManager* pD3DHook;