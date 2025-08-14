
// Test_QRCodeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Test_QRCode.h"
#include "Test_QRCodeDlg.h"
#include "afxdialogex.h"

#include "qrcodegen.hpp"

#include "D:\\1.Projects_C++\\Common\\Functions.h"
#include "D:\\1.Projects_C++\\Common\\GdiplusBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using std::uint8_t;
using qrcodegen::QrCode;
using qrcodegen::QrSegment;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestQRCodeDlg 대화 상자



CTestQRCodeDlg::CTestQRCodeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_QRCODE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestQRCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestQRCodeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CTestQRCodeDlg 메시지 처리기

BOOL CTestQRCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	const char* text = "otpauth://totp/helpu.jp?secret=BXKPLCWT27E2IC2S43OMLVMR6JZNTRTZLE6536DSDO5EMOVMOYQNTPYRDHTW2RPRCIVZQPJNXX7QCLGEL4MIHY7XNRKAH7ZINQHZ6YA=&issuer=id[SUPPORTER]";              // User-supplied text
	const QrCode::Ecc errCorLvl = QrCode::Ecc::LOW;  // Error correction level

	// Make and print the QR Code symbol
	const QrCode qr = QrCode::encodeText(text, errCorLvl);

	//dot size = 4px, margin = 4px for 4 multipled
	int w = qr.getSize() * 4 + 4 * 2;
	int h = qr.getSize() * 4 + 4 * 2;
	CGdiplusBitmap img(w, h, PixelFormat24bppRGB);
	Gdiplus::Graphics g(img);
	Gdiplus::SolidBrush br(Gdiplus::Color::Red);
	//img.get_raw_data();

	for (int y = 0; y < img.height; y += 4)
	{
		for (int x = 0; x < img.width; x += 4)
		{
			TRACE(_T("%d, %d = %d\n"), x, y, qr.getModule((x - 4) / 4, (y - 4) / 4));
			if (qr.getModule((x - 4) / 4, (y - 4) / 4))
				g.FillRectangle(&br, x, y, 4, 4);
			//img.set_pixel(x, y, qr.getModule((x - 4) / 4, (y - 4) / 4) ? Gdiplus::Color::Black : Gdiplus::Color::White);
		}
	}

	//img.set_raw_data();
	img.save(_T("D:\\Test_QRCode.bmp"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestQRCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestQRCodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestQRCodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

