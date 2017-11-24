
// MFC_LD_CodeDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MFC_LD_Code.h"
#include "MFC_LD_CodeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CStringA CMFC_LD_CodeDlg::FileName = " ";
CvCapture *CMFC_LD_CodeDlg::cap;
uint16_t CMFC_LD_CodeDlg::frame_counter=0;
IplImage *CMFC_LD_CodeDlg::frame;
bool CMFC_LD_CodeDlg::flag_Night = false;
CMFC_LD_CodeDlg::CMFC_LD_CodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_LD_CODE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_LD_CodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ImageSource, m_ImageSource);
	DDX_Control(pDX, IDC_ImageResult, m_ImageResult);
}

BEGIN_MESSAGE_MAP(CMFC_LD_CodeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BtnOpenFIle, &CMFC_LD_CodeDlg::OnBnClickedBtnopenfile)
	ON_BN_CLICKED(IDC_Btn_Start, &CMFC_LD_CodeDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CMFC_LD_CodeDlg �T���B�z�`��

BOOL CMFC_LD_CodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w
	m_ImageResult.SetWindowPos(NULL, 640, 45, 1255, 610, SWP_SHOWWINDOW);
	m_ImageSource.SetWindowPos(NULL, 0, 45, 640, 360, SWP_SHOWWINDOW);
	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CMFC_LD_CodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CMFC_LD_CodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_LD_CodeDlg::OnBnClickedBtnopenfile()
{
	CFileDialog OpnDlg(TRUE);
	if (OpnDlg.DoModal() == IDOK)
		FileName = OpnDlg.GetPathName(); // return full path and filename
	cap = cvCaptureFromFile(FileName);
	frame_counter = 0;
}


void CMFC_LD_CodeDlg::OnBnClickedBtnStart()
{
	m_threadPara.m_case = 0;
	m_threadPara.hWnd = m_hWnd;
	m_lpThread = AfxBeginThread(&CMFC_LD_CodeDlg::threadFun, (LPVOID)&m_threadPara);
}
UINT CMFC_LD_CodeDlg::threadFun(LPVOID LParam)
{
	CthreadParam* para = (CthreadParam*)LParam;
	CMFC_LD_CodeDlg* lpview = (CMFC_LD_CodeDlg*)(para->m_lpPara);
	para->m_blthreading = TRUE;

	switch (para->m_case)
	{
	case 0:
		lpview->Thread_CapFromFile(LParam);
		break;

	default:
		break;
	}
	para->m_blthreading = FALSE;
	para->m_case = 0xFF;
	return 0;

}

bool checkObj(Mat _img, int y, int x, int valueCheck)
{
	if (y >= 0 && x >= 0 && y < _img.rows&&x < _img.cols)
		if (_img.data[(y)*_img.cols + (x)] == valueCheck)
			return true;

	return false;
}
bool setToNumber_close(Mat img, int i, int j, Mat& _img, int count)
{
	//�T�{���S���]����F��A�]�������ܴN�n�A���@��
	bool haveSet = false;
	int boxSizeH = 30;
	int boxSizeW = 5;
	if (_img.data[(j)*_img.cols + (i)] == 0 && img.data[(j)*img.cols + (i)] == 255)//�p�G�ۤv�O[������]
																				   //�Y�|�P���F��N��ۤv�ܦ�count
		for (int n = -boxSizeH; n <= boxSizeH; n++)
			for (int m = -boxSizeW; m <= boxSizeW; m++)
				if (checkObj(_img, j + m, i + n, count))
				{
					_img.data[(j)*_img.cols + (i)] = count;
					haveSet = true;
					break;
				}

	return haveSet;
}
int Labeling(Mat img, Mat &_img)
{
	/*
	*  [�D����]  img��0
	*  [�w����]  _img��1~254
	*  [������]  img��255 _img��0
	*/

	//���������L�@�Ӷ®�
	for (int j = 0; j < img.rows; j++)
	{
		img.data[(j)*img.cols + (0)] = 0;
		img.data[(j)*img.cols + (img.cols - 1)] = 0;
	}

	for (int i = 0; i < img.cols; i++)
	{
		img.data[(0)*img.cols + (i)] = 0;
		img.data[(img.rows - 1)*img.cols + (i)] = 0;
	}

	int count = 10;//@�b�o�̳]�w��}�l�ϰ�]�w����
	_img = Mat::zeros(Size(img.cols, img.rows), CV_8UC1);
	bool keepFind = true;
	while (keepFind)
	{
		//����ؤl
		bool find = false;//�D���S�����
		for (int j = 1; j < img.rows && find == false; j++)
			for (int i = 1; i < img.cols&& find == false; i++)
			{
				if (img.data[(j)*img.cols + (i)] == 255 && _img.data[(j)*_img.cols + (i)] == 0)//�p�G�ۤv�O[������]
				{
					//���]�w�ۤv�O[�w����]
					_img.data[(j)*_img.cols + (i)] = count;
					//���X
					find = true;
				}
			}

		//�A�Ӱ��X��
		if (find == true)//�p�G�����h�X��
		{
			for (int j = 0; j < img.rows; j++)
				for (int i = 0; i < img.cols; i++)
					setToNumber_close(img, i, j, _img, count);

			//�����ᬰ�F�U�@�ӽs�����ǳ�
			count = count + 10;//@�b�o�̳]�w�C�Ӱϰ�ƭȮt�h��
		}
		else//�p�G�S���A���N�N��������������F
		{
			//�N�����j�M
			keepFind = false;
		}
	}
	return (count / 10) - 1;
}

void CMFC_LD_CodeDlg::Thread_CapFromFile(LPVOID lParam)
{
	CthreadParam * Thread_Info = (CthreadParam *)lParam;
	CMFC_LD_CodeDlg * hWnd = (CMFC_LD_CodeDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	uint16_t total_frameNo = cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_COUNT);
	if (cvGrabFrame(cap))
	{
		frame = cvRetrieveFrame(cap);
		frame_counter++;

		Mat Img = frame;
		flag_Night = NightMode(Img);
		if (!flag_Night)hWnd->SetDlgItemText(IDC_FrameMode, _T("Day"));
		if (flag_Night)hWnd->SetDlgItemText(IDC_FrameMode, _T("Night"));
	}
	while (cvGrabFrame(cap))
	{
		frame = cvRetrieveFrame(cap);
		frame_counter++;
		Mat Img = frame;
		clock_t t1 = clock();
		Mat Img_ROI = Img(cv::Rect(cv::Point(0, 450), cv::Point(1225, 610)));
		Mat Img_Mask;
		Mat Img_finalResult = Mat::zeros(Img_ROI.size(), CV_8UC3);
		CVector <objectInfo> obj;
		FindLineMask(Img_ROI, Img_Mask, obj);
		for (uint16_t i = 0; i < obj.size(); i++)
		{
			if (obj[i].Area > 250)
			{
				string LineType = LineClassify(obj[i].Img_obj, obj[i].center, obj[i].Area);
				Mat Img_ColorMask;	std::string LineColor;
				if (LineType != "false")
				{
					cvtColor(obj[i].Img_obj, Img_ColorMask, CV_GRAY2BGR);
					FillMaskColor(Img_ColorMask,Img_ROI,Img_ColorMask, obj[i].XYmax, obj[i].XYmin);
					LineColor = GetColor(Img_ColorMask);
				}
				if (LineColor != "false" && LineType != "false")
				{
					std::string L;
					L = LineColor + " " + LineType;
					bitwise_or(Img_ColorMask, Img_finalResult, Img_finalResult);
					ImgText(&(IplImage)Img_finalResult, L, obj[i].center.x, obj[i].center.y);
				}
			}
		}
		hWnd->ShowImage(frame, hWnd->GetDlgItem(IDC_ImageSource));
		hWnd->ShowImage(Img_finalResult, hWnd->GetDlgItem(IDC_ImageResult));
		CString str;
		str.Format(_T("%d / %d"), frame_counter, total_frameNo);
		hWnd->SetDlgItemText(IDC_FrameCount, str);
	}
}
void CMFC_LD_CodeDlg::FindLineMask(Mat src, Mat &dst, CVector <objectInfo> &obj) {
	Mat img_gray;
	cvtColor(src, img_gray, CV_RGB2GRAY);
	Mat img_blur;
	GaussianBlur(img_gray, img_blur, cv::Size(5, 5), 0, 0);
	Mat edges;
	Canny(img_blur, edges, 70, 150);
	Mat Img_Line, Img_PretendLine;
	PretendLine(edges, Img_PretendLine);
	HoughLineDetection(edges, Img_Line);
	Mat Img_Label;
	int Objcounter = Labeling(Img_Line, Img_Label);

	for (uint16_t i = 0; i < Objcounter; i++)
	{
		objectInfo objectInitial;
		objectInitial.center = { 0 };
		objectInitial.Area = 0;
		objectInitial.XYmax = { 0 };
		objectInitial.XYmin = { max(Img_Line.cols,Img_Line.rows),max(Img_Line.cols,Img_Line.rows) };
		objectInitial.Img_obj = Mat::zeros(Img_Line.size(), Img_Line.type());
		obj.push_back(objectInitial);
	}
	
	for(uint16_t i=0;i<Img_Line.cols;i++)
		for (uint16_t j = 0; j < Img_Line.rows; j++)
		{
			for (uint16_t k = 0; k < Objcounter; k++)
			{
				if (Img_Label.data[j*Img_Label.cols + i] == (10 * k + 10))
				{
					obj[k].Img_obj.data[j*Img_Label.cols + i] = 255;
					if (obj[k].XYmax.x < i)obj[k].XYmax.x = i;
					if (obj[k].XYmax.y < j)obj[k].XYmax.y = j;
					if (obj[k].XYmin.x > i)obj[k].XYmin.x = i;
					if (obj[k].XYmin.y > j)obj[k].XYmin.y = j;
				}
			}
		}

	for(uint16_t k=0;k<Objcounter;k++)
		for (size_t j = obj[k].XYmin.y; j <= obj[k].XYmax.y; j++)
		{
			Stack<cv::Point> Compensates;
			for (size_t i = obj[k].XYmin.x; i <= obj[k].XYmax.x; i++)
				if (obj[k].Img_obj.data[j*obj[k].Img_obj.cols + i] > 0)
					Compensates.push(Point(i, j));
			if (Compensates.size() >= 2)
			{
				int DataNum = Compensates.size();
				cv::Point P = Compensates.pop();
				for (uint16_t i = 0; i < DataNum - 1; i++)
				{
					cv::Point P2 = Compensates.pop();
					if (abs(P2.x - P.x) < 35 && abs(P2.x - P.x) > 1)
						line(obj[k].Img_obj, P, P2, Scalar(255), 2);
					P = P2;
				}
			}
		}
  
	for (uint16_t i = 0; i < obj.size(); i++)
	{
		obj[i].center = GetCenterAndArea(obj[i].Img_obj, obj[i].Area, obj[i].XYmax, obj[i].XYmin);
		Image_And_Range(Img_PretendLine, obj[i].Img_obj, obj[i].Img_obj, obj[i].XYmax, obj[i].XYmin);
	}
		

}


//�ΨӱN�_�����u�P���{�������D�u���|�X(�涰)
void CMFC_LD_CodeDlg::Image_And_Range(cv::Mat src, Mat src2, cv::Mat &dst,Point MaxXY,Point MinXY)
{
	Mat temp;
	if ((src.channels() == src2.channels()) && (src.size == src2.size))
	{
		temp = Mat::zeros(cv::Size(src.size()), src.type());
		if (src.channels() == 3)
		for (uint16_t i = MinXY.x; i < MaxXY.x; i++)
		{
			for (uint16_t j = MinXY.y; j < MaxXY.y; j++)
			{	
				
				for (unsigned char k = 0; k < 3; k++)
				{
					UINT32 index = j*3*src.cols + i*3;
					temp.data[index + k] = min(src.data[index + k], src2.data[index + k]);
				}

			}
		}
		if(src.channels() == 1)
			for (uint16_t i = MinXY.x; i < MaxXY.x; i++)
				for (uint16_t j = MinXY.y; j < MaxXY.y; j++)
				{
					temp.data[j*src.cols + i] = min(src.data[j*src.cols + i], src2.data[j*src.cols + i]);
				}
			
	}
	temp.copyTo(dst);
}
string CMFC_LD_CodeDlg::LineClassify(cv::Mat src, cv::Point Center, double Area)
{
	int LineWidth[2] = { 0 };

	for (uint16_t i = 1; i < src.cols; i++)
	{
		if (src.at<uchar>(Center.y, i) > 0 && src.at<uchar>(Center.y, i - 1) == 0)//�����k����
			LineWidth[0] = i;
		if (src.at<uchar>(Center.y, src.cols - i) > 0 && src.at<uchar>(Center.y, src.cols - i + 1) == 0)//�k��������
			LineWidth[1] = src.cols - i - 1;
	}
	double Length = Area / (abs(LineWidth[1] - LineWidth[0]));
	if (abs(LineWidth[1] - LineWidth[0]) > 15 && abs(LineWidth[1] - LineWidth[0]) < 48)//��u
	{
		if (Length > 35 && Length < 120)//��u
		{
			return "dotted line";
		}
		else if (Length > 130 && Length < 180)//��u
		{

			return "Solid line";
		}
		else
			return "false";
	}
	else if (abs(LineWidth[1] - LineWidth[0]) >= 48 && abs(LineWidth[1] - LineWidth[0])<80 && Length > 130 && Length < 180)//���u
	{
		return "double Line";
	}
	else
		return "false";

}
bool CMFC_LD_CodeDlg::NightMode(cv::Mat img)
{
	Mat Img_gray;

	cvtColor(img, Img_gray, CV_BGR2GRAY);
	double sum = 0;
	for (uint16_t i = 0; i<Img_gray.cols; i++)
		for (uint16_t j = 0; j < Img_gray.rows; j++)
		{
			sum += Img_gray.data[j*Img_gray.cols + i];
		}
	sum = sum / (Img_gray.rows*Img_gray.cols);
	if (sum >= 100)return false;
	if (sum < 100)return true;
}
void CMFC_LD_CodeDlg::ShowImage(cv::Mat Image, CWnd* pWnd)
{
	
	cv::Mat imgTmp;
	CRect rect;
	pWnd->GetClientRect(&rect);
	cv::resize(Image, imgTmp, cv::Size(rect.Width(), rect.Height()));

	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA);
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels()*(imgTmp.depth() + 1);
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;

	CDC *pDC = pWnd->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}
void CMFC_LD_CodeDlg::HoughLineDetection(cv::Mat src, cv::Mat &dst)
{
	vector<Vec4i> lines;
	int hough_threshold = 30;
	int hough_minLineLength = 60;
	
	HoughLinesP(src, lines, 1, CV_PI / 180, hough_threshold, hough_minLineLength, 50);
	dst = Mat::zeros(src.size(), CV_8UC1);
	for (uint16_t i = 0; i < lines.size(); i++)
	{
		if (lines[i][2] != lines[i][0])
		{
			double theta = atan((double)(lines[i][3] - lines[i][1]) / (double)(lines[i][2] - lines[i][0]));
			theta = theta * 180 / M_PI;
			if (30 <= theta  && theta <= 90 && lines[i][0] >= src.cols / 2)//�k��
				cv::line(dst, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), Scalar(255));
			if (-30 >= theta  && theta >= -90 && lines[i][0] <= src.cols / 2)//����
				cv::line(dst, cv::Point(lines[i][0], lines[i][1]), cv::Point(lines[i][2], lines[i][3]), Scalar(255));
		}
	}
}
void CMFC_LD_CodeDlg::PretendLine(cv::Mat src, cv::Mat &dst)
{
	vector<Vec4i> lines;
	int hough_threshold = 10;
	int hough_minLineLength = 60;
	IplConvKernel *pKernel = NULL;
	pKernel = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);
	cvDilate(&(IplImage)src, &(IplImage)src, pKernel, 1);
	cvErode(&(IplImage)src, &(IplImage)src, pKernel, 1);
	HoughLinesP(src, lines, 1, CV_PI / 180, hough_threshold, hough_minLineLength, 50);
	dst = Mat::zeros(src.size(), CV_8UC1);
	vector<Vec4i> linepoint_right, linepoint_Left;
	for (uint16_t i = 0; i < lines.size(); i++)
	{
		if (lines[i][2] != lines[i][0])
		{
			double theta = atan((double)(lines[i][3] - lines[i][1]) / (double)(lines[i][2] - lines[i][0]));
			theta = theta * 180 / M_PI;
			if (30 <= theta  && theta <= 60 && lines[i][0] >= src.cols / 2)//�k��
				linepoint_right.push_back(lines[i]);

			if (-30 >= theta  && theta >= -60 && lines[i][0] <= src.cols / 2)//����
				linepoint_Left.push_back(lines[i]);
		}
	}
	if (linepoint_right.size() > 0)
	{
		Vec4i closest_RLine = linepoint_right[0];
		for (uint16_t i = 0; i < linepoint_right.size(); i++)
		{
			if (closest_RLine[0] > linepoint_right[i][0])
				closest_RLine = linepoint_right[i];
		}
		double m_R = (double)(closest_RLine[1] - closest_RLine[3]) / (double)(closest_RLine[0] - closest_RLine[2]);
		double b_R = closest_RLine[1] - m_R*closest_RLine[0];
		Vec4i RFinal; RFinal[1] = 0; RFinal[3] = src.rows - 1;
		RFinal[0] = -(int)b_R / m_R;
		RFinal[2] = (int)(RFinal[3] - b_R) / m_R;
		for (size_t i = 0; i < 55; i++)
		{
			cv::line(dst, cv::Point(RFinal[0] + i, RFinal[1]), cv::Point(RFinal[2] + i, RFinal[3]), Scalar(255));
			cv::line(dst, cv::Point(RFinal[0] - i, RFinal[1]), cv::Point(RFinal[2] - i, RFinal[3]), Scalar(255));
		}
	}

	if (linepoint_Left.size() > 0)
	{
		Vec4i closest_LLine = linepoint_Left[0];
		for (uint16_t i = 0; i < linepoint_Left.size(); i++)
		{
			if (closest_LLine[0] < linepoint_Left[i][0])
				closest_LLine = linepoint_Left[i];
		}
		double m_L = (double)(closest_LLine[1] - closest_LLine[3]) / (double)(closest_LLine[0] - closest_LLine[2]);
		double b_L = closest_LLine[1] - m_L*closest_LLine[0];
		Vec4i LFinal; LFinal[1] = 0; LFinal[3] = src.rows - 1;
		LFinal[0] = -(int)b_L / m_L;
		LFinal[2] = (int)(LFinal[3] - b_L) / m_L;
		for (size_t i = 0; i < 55; i++)
		{
			cv::line(dst, cv::Point(LFinal[0] + i, LFinal[1]), cv::Point(LFinal[2] + i, LFinal[3]), Scalar(255));
			cv::line(dst, cv::Point(LFinal[0] - i, LFinal[1]), cv::Point(LFinal[2] - i, LFinal[3]), Scalar(255));
		}
	}
	
}



//�Ψӭp��ΤߩM���n
Point CMFC_LD_CodeDlg::GetCenterAndArea(Mat src,int &Area, Point Max, Point Min)
{
	Point corner;
	int x0 = 0, y0 = 0, sum = 0;
	
	for (int i = Min.x; i <Max.x; i++)
		for (int j = Min.y; j < Max.y; j++)
			if (src.data[j*src.cols+i] > 0)
			{
				x0 = x0 + i;
				y0 = y0 + j;
				sum = sum + 1;
			}

	if (sum == 0) { corner.x = 0; corner.y = 0; }
	else
	{
		corner.x = x0 / sum;
		corner.y = y0 / sum;
	}
	Area = sum;
	return corner;
}
//�ΨӱN���D�u���B�ӹ�ROI�����쥻���D�u���C��
void CMFC_LD_CodeDlg::FillMaskColor(cv::Mat Img_Mask, Mat Img_ROI, cv::Mat &dst,Point XYmax,Point XYmin)
{
	Mat temp = Mat::zeros(cv::Size(Img_Mask.size()), Img_Mask.type());
	for (uint16_t j = XYmin.y; j < XYmax.y; j++)
	{
		uchar* Uchar_ROI = Img_ROI.ptr<uchar>(j);
		uchar* Uchar_temp = temp.ptr<uchar>(j);
		uchar* Uchar_Mask= Img_Mask.ptr<uchar>(j);
		for (uint16_t i = XYmin.x; i < XYmax.x; i++)
		{
		  if(Uchar_Mask[i * 3]>0)
			for (uint16_t k = 0; k<3; k++)
				Uchar_temp[i * 3+k] = Uchar_ROI[i * 3 + k];
		}
	}
	temp.copyTo(dst);
}
std::string  CMFC_LD_CodeDlg::GetColor(Mat src)
{
	UINT32 grade_W = 0, grade_R = 0, grade_Y=0;
	for(uint16_t i=0;i<src.cols;i++)
		for (uint16_t j = 0; j < src.rows; j++)
		{
			if (src.data[j*src.cols * 3 + i * 3] > 0)
			{
				double V = 0.0, S = 0.0;
				double	H = 0.0;
				double max_value = 0.0, min_value = 0;
				
				double R = (double)src.data[j*src.cols * 3 + i * 3 ]/255.0f;
				double G = (double)src.data[j*src.cols * 3 + i * 3 +1] / 255.0f;
				double B = (double)src.data[j*src.cols * 3 + i * 3+2] / 255.0f;

				max_value =max_f(R, G, B);
				min_value=min_f(R, G, B);
				V = max_value;
				if (max_value == 0.0f) {
					S = 0;
					H = 0;
				}
				else if (max_value - min_value == 0.0f) {
					S = 0;
					H = 0;
				}
				else {
					S = (max_value - min_value) / max_value;

					if (max_value == R) {
						H = 60 * ((G - B) / (max_value - min_value)) + 0;
					}
					else if (max_value == G) {
						H = 60 * ((B -R) / (max_value - min_value)) + 120;
					}
					else {
						H = 60 * ((R - G) / (max_value - min_value)) + 240;
					}
				}

				if (H < 0) H += 360.0f;

				unsigned char dst_h = (unsigned char)(H / 2);   // dst_h : 0-180
				unsigned char dst_s = (unsigned char)(S * 255); // dst_s : 0-255
				unsigned char dst_v = (unsigned char)(V * 255); // dst_v : 0-255
				if (flag_Night == false)//�鶡
				{
					if ((0 <= dst_h) && (dst_h <= 180) && (0 <= dst_s && dst_s <= 60) && (180 <= dst_v) && (dst_v <= 255))//�զ�
						grade_W++;
					if ((120 <= dst_h) && (dst_h <= 165) && (43 <= dst_s) && (dst_s <= 255) && (46 <= dst_v) && (dst_v <= 255))//����
						grade_R++;
					if ((90 <= dst_h) && (dst_h <= 180) &&( 60 <= dst_s) && (dst_s <= 245) && (160 <= dst_v) && (dst_v <= 255))//����
						grade_Y++;
				}
				if (flag_Night == true)//�]��
				{
					if (0 <= H && H <= 170 && 0 <= S && S <= 40 && 60 <= V && V <= 255)//�զ�
						grade_W++;
					if (77 <= H && H <= 117 && 16 <= S && S <= 116 && 105 <= V && V <= 252)//����
						grade_Y++;
				}
			}
		}
	UINT32 GradeMax = max(grade_W, grade_R);
    GradeMax = max(grade_Y, GradeMax);
	if (GradeMax== grade_W)
		return "White";
	if (GradeMax == grade_R)
		return "Red";
	if (GradeMax == grade_Y)
        return "Yellow";

}

void CMFC_LD_CodeDlg::ImgText(IplImage* img, std::string text, int x, int y)
{
	CvFont font;
	double hscale = 0.6;
	double vscale = 0.6;
	int linewidth = 2;
	char buf[1024] = { 0 };
	memcpy(buf, text.c_str(), text.size());
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hscale, vscale, 0, linewidth);
	CvScalar textColor = cvScalar(0, 0, 255);
	CvPoint textPos = cvPoint(x, y);
	cvPutText(img, buf, textPos, &font, textColor);
}
