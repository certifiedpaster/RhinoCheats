#include "MainDlg.h"

cMainDlg::cMainDlg(HINSTANCE instance, std::vector<std::string>& logindata, std::string& info_from_server) :
cDialog(IDD_MAIN), 
_instance(instance),
_logindata(logindata),
_info_from_server(info_from_server)
{
	// setup event handlers
	_messages[WM_INITDIALOG] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnInit);
	_messages[WM_COMMAND] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnCommand);
	_messages[WM_CLOSE] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnClose);

	_events[ID_ACCOUNT_REFRESH] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnRefresh);
	_events[ID_ACCOUNT_LOGOUT] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnLogout);
	_events[ID_ACCEL_REFRESH] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnRefresh);
	_events[ID_ACCEL_LOGOUT] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnLogout);
	_events[IDC_INJECT] = static_cast<cDialog::fnDlgProc>(&cMainDlg::OnInject);
}

cMainDlg::~cMainDlg()
{
	// deconstructor
}

INT_PTR cMainDlg::OnInit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	cDialog::OnInit(hDlg, message, wParam, lParam);

	// load our icon
	HICON hIcon = LoadIcon(_instance, MAKEINTRESOURCE(IDI_ICON));
	SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	DeleteObject(hIcon);

	// assign our control IDs to our control class objects
	_clientInfo.Attach(_hwnd, IDC_CLIENTINFO);
	_cheatList.Attach(_hwnd, IDC_CHEATLIST);
	_inject.Attach(_hwnd, IDC_INJECT);
	_injectionState.Attach(_hwnd, IDC_INJECTIONSTATE);
	_info.Attach(CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", _hwnd, IDR_STATUS));
	
	// additional listview setup
	ListView_SetExtendedListViewStyle(_cheatList.GetHwnd(), LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	// listview sections
	_cheatList.AddColumn(VMProtectDecryptString("Game"), 200, Game);
	_cheatList.AddColumn(VMProtectDecryptString("Subscription"), 100, Subscription);
	_cheatList.AddColumn(VMProtectDecryptString("Expiration"), 100, Expiration);

	// progress bar setup
	_injectionState.SetRange(0, 90);

	// status bar setup
	_info.SetParts({ -1 });
	_info.SetText(0, VMProtectDecryptString("Idle"));

	_clientInfoFormat = _clientInfo.Text();

	// update our client's info with their name and the number of available cheats
	std::string szName = GetClientInfo(_info_from_server);
	_clientInfo.Text(blackbone::Utils::WstringToAnsi(blackbone::Utils::FormatString(blackbone::Utils::AnsiToWstring(_clientInfoFormat).c_str(), blackbone::Utils::AnsiToWstring(szName).c_str(), cheatList.size())));

	// dynamically add every member of our cheatlist to our listview control
	for (auto& cheat : cheatList)
		AddCheatToList(&cheat);

	return TRUE;
}

INT_PTR cMainDlg::OnClose(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// self-explanatory?
	return cDialog::OnClose(hDlg, message, wParam, lParam);
}

INT_PTR cMainDlg::OnRefresh(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (RefreshClientInfo(_logindata[0], _logindata[1], _logindata[2]))
	{
		// avoid duplicate entries
		cheatList.clear();
		_cheatList.Reset();

		// update our client's info with their name and the number of available cheats
		std::string szName = GetClientInfo(_info_from_server);
		_clientInfo.Text(blackbone::Utils::WstringToAnsi(blackbone::Utils::FormatString(blackbone::Utils::AnsiToWstring(_clientInfoFormat).c_str(), blackbone::Utils::AnsiToWstring(szName).c_str(), cheatList.size())));

		// dynamically add every member of our cheatlist to our listview control
		for (auto& cheat : cheatList)
			AddCheatToList(&cheat);

		return TRUE;
	}

	return cDialog::OnClose(hDlg, message, wParam, lParam);
}

INT_PTR cMainDlg::OnLogout(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// erase contents of sh_settings.ini and close the main dialog
	WritePrivateProfileString(VMProtectDecryptString("RhinoCheats"), VMProtectDecryptString("USER"), "", VMProtectDecryptString("C:\\RC\\Authorization.ini"));
	WritePrivateProfileString(VMProtectDecryptString("RhinoCheats"), VMProtectDecryptString("PASS"), "", VMProtectDecryptString("C:\\RC\\Authorization.ini"));

	return cDialog::OnClose(hDlg, message, wParam, lParam);
}

INT_PTR cMainDlg::OnInject(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// start the injection procedure on it's own thread using mutual exclusion so potential hangups don't stall the program or cause deadlock
	std::lock_guard<std::mutex> lg(_lock);
	std::thread(&cMainDlg::Inject, this).detach();

	return TRUE;
}
