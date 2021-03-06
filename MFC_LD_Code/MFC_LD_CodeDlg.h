
// MFC_LD_CodeDlg.h : 標頭檔
//

#pragma once
#define _USE_MATH_DEFINES
#define min_f(a, b, c)  (fminf(a, fminf(b, c)))
#define max_f(a, b, c)  (fmaxf(a, fmaxf(b, c)))
#include"cv.h"
#include"highgui.h"
#include "afxwin.h"
#include"math.h"
#include"Stack.h"
#include"ClassVector.h"
using namespace cv;
struct objectInfo
{
	Mat Img_obj;
	cv::Point center;
	cv::Point XYmax;
	cv::Point XYmin;
	int Area;

};
struct CthreadParam
{
public:
	HWND hWnd;
	LPVOID m_lpPara;
	UINT   m_case;
	BOOL m_blthreading;
};
// CMFC_LD_CodeDlg 對話方塊
class CMFC_LD_CodeDlg : public CDialogEx
{
// 建構
public:
	CMFC_LD_CodeDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_LD_CODE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnopenfile();
	CthreadParam m_threadPara;
	CWinThread*  m_lpThread;
	static UINT threadFun(LPVOID LParam);

	void Thread_CapFromFile(LPVOID lParam);
	void FindLineMask(Mat src, Mat & dst, CVector<objectInfo>& obj);
	
	void Image_And_Range(cv::Mat src, Mat src2, cv::Mat & dst, Point MaxXY, Point MinXY);
	string LineClassify(cv::Mat src, cv::Point Center, double Area);
	bool NightMode(cv::Mat img);
	void ShowImage(cv::Mat Image, CWnd * pWnd);
	void HoughLineDetection(cv::Mat src, cv::Mat & dst);
	void PretendLine(cv::Mat src, cv::Mat & dst);

	Point GetCenterAndArea(Mat src, int & Area, Point Max, Point Min);


	void FillMaskColor(cv::Mat src, Mat src2, cv::Mat & dst);

	void FillMaskColor(cv::Mat Img_Mask, Mat Img_ROI, cv::Mat & dst, Point XYmax, Point XYmin);

	std::string  GetColor(Mat src);


	void ImgText(IplImage * img, std::string text, int x, int y);
	
	static CStringA FileName;
	static CvCapture *cap;
	static uint16_t frame_counter;
	static IplImage* frame;
	CStatic m_ImageSource;
	CStatic m_ImageResult;
	afx_msg void OnBnClickedBtnStart();
	static bool flag_Night;
};
