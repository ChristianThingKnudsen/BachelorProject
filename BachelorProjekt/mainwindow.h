#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <iostream>

//CBCTRecon header files
#include "cbctrecon.h"
#include "cbctregistration.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
  std::unique_ptr<CbctRecon> m_cbctrecon;
  CbctRegistration *m_cbctregistration; // just for convienience

public slots:
    void foo();//void foo() {std::cerr << "hello world!\n";} // This is a test
    //void showProgressBar(std::text, std:_updateTime) {}
    //Methods imported from CBCTRecon

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
    void SLT_LoadSelectedProjFiles(){}; // based on presetting values on GUI,
                                      // including geometry files
    void SLT_ReloadProjections(){};
    void SLT_ExportHis(){};

    void SLT_LoadImageFloat3D(){}; // Dose file
    void SLTM_LoadDICOMdir(){};
    void SLTM_LoadRTKoutput(){};

    void SLT_DrawRawImages() const{}; // external *.his images
    void SLT_DrawProjImages(){}; // draw images from HIS FILE READER or filtered
                               // image before going into recon.
    void SLT_DrawReconImage(){};

    // tools
    void SLT_FileNameHex2Dec(){};
    void SLT_MakeElektaXML(){};

    // Gain/ Offset correction
    void SLT_OpenOffsetFile(){};
    void SLT_OpenGainFile(){};
    void SLT_OpenBadpixelFile(){};
    void SLT_ApplyCalibration() const{};

    // Gain/ Offset correction
    void SLT_SetHisDir();
    void SLT_OpenElektaGeomFile(){};
    void SLT_SetOutputPath(){};
    void SLT_DoReconstruction(){};
    // Profile table
    // void SLT_GetProjectionProfile();
    // void SLT_GetReconImgProfile(){};
    void SLT_CopyTableToClipBoard() const{};
    void SLT_DataProbeProj() const{};
    void SLT_DataProbeRecon() const{};
    void SLT_DrawGraph() const{};
    void SLT_InitializeGraphLim() const{};
    void SLT_UpdateTable(){};
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
    void SLT_DoBowtieCorrection(){};
    void SLT_Export2DDose_TIF(){};
    void SLTM_Export2DDoseMapAsMHA(){};
    void SLT_ViewRegistration() const{};
    void SLT_ViewHistogram() const{};
    void SLT_DoScatterCorrection_APRIORI(){};
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

private slots:
    void on_btnLoadCT_clicked();

    void on_btnInfo_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
