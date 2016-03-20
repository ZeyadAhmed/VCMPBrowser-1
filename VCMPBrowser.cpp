#include "VCMPBrowser.h"

HINSTANCE g_hInst;
HWND g_hMainWnd;
HWND g_hWndTab;
HWND g_hWndListViewServers;
HWND g_hWndListViewHistory;
HWND g_hWndGroupBox1;
HWND g_hWndListViewPlayers;
HWND g_hWndGroupBox2;
HWND g_hWndStatusBar;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

inline wchar_t* LoadStr(wchar_t* origString, UINT ID) { wchar_t* str; return (LoadString(g_hInst, ID, (LPWSTR)&str, 0) ? str : origString); }

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VCMPBROWSER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VCMPBROWSER);
	wcex.lpszClassName = L"VCMPBrowser";
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInst = hInstance;

	g_hMainWnd = CreateWindowEx(0, L"VCMPBrowser", LoadStr(L"VCMPBrowser", IDS_APP_TITLE), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, 1000, 600, nullptr, nullptr, hInstance, nullptr);

	if (!g_hMainWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hMainWnd, nCmdShow);
	UpdateWindow(g_hMainWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		RECT rcClient;
		GetClientRect(hWnd, &rcClient);
		g_hWndTab = CreateWindow(WC_TABCONTROL, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, 0, 0, rcClient.right - 259, rcClient.bottom - 120, hWnd, nullptr, g_hInst, nullptr);
		if (g_hWndTab)
		{
			SetWindowFont(g_hWndTab, hFont, FALSE);

			TCITEM tie;
			tie.mask = TCIF_TEXT;
			tie.pszText = LoadStr(L"Favorites", IDS_FAVORITES);
			TabCtrl_InsertItem(g_hWndTab, 0, &tie);
			tie.pszText = LoadStr(L"Internet", IDS_INTERNET);
			TabCtrl_InsertItem(g_hWndTab, 1, &tie);
			tie.pszText = LoadStr(L"Official", IDS_OFFICIAL);
			TabCtrl_InsertItem(g_hWndTab, 2, &tie);
			tie.pszText = LoadStr(L"History", IDS_HISTORY);
			TabCtrl_InsertItem(g_hWndTab, 3, &tie);

			g_hWndListViewServers = CreateWindow(WC_LISTVIEW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE, 1, 21, rcClient.right - 259 - 4, rcClient.bottom - 140 - 21 - 2, g_hWndTab, nullptr, g_hInst, nullptr);
			if (g_hWndListViewServers)
			{
				SetWindowTheme(g_hWndListViewServers, L"Explorer", nullptr);
				ListView_SetExtendedListViewStyle(g_hWndListViewServers, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

				LVCOLUMN lvc;
				lvc.mask = LVCF_WIDTH;
				lvc.cx = 30;
				ListView_InsertColumn(g_hWndListViewServers, 0, &lvc);

				lvc.mask = LVCF_WIDTH | LVCF_TEXT;
				lvc.cx = 260;
				lvc.pszText = LoadStr(L"Server Name", IDS_SERVERNAME);
				ListView_InsertColumn(g_hWndListViewServers, 1, &lvc);

				lvc.cx = 60;
				lvc.pszText = LoadStr(L"Ping", IDS_PING);
				ListView_InsertColumn(g_hWndListViewServers, 2, &lvc);

				lvc.cx = 80;
				lvc.pszText = LoadStr(L"Players", IDS_PLAYERS);
				ListView_InsertColumn(g_hWndListViewServers, 3, &lvc);

				lvc.cx = 70;
				lvc.pszText = LoadStr(L"Version", IDS_VERSION);
				ListView_InsertColumn(g_hWndListViewServers, 4, &lvc);

				lvc.cx = 120;
				lvc.pszText = LoadStr(L"Gamemode", IDS_GAMEMODE);
				ListView_InsertColumn(g_hWndListViewServers, 5, &lvc);

				lvc.cx = 100;
				lvc.pszText = LoadStr(L"Map Name", IDS_MAPNAME);
				ListView_InsertColumn(g_hWndListViewServers, 6, &lvc);

				LVITEM lvi;
				lvi.mask = LVIF_STATE;
				lvi.stateMask = 0;
				lvi.iSubItem = 0;
				lvi.state = 0;
				lvi.iItem = 0;
				lvi.pszText = nullptr;
				int i = ListView_InsertItem(g_hWndListViewServers, &lvi);

				ListView_SetItemText(g_hWndListViewServers, i, 1, L"TestServer");
				ListView_SetItemText(g_hWndListViewServers, i, 2, L"1234");
				ListView_SetItemText(g_hWndListViewServers, i, 3, L"0/0");
				ListView_SetItemText(g_hWndListViewServers, i, 4, L"04rel999");
				ListView_SetItemText(g_hWndListViewServers, i, 5, L"modename");
				ListView_SetItemText(g_hWndListViewServers, i, 6, L"mapname");
			}

			g_hWndListViewHistory = CreateWindow(WC_LISTVIEW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_AUTOARRANGE, 1, 21, rcClient.right - 259 - 4, rcClient.bottom - 140 - 21 - 2, g_hWndTab, nullptr, g_hInst, nullptr);
			if (g_hWndListViewHistory)
			{
				SetWindowTheme(g_hWndListViewHistory, L"Explorer", nullptr);
				ListView_SetExtendedListViewStyle(g_hWndListViewHistory, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

				LVCOLUMN lvc;
				lvc.mask = LVCF_WIDTH;
				lvc.cx = 30;
				ListView_InsertColumn(g_hWndListViewHistory, 0, &lvc);

				lvc.mask = LVCF_WIDTH | LVCF_TEXT;
				lvc.cx = 220;
				lvc.pszText = LoadStr(L"Server Name", IDS_SERVERNAME);
				ListView_InsertColumn(g_hWndListViewHistory, 1, &lvc);

				lvc.cx = 60;
				lvc.pszText = LoadStr(L"Ping", IDS_PING);
				ListView_InsertColumn(g_hWndListViewHistory, 2, &lvc);

				lvc.cx = 80;
				lvc.pszText = LoadStr(L"Players", IDS_PLAYERS);
				ListView_InsertColumn(g_hWndListViewHistory, 3, &lvc);

				lvc.cx = 70;
				lvc.pszText = LoadStr(L"Version", IDS_VERSION);
				ListView_InsertColumn(g_hWndListViewHistory, 4, &lvc);

				lvc.cx = 100;
				lvc.pszText = LoadStr(L"Gamemode", IDS_GAMEMODE);
				ListView_InsertColumn(g_hWndListViewHistory, 5, &lvc);

				lvc.cx = 160;
				lvc.pszText = LoadStr(L"Last Played", IDS_LASTPLAYED);
				ListView_InsertColumn(g_hWndListViewHistory, 6, &lvc);
			}
		}

		g_hWndGroupBox1 = CreateWindow(WC_BUTTON, LoadStr(L"Players", IDS_PLAYERSLIST), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_GROUPBOX, rcClient.right - 259, 0, 259, rcClient.bottom - 140, hWnd, nullptr, g_hInst, nullptr);
		if (g_hWndGroupBox1)
		{
			SetWindowFont(g_hWndGroupBox1, hFont, FALSE);

			g_hWndListViewPlayers = CreateWindowEx(0, WC_LISTVIEW, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | LVS_REPORT | LVS_SHOWSELALWAYS | LVS_NOCOLUMNHEADER | LVS_SINGLESEL, 1, 12, 259 - 2, rcClient.bottom - 140 - 12 - 2, g_hWndGroupBox1, nullptr, g_hInst, nullptr);
			if (g_hWndListViewPlayers)
			{
				SetWindowTheme(g_hWndListViewPlayers, L"Explorer", nullptr);
				ListView_SetExtendedListViewStyle(g_hWndListViewPlayers, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

				LVCOLUMN lvc;
				lvc.mask = LVCF_WIDTH;
				lvc.cx = 257;
				ListView_InsertColumn(g_hWndListViewPlayers, 0, &lvc);

				LVITEM lvi;
				lvi.mask = LVIF_TEXT | LVIF_STATE;
				lvi.stateMask = 0;
				lvi.iSubItem = 0;
				lvi.state = 0;
				lvi.iItem = 0;
				lvi.pszText = L"Test";
				ListView_InsertItem(g_hWndListViewPlayers, &lvi);
				ListView_InsertItem(g_hWndListViewPlayers, &lvi);
				ListView_InsertItem(g_hWndListViewPlayers, &lvi);
				ListView_InsertItem(g_hWndListViewPlayers, &lvi);
				ListView_InsertItem(g_hWndListViewPlayers, &lvi);
			}
		}

		g_hWndGroupBox2 = CreateWindow(WC_BUTTON, LoadStr(L"Server Info", IDS_SERVERINFO), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | BS_GROUPBOX, 0, rcClient.bottom - 140, rcClient.right, 118, hWnd, nullptr, g_hInst, nullptr);
		if (g_hWndGroupBox2)
		{
			SetWindowFont(g_hWndGroupBox2, hFont, FALSE);

			HWND hStatic = CreateWindow(WC_STATIC, LoadStr(L"Server Name:", IDS_SERVERNAME1), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_RIGHT, 10, 20, 100, 16, g_hWndGroupBox2, nullptr, g_hInst, nullptr);
			if (hStatic) SetWindowFont(hStatic, hFont, FALSE);

			HWND hEdit = CreateWindow(WC_EDIT, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY, 112, 20, 300, 16, g_hWndGroupBox2, (HMENU)1001, g_hInst, nullptr);
			if (hEdit) SetWindowFont(hEdit, hFont, FALSE);

			hStatic = CreateWindow(WC_STATIC, LoadStr(L"Server IP:", IDS_SERVERIP), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_RIGHT, 10, 40, 100, 16, g_hWndGroupBox2, nullptr, g_hInst, nullptr);
			if (hStatic) SetWindowFont(hStatic, hFont, FALSE);

			hEdit = CreateWindow(WC_EDIT, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY, 112, 40, 300, 16, g_hWndGroupBox2, (HMENU)1002, g_hInst, nullptr);
			if (hEdit) SetWindowFont(hEdit, hFont, FALSE);

			hStatic = CreateWindow(WC_STATIC, LoadStr(L"Server Players:", IDS_SERVERPLAYERS), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_RIGHT, 10, 60, 100, 16, g_hWndGroupBox2, nullptr, g_hInst, nullptr);
			if (hStatic) SetWindowFont(hStatic, hFont, FALSE);

			hEdit = CreateWindow(WC_EDIT, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY, 112, 60, 300, 16, g_hWndGroupBox2, (HMENU)1003, g_hInst, nullptr);
			if (hEdit) SetWindowFont(hEdit, hFont, FALSE);

			hStatic = CreateWindow(WC_STATIC, LoadStr(L"Server Ping:", IDS_SERVERPING), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_RIGHT, 10, 80, 100, 16, g_hWndGroupBox2, nullptr, g_hInst, nullptr);
			if (hStatic) SetWindowFont(hStatic, hFont, FALSE);

			hEdit = CreateWindow(WC_EDIT, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY, 112, 80, 300, 16, g_hWndGroupBox2, (HMENU)1004, g_hInst, nullptr);
			if (hEdit) SetWindowFont(hEdit, hFont, FALSE);

			hStatic = CreateWindow(WC_STATIC, LoadStr(L"Server Gamemode:", IDS_SERVERGAMEMODE), WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SS_RIGHT, 10, 100, 100, 16, g_hWndGroupBox2, nullptr, g_hInst, nullptr);
			if (hStatic) SetWindowFont(hStatic, hFont, FALSE);

			hEdit = CreateWindow(WC_EDIT, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY, 112, 100, 300, 16, g_hWndGroupBox2, (HMENU)1005, g_hInst, nullptr);
			if (hEdit) SetWindowFont(hEdit, hFont, FALSE);
		}

		g_hWndStatusBar = CreateWindow(STATUSCLASSNAME, nullptr, WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hWnd, nullptr, g_hInst, nullptr);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_NOTIFY:
	{
		switch (((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			switch (TabCtrl_GetCurSel(((LPNMHDR)lParam)->hwndFrom))
			{
			case 0: // Favorites
			case 1: // Internet
			case 2: // Official
				ShowWindow(g_hWndListViewServers, SW_SHOW);
				ShowWindow(g_hWndListViewHistory, SW_HIDE);
				break;
			case 3: // History
				ShowWindow(g_hWndListViewHistory, SW_SHOW);
				ShowWindow(g_hWndListViewServers, SW_HIDE);
				break;
			}
		}
	}
	break;
	case WM_SIZE:
	{
		int clientWidth = GET_X_LPARAM(lParam), clientHeight = GET_Y_LPARAM(lParam);
		SetWindowPos(g_hWndTab, 0, 0, 0, clientWidth - 259, clientHeight - 140, SWP_NOZORDER);
		SetWindowPos(g_hWndListViewServers, 0, 1, 21, clientWidth - 259 - 4, clientHeight - 140 - 21 - 2, SWP_NOZORDER);
		SetWindowPos(g_hWndListViewHistory, 0, 1, 21, clientWidth - 259 - 4, clientHeight - 140 - 21 - 2, SWP_NOZORDER);
		SetWindowPos(g_hWndGroupBox1, 0, clientWidth - 259, 0, 259, clientHeight - 140, SWP_NOZORDER);
		SetWindowPos(g_hWndListViewPlayers, 0, 1, 12, 259 - 2, clientHeight - 140 - 12 - 2, SWP_NOZORDER);
		SetWindowPos(g_hWndGroupBox2, 0, 0, clientHeight - 140, clientWidth, 118, SWP_NOZORDER);
		SendMessage(g_hWndStatusBar, WM_SIZE, 0, 0);
	}
	break;
	case WM_GETMINMAXINFO:
		((LPMINMAXINFO)lParam)->ptMinTrackSize = { 750, 500 };
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_CLICK:
		case NM_RETURN:
			ShellExecute(hDlg, L"open", ((PNMLINK)lParam)->item.szUrl, nullptr, nullptr, SW_SHOW);
		}
	}
	return (INT_PTR)FALSE;
}