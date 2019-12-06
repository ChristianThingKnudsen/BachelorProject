#ifndef SCUI_H
#define SCUI_H

#include <QMainWindow>

#include <iostream>

//CBCTRecon header files
#include "cbctrecon.h"
#include "cbctregistration.h"
#include "cbctregistration_test.hpp"
//Qt
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class Scui; }
QT_END_NAMESPACE

class LoadingThread;
class ScatterCorrectingThread;
class WEPLThread;

class Scui : public QMainWindow
{
    Q_OBJECT

public:
    Scui(QWidget *parent = nullptr);
    ~Scui();

public:
  std::mutex m_mutex;
  std::unique_ptr<CbctRecon> m_cbctrecon;
  std::unique_ptr<CbctRegistration> m_cbctregistration; // just for convienience
  std::unique_ptr<QStandardItemModel> m_pTableModel;
  std::unique_ptr<CbctRegistrationTest> m_dlgRegistration;
  FDK_options getFDKoptions() const;
  void UpdateReconImage(UShortImageType::Pointer &spNewImg);
  void init_DlgRegistration(QString &str_dcm_uid) const;
  FilterReaderType::Pointer ReadBowtieFileWhileProbing(const QString &proj_path, std::tuple<bool, bool> &answers);
  std::tuple<bool, bool> probeUser(const QString &guessDir);
  void SelectComboExternal(const int idx, const enREGI_IMAGES iImage);
  void UpdateListOfComboBox(const int idx) const;
  void LoadImgFromComboBox(const int idx, QString &strSelectedComboTxt);
  ctType get_ctType(const QString &selText);
  void whenFixedImgLoaded() const;
  //Threads
  LoadingThread *lThread;
  ScatterCorrectingThread *scThread;
  WEPLThread *weplThread;
  bool scatterCorrectingIsDone = false;

public slots:
    void SLT_DrawReconImage();

    // Gain/ Offset correction
    void SLT_FixedImageSelected(QString selText);
    void SLT_MovingImageSelected(QString selText);
    void SLT_DrawImageWhenSliceChange();
    void SLT_DrawImageInFixedSlice() const;
    void SLT_OpenInfo();
    void SLT_OpenAdvancedMode();
    void SLT_Exit();
    void SLT_IncreaseSliderValue();
    void SLT_DecreaseSliderValue();
    void SLT_StartLoadingThread();
    void SLT_StartScatterCorrectingThread();
    void SLT_UpdateSlider(int);
    void SLT_DisconnectSlider();
    void SLT_ReConnectSlider(int);
    void SLT_UpdateLabelRaw(QString);
    void SLT_UpdateLabelCor(QString);
    void ImageManualMoveOneShot(
            const float shiftX, const float shiftY,
            const float shiftZ);
    void SLT_PassFixedImgForAnalysis();
    void SLT_UpdateProgressBarLoad(int);
    void SLT_UpdateProgressBarSC(int);
    void SLT_SCThreadIsDone();
    void SLT_WEPLcalc(QString);
    void on_comboBoxWEPL_currentIndexChanged(const QString &arg1);
    void SLT_GetCBCTPath();
    void SLT_GetCTPath();
    void SLT_StartWEPLThread();
    //void SLT_DrawReconInFixedSlice(); // For further use
    void SLT_UpdateProgressBarWEPL(int progress);
    void SLT_LThreadIsDone();
    void SLT_SliderValueChanged();
    void SLT_DrawReconImageInSlices();
    void SLT_RestartSCUI();
    void SLT_CallPhysicist();
    void UpdateVOICombobox(const ctType ct_type) const;

private:
    Ui::Scui *ui;
    int m_enViewArrange = 0;
    int m_enViewArrangeRaw = 0;

public:
    YK16GrayImage *m_YKDisp;
    YK16GrayImage *m_YKImgFixed;
    YK16GrayImage *m_YKImgMoving;
    UShortImageType::Pointer m_spFixedImg;  // pointer only, for display
    UShortImageType::Pointer m_spMovingImg; // pointer only, for display
    UShortImageType::Pointer m_spFixedDose;  // pointer only, for display
    UShortImageType::Pointer m_spMovingDose; // pointer only, for display
    AG17RGBAImage *m_DoseImgFixed;
    AG17RGBAImage *m_DoseImgMoving;
    AG17RGBAImage *m_AGDisp_Overlay;
    QString CBCTPath = QString("");
    QString CTPath = QString("");
    QString DosisPlanPath = QString("");
    QString Structure = QString("");
    int View = 0;
    int RegionChosen = 0;
    QString Root;
    QString DownArrow;
    YK16GrayImage *m_YKDispRaw;
    YK16GrayImage *m_YKImgRawFixed;
    YK16GrayImage *m_YKImgRawMoving;
    UShortImageType::Pointer m_spRawFixedImg;  // pointer only, for display
    UShortImageType::Pointer m_spRawMovingImg; // pointer only, for display

#ifdef USE_CUDA
    bool m_UseCUDA = true;
    bool m_UseOpenCL = false;
#elif RTK_USE_OPENCL
    bool m_UseOpenCL = true;
    bool m_UseCUDA = false;
#else
    bool m_UseOpenCL = false;
    bool m_UseCUDA = false;
#endif

private slots:
    void on_comboBoxPlanView_currentIndexChanged(const QString &arg1);
    void on_comboBox_region_currentIndexChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
