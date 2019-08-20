//=====================================================================================

#pragma once

#include <Windows.h>
#include <vector>

//=====================================================================================

#if defined (_M_AMD64) || defined (__x86_64__)
#define USE64
#elif defined (_M_IX86) || defined (__i386__)
#define USE32
#endif

#if defined USE64
#define POINTER DWORD64
#define XIP Rip
#define BASE Rax
#define REGISTER_COUNT 0x10
#elif defined USE32
#define POINTER DWORD32
#define XIP Eip
#define BASE Edi
#define REGISTER_COUNT 0x6
#endif

#define POINTER_SIZE sizeof(POINTER)

//=====================================================================================

namespace VEH
{
	typedef void(*tCallback)();

	typedef struct
	{
		int iIndex;
		POINTER pBackup, pDvar, pException;
		tCallback Callback;
	} sContext;

	static LPTOP_LEVEL_EXCEPTION_FILTER pVectoredExceptionHandler;
	static std::vector<sContext> vContext;

	static LONG CALLBACK VectoredExceptionHandler(_In_ LPEXCEPTION_POINTERS ExceptionInfo)
	{
		POINTER pRegisters = reinterpret_cast<POINTER>(&ExceptionInfo->ContextRecord->BASE);

		for (int i = 0; i < REGISTER_COUNT; i++)
		{
			for (auto& Context : vContext)
			{
				if (*reinterpret_cast<POINTER*>(pRegisters + (i * POINTER_SIZE)) == Context.iIndex)
				{
					*reinterpret_cast<POINTER*>(pRegisters + (i * POINTER_SIZE)) = Context.pBackup;

					if (ExceptionInfo->ContextRecord->XIP == Context.pException)
						Context.Callback();

					return EXCEPTION_CONTINUE_EXECUTION;
				}
			}
		}

		if (pVectoredExceptionHandler)
			return pVectoredExceptionHandler(ExceptionInfo);

		else
			return EXCEPTION_CONTINUE_SEARCH;
	}
	/*
	//=====================================================================================
	*/
	static void AddHook(POINTER dvar, POINTER exception, tCallback callback)
	{
		sContext Context;

		Context.iIndex = static_cast<int>(vContext.size() + 1);
		Context.pBackup = *reinterpret_cast<POINTER*>(dvar);
		Context.pDvar = dvar;
		Context.pException = exception;
		Context.Callback = callback;

		vContext.push_back(Context);
	}
	/*
	//=====================================================================================
	*/
	static void ForceExceptions()
	{
		pVectoredExceptionHandler = SetUnhandledExceptionFilter(VectoredExceptionHandler);

		for (auto& Context : vContext)
			*reinterpret_cast<POINTER*>(Context.pDvar) = Context.iIndex;
	}
	/*
	//=====================================================================================
	*/
	static void RemoveHooks()
	{
		for (auto& Context : vContext)
			*reinterpret_cast<POINTER*>(Context.pDvar) = Context.pBackup;

		vContext.clear();

		SetUnhandledExceptionFilter(pVectoredExceptionHandler);
	}
};

//=====================================================================================