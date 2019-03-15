
// ConfigFeatureOptionsDlg.h : header file
//

#pragma once

#include "OptionsFile.h"
// CConfigFeatureOptionsDlg dialog
class CConfigFeatureOptionsDlg : public CDialogEx {
// Construction
 public:
  CConfigFeatureOptionsDlg(CWnd *pParent = NULL);	// standard constructor

// Dialog Data
  enum { IDD = IDD_CONFIGFEATUREOPTIONS_DIALOG };

 protected:
  virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV support


// Implementation
 protected:
  HICON m_hIcon;
  OptionsFile m_optionFile;
  // Generated message map functions
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()
 public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedButtonEnableRemoveSofttissue();
  afx_msg void OnBnClickedButtonDisableRemoveSofttissue();
  void Refresh();
  afx_msg void OnBnClickedReset();
};
