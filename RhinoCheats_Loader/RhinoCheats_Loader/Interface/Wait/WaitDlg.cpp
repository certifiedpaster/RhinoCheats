#include "WaitDlg.h"

cWaitDlg::cWaitDlg(HINSTANCE instance, InjectContext& context) : cDialog(IDD_WAIT),
_instance(instance), _context(context), _waitThread(&cWaitDlg::WaitForInjection, this)
{
	// setup event handlers
	_messages[WM_INITDIALOG] = static_cast<cDialog::fnDlgProc>(&cWaitDlg::OnInit);
	_messages[WM_CLOSE] = static_cast<cDialog::fnDlgProc>(&cWaitDlg::OnCancel);

	_events[IDCANCEL] = static_cast<cDialog::fnDlgProc>(&cWaitDlg::OnCancel);
}

cWaitDlg::~cWaitDlg()
{
	// join thread if possible
	if (_waitThread.joinable())
		_waitThread.join();
}

INT_PTR cWaitDlg::OnInit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	cDialog::OnInit(hDlg, message, wParam, lParam);

	// load our icon
	HICON hIcon = LoadIcon(_instance, MAKEINTRESOURCE(IDI_ICON));
	SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	DeleteObject(hIcon);

	// assign our control IDs to our control class objects
	_waitText.Attach(_hwnd, IDC_WAITTEXT);
	_waitText.Text(VMProtectDecryptString("Awaiting launch of ") + blackbone::Utils::WstringToAnsi(_context.procName));

	_progressBar.Attach(_hwnd, IDC_WAITBAR);
	_progressBar.SetMarque(true);

	return TRUE;
}

INT_PTR cWaitDlg::OnCancel(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	// User clicked the cancel button
	if (cMessage::ShowQuestion(_hwnd, VMProtectDecryptString("Are you sure you wish to cancel?")))
		_context.waitActive = false;

	return TRUE;
}

NTSTATUS cWaitDlg::WaitForInjection()
{
	PROCESS_INFORMATION pi = { NULL }; // handle/id for process/thread

	for (bool inject = true; inject && _status != STATUS_REQUEST_ABORTED; inject = false)
		_status = GetTargetProcess(_context, pi); // Scan for our process

	CloseDialog();
	return _status;
}