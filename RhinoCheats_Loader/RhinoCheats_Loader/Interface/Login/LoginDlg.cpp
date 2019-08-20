#include "LoginDlg.h"

cLoginDlg::cLoginDlg(HINSTANCE instance) : cDialog(IDD_LOGIN), _instance(instance)
{
	// setup event handlers
	_messages[WM_INITDIALOG] = static_cast<cDialog::fnDlgProc>(&cLoginDlg::OnInit);
	_messages[WM_COMMAND] = static_cast<cDialog::fnDlgProc>(&cLoginDlg::OnCommand);
	_messages[WM_CLOSE] = static_cast<cDialog::fnDlgProc>(&cLoginDlg::OnClose);

	_events[IDCANCEL] = static_cast<cDialog::fnDlgProc>(&cLoginDlg::OnClose);
	_events[IDOK] = static_cast<cDialog::fnDlgProc>(&cLoginDlg::OnOK);
}

cLoginDlg::~cLoginDlg()
{
	// deconstructor
}

INT_PTR cLoginDlg::OnInit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	cDialog::OnInit(hDlg, message, wParam, lParam);

	// load our icon
	HICON hIcon = LoadIcon(_instance, MAKEINTRESOURCE(IDI_ICON));
	SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	DeleteObject(hIcon);

	// assign our control IDs to our control class objects
	_editUserName.Attach(_hwnd, IDC_USERNAME);
	_editPassWord.Attach(_hwnd, IDC_PASSWORD);

	return TRUE;
}

INT_PTR cLoginDlg::OnClose(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// self-explanatory?
	return cDialog::OnClose(hDlg, message, wParam, lParam);	
}

INT_PTR cLoginDlg::OnOK(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// username - isEmpty
	if (_editUserName.Text().empty()) {		
		cMessage::ShowWarning(_hwnd, VMProtectDecryptString("Username cannot be empty."));
		return TRUE;
	}
	// username - space check
	if (_editUserName.Text().find(' ') != std::string::npos)
	{
		cMessage::ShowWarning(_hwnd, VMProtectDecryptString("Username cannot have spaces."));
		return TRUE;
	}
	// password - isEmpty
	if (_editPassWord.Text().empty()) {
		cMessage::ShowWarning(_hwnd, VMProtectDecryptString("Password cannot be empty."));
		return TRUE;
	}
	// password - space check
	if (_editPassWord.Text().find(' ') != std::string::npos)
	{
		cMessage::ShowWarning(_hwnd, VMProtectDecryptString("Password cannot have spaces."));
		return TRUE;
	}

	// write user login credentials to auth file if they are sucessfully validated
	if (ValidateClientInfo(_editUserName.Text(), _editPassWord.Text()))
	{
		WritePrivateProfileString(VMProtectDecryptString("RhinoCheats"), VMProtectDecryptString("USER"), _editUserName.Text().c_str(), VMProtectDecryptString("C:\\RC\\Authorization.ini"));
		WritePrivateProfileString(VMProtectDecryptString("RhinoCheats"), VMProtectDecryptString("PASS"), _editPassWord.Text().c_str(), VMProtectDecryptString("C:\\RC\\Authorization.ini"));

		return cDialog::OnClose(hDlg, message, wParam, lParam);
	}

	// validation failed
	return TRUE;
}