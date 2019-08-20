#include "Login/LoginDlg.h"
#include "Main/MainDlg.h"
#include "Wait/WaitDlg.h"
#include "../Security/HWID/HWID.h"

bool cLoginDlg::ValidateClientInfo(std::string username, std::string password)
{
	// encrypt our plaintext to construct our request
	std::string encrypted_username = encrypt(username);
	std::string encrypted_password = encrypt(password);
	std::string encrypted_hwid = encrypt(GetHWID());

	// construct a request to send to the server
	std::string request = VMProtectDecryptString("login.php?");
	request.append(
		VMProtectDecryptString("&username=") +
		encrypted_username +
		VMProtectDecryptString("&password=") +
		encrypted_password +
		VMProtectDecryptString("&hwid=") +
		encrypted_hwid);

	// execute server request
	std::string _Info_from_server = HttpRequest(VMProtectDecryptString("authv1.rhinocheats.com"), request);

	// validate server response
	if (decrypt(_Info_from_server).empty())
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("The HTTP request returned NULL (failure)."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("nope."))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("This release has been obsoleted."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("empty hwid"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("Your HWID couldn't be determined."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("development"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("This product is undergoing development."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("different hwid"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("Your HWID has changed."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("no cheats"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("There's no cheats associated with your account."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("no customer"))
	{
		cMessage::ShowError(_hwnd, blackbone::Utils::WstringToAnsi(blackbone::Utils::FormatString(blackbone::Utils::AnsiToWstring(VMProtectDecryptString("Customer with the username \"%s\" doesn't exist.")).c_str(), blackbone::Utils::AnsiToWstring(decrypt(encrypted_username)).c_str())));
		return false;
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("bad password"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("Your password is incorrect."));
		return false;
	}

	// build info to pass into the constructor for our main dialog class
	_loginData.push_back(encrypted_username);
	_loginData.push_back(encrypted_password);
	_loginData.push_back(encrypted_hwid);

	_info_from_server = decrypt(_Info_from_server);

	return true;
}

bool cMainDlg::RefreshClientInfo(std::string username, std::string password, std::string hwid)
{
	std::string encrypted_username = username;
	std::string encrypted_password = password;
	std::string encrypted_hwid = hwid;

	// construct a request to send to the server
	std::string request = VMProtectDecryptString("login.php?");
	request.append(
		VMProtectDecryptString("&username=") +
		encrypted_username +
		VMProtectDecryptString("&password=") +
		encrypted_password +
		VMProtectDecryptString("&hwid=") +
		encrypted_hwid);

	// execute server request
	std::string _Info_from_server = HttpRequest(VMProtectDecryptString("authv1.rhinocheats.com"), request);

	// validate server response
	if (decrypt(_Info_from_server).empty())
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("The HTTP request returned NULL (failure)."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("nope."))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("This release has been obsoleted."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("empty hwid"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("Your HWID couldn't be determined."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("development"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("This product is undergoing development."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("different hwid"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("Your HWID has changed."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("no cheats"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("There's no cheats associated with your account."));
		exit(EXIT_FAILURE);
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("no customer"))
	{
		cMessage::ShowError(_hwnd, blackbone::Utils::WstringToAnsi(blackbone::Utils::FormatString(blackbone::Utils::AnsiToWstring(VMProtectDecryptString("Customer with the username \"%s\" doesn't exist.")).c_str(), blackbone::Utils::AnsiToWstring(decrypt(encrypted_username)).c_str())));
		return false;
	}

	if (decrypt(_Info_from_server) == VMProtectDecryptString("bad password"))
	{
		cMessage::ShowError(_hwnd, VMProtectDecryptString("Your password is incorrect."));
		return false;
	}

	_info_from_server = decrypt(_Info_from_server);

	return true;
}

std::string cMainDlg::GetClientInfo(std::string sData)
{
	int iLine = sData.find(VMProtectDecryptString("<br>"));

	std::string szName(sData.substr(0, iLine));

	// Getting the cheats
	sData = sData.substr(iLine + strlen(VMProtectDecryptString("<br>")), sData.length());

	while (TRUE) 
	{
		// Getting the whole cheat's line.
		iLine = sData.find(VMProtectDecryptString("#end#"), 0);

		if (iLine == -1)
			break;

		// Creating a substring to scan the whole line.
		std::string scheat = sData.substr(0, iLine);
		int icheat = scheat.find(VMProtectDecryptString("#"), 0);

		Rhinocheats ap;

		// name
		ap.name = scheat.substr(0, icheat);

		scheat = scheat.substr(icheat + strlen(VMProtectDecryptString("#")), scheat.length());
		icheat = scheat.find(
			VMProtectDecryptString("#"), 0);

		// process
		ap.process = scheat.substr(0, icheat);

		scheat = scheat.substr(icheat + strlen(VMProtectDecryptString("#")), scheat.length());
		icheat = scheat.find(
			VMProtectDecryptString("#"), 0);

		// link
		ap.link = scheat.substr(0, icheat);

		scheat = scheat.substr(icheat + strlen(VMProtectDecryptString("#")), scheat.length());
		icheat = scheat.find(
			VMProtectDecryptString("#"), 0);

		// subscription
		ap.subscription = scheat.substr(0, icheat);

		scheat = scheat.substr(icheat + strlen(VMProtectDecryptString("#")), scheat.length());
		icheat = scheat.find(
			VMProtectDecryptString("#"), 0);

		// dateStart
		ap.date_start = scheat.substr(0, icheat);

		scheat = scheat.substr(icheat + strlen(VMProtectDecryptString("#")), scheat.length());
		icheat = scheat.find(
			VMProtectDecryptString("#end#"), 0);

		// dateEnd
		ap.date_end = scheat.substr(0, icheat);

		scheat.clear();

		// Lets store the cheat into our cheatList
		cheatList.push_back(ap);

		//After #end#, to get the other cheats
		sData = sData.substr(iLine + strlen(VMProtectDecryptString("#end#")), sData.length());
	}

	return szName;
}

void cMainDlg::AddCheatToList(Rhinocheats* cheat)
{
	std::string szSubscriptionType;

	switch ((eSubscriptionType)atoi(cheat->subscription.c_str())) // assign a string for the type of subscription
	{
	case _1Day:
		szSubscriptionType = VMProtectDecryptString("1 Day");
		break;

	case _1Week:
		szSubscriptionType = VMProtectDecryptString("1 Week");
		break;

	case _2Weeks:
		szSubscriptionType = VMProtectDecryptString("2 Weeks");
		break;

	case _1Month:
		szSubscriptionType = VMProtectDecryptString("1 Month");
		break;

	case _3Months:
		szSubscriptionType = VMProtectDecryptString("3 Months");
		break;

	case _6Months:
		szSubscriptionType = VMProtectDecryptString("6 Months");
		break;

	case _1Year:
		szSubscriptionType = VMProtectDecryptString("1 Year");
		break;

	case _Lifetime:
		szSubscriptionType = VMProtectDecryptString("Lifetime");
		break;

	case _SpecialAccess:
		szSubscriptionType = VMProtectDecryptString("Special Access");
		break;

	default:
		return;
	}

	_cheatList.AddItem(cheat->name, (LPARAM)cheat, { szSubscriptionType, cheat->date_end }); // notice how we store the cheat as the LPARAM for later injection
}

NTSTATUS cWaitDlg::GetTargetProcess(InjectContext& context, PROCESS_INFORMATION& pi)
{
	std::vector<blackbone::ProcessInfo> newList;

	if (context.procList.empty())
		context.procList = blackbone::Process::EnumByNameOrPID(0, context.procPath).result(std::vector<blackbone::ProcessInfo>());

	// Wait patiently for our game's process to be launched, any matching process that was launched before this will NOT be detected. For safety reasons the game must be launched afterwards
	for (context.waitActive = true;; Sleep(10))
	{
		// User pressed cancel
		if (!context.waitActive)
		{
			return STATUS_REQUEST_ABORTED;
		}
		// Process was found
		if (!context.procDiff.empty())
		{
			context.procList = newList;
			context.pid = context.procDiff.front().pid;
			context.procDiff.erase(context.procDiff.begin());

			break;
		}
		// Keep checking for the process to launch
		else
		{
			newList = blackbone::Process::EnumByNameOrPID(0, context.procPath).result(std::vector<blackbone::ProcessInfo>());
			std::set_difference(
				newList.begin(), newList.end(),
				context.procList.begin(), context.procList.end(),
				std::inserter(context.procDiff, context.procDiff.begin())
			);
		}
	}

	return STATUS_SUCCESS;
}

void cMainDlg::Inject()
{
	// Disable button to prevent overlapping injections
	_inject.Disable();

	// get info for the highlighted cheat
	int iSelectedCheat = _cheatList.Selection();

	// user has highlighted a cheat
	if (iSelectedCheat > -1)
	{
		Rhinocheats* cheat = (Rhinocheats*)_cheatList.GetParam(iSelectedCheat); // get the cheat we stored as the LPARAM earlier.

		_injectionState.SetPos(10);
		_info.SetText(0, VMProtectDecryptString("Enumerating processes for ") + cheat->process + VMProtectDecryptString("..."));

		// get pid for our process
		cWaitDlg::InjectContext context;

		context.procName = blackbone::Utils::AnsiToWstring(cheat->name);
		context.procPath = blackbone::Utils::AnsiToWstring(cheat->process);

		cWaitDlg WaitDlg(_instance, context);
		WaitDlg.RunModal(_hwnd);

		if (!NT_SUCCESS(WaitDlg._status))
		{
			_injectionState.SetPos(0);
			_info.SetText(0, VMProtectDecryptString("Idle"));
			_inject.Enable();
			return;
		}

		_injectionState.SetPos(20);
		_info.SetText(0, VMProtectDecryptString("Requesting ") + cheat->name + VMProtectDecryptString(" cheat..."));
		
		// construct a request to send to the server
		std::string request = cheat->link.erase(0, strlen(VMProtectDecryptString("https://authv2.rhinocheats.com/"))) + VMProtectDecryptString("?");

		request.append(
			VMProtectDecryptString("&username=") +
			_logindata[0] +
			VMProtectDecryptString("&password=") +
			_logindata[1] +
			VMProtectDecryptString("&hwid=") +
			_logindata[2]);

		_injectionState.SetPos(30);
		_info.SetText(0, VMProtectDecryptString("Retrieving data from the server..."));

		// execute server request
		std::string _Info_from_server = HttpRequest(VMProtectDecryptString("authv2.rhinocheats.com"), request);

		_injectionState.SetPos(40);
		_info.SetText(0, VMProtectDecryptString("Validating response..."));

		// validate server response
		if (decrypt(_Info_from_server).empty())
		{
			cMessage::ShowError(_hwnd, VMProtectDecryptString("The data returned failed to validate."));
			_injectionState.SetPos(0);
			_info.SetText(0, VMProtectDecryptString("Idle"));
			_inject.Enable();
			return;
		}

		if (decrypt(_Info_from_server) == VMProtectDecryptString("development"))
		{
			cMessage::ShowError(_hwnd, VMProtectDecryptString("The data returned failed to validate."));
			_injectionState.SetPos(0);
			_info.SetText(0, VMProtectDecryptString("Idle"));
			_inject.Enable();
			return;
		}

		if (decrypt(_Info_from_server) == VMProtectDecryptString("empty hwid") ||
			decrypt(_Info_from_server) == VMProtectDecryptString("paranormal activity.") ||
			decrypt(_Info_from_server) == VMProtectDecryptString("the session is not authenticated.") ||
			decrypt(_Info_from_server) == VMProtectDecryptString("nope."))
		{
			cMessage::ShowError(_hwnd, VMProtectDecryptString("The data returned failed to validate."));
			_injectionState.SetPos(0);
			_info.SetText(0, VMProtectDecryptString("Idle"));
			_inject.Enable();
			return;
		}

		_injectionState.SetPos(50);
		_info.SetText(0, VMProtectDecryptString("Attaching to ") + cheat->process + VMProtectDecryptString("..."));
		
		// attempt to attach to the pid from above
		_lastStatus = _process.Attach(context.pid);

		if (!NT_SUCCESS(_lastStatus))
		{
			_process.Detach();
			cMessage::ShowError(_hwnd, VMProtectDecryptString("Failed to attach to ") + cheat->process + blackbone::Utils::WstringToAnsi(blackbone::Utils::FormatString(blackbone::Utils::AnsiToWstring(VMProtectDecryptString(" (0x%X).")).c_str(), context.pid)));
			_injectionState.SetPos(0);
			_info.SetText(0, VMProtectDecryptString("Idle"));
			_inject.Enable();
			return;
		}

		_injectionState.SetPos(60);
		for (int seconds = 15; seconds > 0; --seconds, Sleep(1000))
			_info.SetText(0, VMProtectDecryptString("Sleeping for ") + std::to_string(seconds) + VMProtectDecryptString(" seconds..."));

		_injectionState.SetPos(70);
		_info.SetText(0, VMProtectDecryptString("Injecting ") + cheat->name + VMProtectDecryptString(" cheat..."));

		// attempt to inject if attach succeeds
		_lastStatus = _process.mmap().MapImage(decrypt(_Info_from_server).size(), decrypt(_Info_from_server).data(), false, blackbone::WipeHeader).status;

		if (!NT_SUCCESS(_lastStatus))
		{
			cMessage::ShowError(_hwnd, VMProtectDecryptString("Failed to inject ") + cheat->name + VMProtectDecryptString(" cheat."));
			_injectionState.SetPos(0);
			_info.SetText(0, VMProtectDecryptString("Idle"));
			_inject.Enable();
			return;
		}

		_injectionState.SetPos(80);
		_info.SetText(0, VMProtectDecryptString("Cleaning up..."));

		// let's clean up after ourselves, mkay?
		_process.mmap().Cleanup();
		_process.Detach();

		_injectionState.SetPos(90);
		_info.SetText(0, VMProtectDecryptString("Success"));

		// let the user know their cheat was loaded and auto close
		cMessage::ShowInfo(_hwnd, cheat->name + VMProtectDecryptString(" cheat loaded!"));
		CloseDialog();
	}

	// no cheat highlighted
	else cMessage::ShowWarning(_hwnd, VMProtectDecryptString("You must select a cheat."));
	_inject.Enable();
}