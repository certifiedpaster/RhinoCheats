#pragma once

#include <thread>
#include <future>

#include "../../Networking/Network.h"

#include "../../Resources/Resource.h"
#include "../WinAPI/Dialog.hpp"
#include "../WinAPI/Message.hpp"
#include "../WinAPI/Static.hpp"
#include "../WinAPI/ProgressBar.hpp"

#include "../../Security/Encryption/Encryption.h"

#include <src/BlackBone/Process/Process.h>
#include <src/BlackBone/Misc/Utils.h>

#include <VMProtectSDK.h>

class cWaitDlg : public cDialog
{
public:

	typedef struct
	{
		DWORD pid = 0;
		std::wstring procName;
		std::wstring procPath;

		bool waitActive = false;
		std::vector<blackbone::ProcessInfo> procList;
		std::vector<blackbone::ProcessInfo> procDiff;
	} InjectContext; // Struct for our game name, process name, and our pid to obtain

	cWaitDlg(HINSTANCE instance, InjectContext& context);
	~cWaitDlg();

	NTSTATUS _status = STATUS_SUCCESS;

private:

	NTSTATUS WaitForInjection();
	NTSTATUS GetTargetProcess(InjectContext & context, PROCESS_INFORMATION & pi); // Scan for our process

	DLG_HANDLER(OnInit);
	DLG_HANDLER(OnCancel);

protected:

	typedef enum
	{
		Game,
		Subscription,
		Expiration
	} eListViewColumn;

	typedef enum
	{
		_1Day,
		_1Week,
		_2Weeks,
		_1Month,
		_3Months,
		_6Months,
		_1Year,
		_Lifetime,
		_SpecialAccess,
	} eSubscriptionType;

	typedef struct
	{
		std::string name;
		std::string process;
		std::string link;
		std::string subscription;
		std::string date_start;
		std::string date_end;
	} Rhinocheats;

	ctrl::cStatic _waitText;
	ctrl::cProgressBar _progressBar;

	InjectContext& _context;
	std::thread _waitThread;

	HINSTANCE _instance; // passed in through WinMain and this class's default constructor
};