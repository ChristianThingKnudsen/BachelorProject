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
  std::unique_ptr<CbctRecon> m_cbctrecon;
  std::unique_ptr<CbctRegistration> m_cbctregistration; // just for convienience
  std::unique_ptr<QStandardItemModel> m_pTableModel;
  std::unique_ptr<CbctRegistrationTest> m_dlgRegistration;
  FDK_options getFDKoptions() const;
  void UpdateReconImage(UShortImageType::Pointer &spNewImg, QString &fileName);
  void init_DlgRegistration(QString &str_dcm_uid) const;
  FilterReaderType::Pointer ReadBowtieFileWhileProbing(const QString &proj_path, std::tuple<bool, bool> &answers);
  std::tuple<bool, bool> probeUser(const QString &guessDir);
  void SelectComboExternal(const int idx, const enREGI_IMAGES iImage);
  void UpdateListOfComboBox(const int idx) const;
  void LoadImgFromComboBox(const int idx, QString &strSelectedComboTxt);
  void UpdateVOICombobox(const ctType ct_type) const;
  ctType get_ctType(const QString &selText);
  void whenFixedImgLoaded() const;
  //Threads
  LoadingThread *lThread;
  ScatterCorrectingThread *scThread;
  WEPLThread *weplThread;
  bool scatterCorrectingIsDone = false;


public slots:
    void SLT_LoadRawImages(){}; // independent 2d projection files //not used in
                              // clinical case
    void SLT_Load3DImage(){};   // indenepndent 3D mha file. UshortFormat. Do
                              // reconstruction is an antoher way to make
                              // m_spReconImg
    void SLT_Load3DImageShort(){};
    void SLT_LoadPlanCT_mha(){};
    void SLT_LoadPlanCT_USHORT(){};
    void SLT_LoadCBCTcorrMHA(){};
    void SLT_LoadCTrigidMHA(){};
    void SLT_LoadCTdeformMHA(){};
    void SLT_LoadNKIImage(){};
    void SLT_ReloadProjections(){};
    void SLT_ExportHis(){};

    void SLT_LoadImageFloat3D(){}; // Dose file
    void SLTM_LoadDICOMdir(){};
    void SLTM_LoadRTKoutput(){};

    void SLT_DrawRawImages() const{}; // external *.his images
    void SLT_DrawProjImages(){}; // draw images from HIS FILE READER or filtered
                               // image before going into recon.
    void SLT_DrawReconImage();

    // tools
    void SLT_FileNameHex2Dec(){};
    void SLT_MakeElektaXML(){};

    // Gain/ Offset correction
    void SLT_OpenOffsetFile(){};
    void SLT_OpenGainFile(){};
    void SLT_OpenBadpixelFile(){};
    void SLT_ApplyCalibration() const{};

    // Gain/ Offset correction
    void SLT_OpenElektaGeomFile();
    void SLT_SetOutputPath(){};
    void SLT_CopyTableToClipBoard() const{};
    void SLT_DataProbeProj() const{};
    void SLT_DataProbeRecon() const{};
    void SLT_DrawGraph() const {};
    void SLT_CalculateROI_Recon(){};
    void SLT_CalculateROI_Proj(){};
    void SLT_GoForcedProbePos(){};
    void SLT_PostApplyFOVDispParam(){};
    void SLT_DoPostProcessing(){}; // cropping Circle
    void SLT_PostProcCropInv(){};
    void SLT_ExportReconUSHORT(){};
    void SLT_ExportReconSHORT_HU(){};
    void SLT_ExportALL_DCM_and_SHORT_HU_and_calc_WEPL(){};
    void SLT_DoBHC(){};
    void SLT_Export2DDose_TIF(){};
    void SLTM_Export2DDoseMapAsMHA(){};
    void SLT_ViewHistogram() const{};
    void SLT_CalcAndSaveAngularWEPL(){};
    void SLT_DoScatterCorrectionUniform(){};
    void SLT_FileExportShortDICOM_CurrentImg(){};
    void SLT_AddConstHUToCurImg(){};
    void SLT_CropSkinUsingRS(){};
    void SLT_CropSkinUsingThreshold(){};
    void SLT_ExportAngularWEPL_byFile(){};
    void SLT_GeneratePOIData() const{};
    void SLT_LoadPOIData(){};
    static void SLT_StartSyncFromSharedMem(){};
    static void SLT_StopSyncFromSharedMem(){};
    void SLT_TimerEvent(){};
    void SLTM_ViewExternalCommand() const{};
    void SLT_MedianFilterDoNow(){};
    void SLTM_ExportProjGeometryTXT(){};
    void SLTM_ForwardProjection(){};
    void SLTM_FineResolScatterCorrectrionMacro(){}; // projection: full, scatter
                                                  // map:512x512

    void SLTM_FullScatterCorrectionMacroAP(){};
    void SLTM_BatchScatterCorrectionMacroAP(){};
    void SLT_OpenPhaseData(){};      // fill lineEdit_PhaseTxtPath
    void SLT_Export4DCBCT() const{}; // phase resorting
    void SLT_DoCouchCorrection(){};
    void SLTM_WELPCalcMultipleFiles(){};
    void SLTM_ScatterCorPerProjRef(){};
    void SLTM_LoadPerProjRefList(){};
    void SLTM_CropMaskBatch(){};
    void SLT_OutPathEdited() const{};  
    void SLT_SaveCurrentSetting() const{};
    void SLT_CropSupInf(){};
    void SLT_FixedImageSelected(QString selText);
    void SLT_MovingImageSelected(QString selText);
    void SLT_DrawImageWhenSliceChange();
    void SLT_DrawImageInFixedSlice() const;
    void SLT_OpenInfo();
    void SLT_OpenAdvancedMode();
    void SLT_Exit();
    void SLT_PreProcessCT();
    void SLT_IncreaseSliderValue();
    void SLT_DecreaseSliderValue();
    void SLT_StartLoadingThread();
    void SLT_ShowMessageBox(int, QString, QString);
    void SLT_StartScatterCorrectingThread();
    void SLT_InitializeSlider(FDK_options);
    void SLT_SetButtonsAfterLoad();
    void SLT_UpdateSlider(int);
    void SLT_DisconnectSlider();
    void SLT_ReConnectSlider(int);
    void SLT_UpdateLabelRaw(QString);
    void SLT_UpdateLabelCor(QString);
    void ImageManualMoveOneShot(
            const float shiftX, const float shiftY,
            const float shiftZ);
    void SLT_PassFixedImgForAnalysis(QString);
    void SLT_UpdateProgressBarLoad(int);
    void SLT_UpdateProgressBarSC(int);
    void SLT_SCThreadIsDone();
    void SLT_WEPLcalc(QString);
    void on_comboBoxWEPL_currentIndexChanged(const QString &arg1);
    void SLT_GetCBCTPath();
    void SLT_GetCTPath();
    void SLT_StartWEPLThread();
    void SLT_DrawReconInFixedSlice();
    void SLT_UpdateProgressBarWEPL(int progress);
    void SLT_LThreadIsDone();
    void SLT_SliderValueChanged();
    void SLT_DrawReconImageInSlices();

private:
    Ui::Scui *ui;
    int m_enViewArrange{};
    int m_enViewArrangeRaw{};
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
    QString Structure = QString("");
    int View =0;
    YK16GrayImage *m_YKDispRaw;
    YK16GrayImage *m_YKImgRawFixed;
    YK16GrayImage *m_YKImgRawMoving;
    UShortImageType::Pointer m_spRawFixedImg;  // pointer only, for display
    UShortImageType::Pointer m_spRawMovingImg; // pointer only, for display

private slots:

    void on_comboBoxPlanView_currentIndexChanged(const QString &arg1);
};
#endif // MAINWINDOW_H
