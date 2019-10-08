/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../BachelorProjekt/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[94];
    char stringdata0[1938];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 3), // "foo"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 17), // "SLT_LoadRawImages"
QT_MOC_LITERAL(4, 34, 15), // "SLT_Load3DImage"
QT_MOC_LITERAL(5, 50, 20), // "SLT_Load3DImageShort"
QT_MOC_LITERAL(6, 71, 18), // "SLT_LoadPlanCT_mha"
QT_MOC_LITERAL(7, 90, 21), // "SLT_LoadPlanCT_USHORT"
QT_MOC_LITERAL(8, 112, 19), // "SLT_LoadCBCTcorrMHA"
QT_MOC_LITERAL(9, 132, 18), // "SLT_LoadCTrigidMHA"
QT_MOC_LITERAL(10, 151, 19), // "SLT_LoadCTdeformMHA"
QT_MOC_LITERAL(11, 171, 16), // "SLT_LoadNKIImage"
QT_MOC_LITERAL(12, 188, 25), // "SLT_LoadSelectedProjFiles"
QT_MOC_LITERAL(13, 214, 8), // "QString&"
QT_MOC_LITERAL(14, 223, 4), // "path"
QT_MOC_LITERAL(15, 228, 21), // "SLT_ReloadProjections"
QT_MOC_LITERAL(16, 250, 13), // "SLT_ExportHis"
QT_MOC_LITERAL(17, 264, 20), // "SLT_LoadImageFloat3D"
QT_MOC_LITERAL(18, 285, 17), // "SLTM_LoadDICOMdir"
QT_MOC_LITERAL(19, 303, 18), // "SLTM_LoadRTKoutput"
QT_MOC_LITERAL(20, 322, 17), // "SLT_DrawRawImages"
QT_MOC_LITERAL(21, 340, 18), // "SLT_DrawProjImages"
QT_MOC_LITERAL(22, 359, 18), // "SLT_DrawReconImage"
QT_MOC_LITERAL(23, 378, 19), // "SLT_FileNameHex2Dec"
QT_MOC_LITERAL(24, 398, 17), // "SLT_MakeElektaXML"
QT_MOC_LITERAL(25, 416, 18), // "SLT_OpenOffsetFile"
QT_MOC_LITERAL(26, 435, 16), // "SLT_OpenGainFile"
QT_MOC_LITERAL(27, 452, 20), // "SLT_OpenBadpixelFile"
QT_MOC_LITERAL(28, 473, 20), // "SLT_ApplyCalibration"
QT_MOC_LITERAL(29, 494, 13), // "SLT_SetHisDir"
QT_MOC_LITERAL(30, 508, 22), // "SLT_OpenElektaGeomFile"
QT_MOC_LITERAL(31, 531, 17), // "SLT_SetOutputPath"
QT_MOC_LITERAL(32, 549, 20), // "SLT_DoReconstruction"
QT_MOC_LITERAL(33, 570, 24), // "SLT_CopyTableToClipBoard"
QT_MOC_LITERAL(34, 595, 17), // "SLT_DataProbeProj"
QT_MOC_LITERAL(35, 613, 18), // "SLT_DataProbeRecon"
QT_MOC_LITERAL(36, 632, 13), // "SLT_DrawGraph"
QT_MOC_LITERAL(37, 646, 22), // "SLT_InitializeGraphLim"
QT_MOC_LITERAL(38, 669, 15), // "SLT_UpdateTable"
QT_MOC_LITERAL(39, 685, 22), // "SLT_CalculateROI_Recon"
QT_MOC_LITERAL(40, 708, 21), // "SLT_CalculateROI_Proj"
QT_MOC_LITERAL(41, 730, 20), // "SLT_GoForcedProbePos"
QT_MOC_LITERAL(42, 751, 25), // "SLT_PostApplyFOVDispParam"
QT_MOC_LITERAL(43, 777, 20), // "SLT_DoPostProcessing"
QT_MOC_LITERAL(44, 798, 19), // "SLT_PostProcCropInv"
QT_MOC_LITERAL(45, 818, 21), // "SLT_ExportReconUSHORT"
QT_MOC_LITERAL(46, 840, 23), // "SLT_ExportReconSHORT_HU"
QT_MOC_LITERAL(47, 864, 44), // "SLT_ExportALL_DCM_and_SHORT_H..."
QT_MOC_LITERAL(48, 909, 9), // "SLT_DoBHC"
QT_MOC_LITERAL(49, 919, 22), // "SLT_DoBowtieCorrection"
QT_MOC_LITERAL(50, 942, 20), // "SLT_Export2DDose_TIF"
QT_MOC_LITERAL(51, 963, 25), // "SLTM_Export2DDoseMapAsMHA"
QT_MOC_LITERAL(52, 989, 20), // "SLT_ViewRegistration"
QT_MOC_LITERAL(53, 1010, 17), // "SLT_ViewHistogram"
QT_MOC_LITERAL(54, 1028, 31), // "SLT_DoScatterCorrection_APRIORI"
QT_MOC_LITERAL(55, 1060, 26), // "SLT_CalcAndSaveAngularWEPL"
QT_MOC_LITERAL(56, 1087, 30), // "SLT_DoScatterCorrectionUniform"
QT_MOC_LITERAL(57, 1118, 35), // "SLT_FileExportShortDICOM_Curr..."
QT_MOC_LITERAL(58, 1154, 22), // "SLT_AddConstHUToCurImg"
QT_MOC_LITERAL(59, 1177, 19), // "SLT_CropSkinUsingRS"
QT_MOC_LITERAL(60, 1197, 26), // "SLT_CropSkinUsingThreshold"
QT_MOC_LITERAL(61, 1224, 28), // "SLT_ExportAngularWEPL_byFile"
QT_MOC_LITERAL(62, 1253, 19), // "SLT_GeneratePOIData"
QT_MOC_LITERAL(63, 1273, 15), // "SLT_LoadPOIData"
QT_MOC_LITERAL(64, 1289, 26), // "SLT_StartSyncFromSharedMem"
QT_MOC_LITERAL(65, 1316, 25), // "SLT_StopSyncFromSharedMem"
QT_MOC_LITERAL(66, 1342, 14), // "SLT_TimerEvent"
QT_MOC_LITERAL(67, 1357, 24), // "SLTM_ViewExternalCommand"
QT_MOC_LITERAL(68, 1382, 21), // "SLT_MedianFilterDoNow"
QT_MOC_LITERAL(69, 1404, 26), // "SLTM_ExportProjGeometryTXT"
QT_MOC_LITERAL(70, 1431, 22), // "SLTM_ForwardProjection"
QT_MOC_LITERAL(71, 1454, 37), // "SLTM_FineResolScatterCorrectr..."
QT_MOC_LITERAL(72, 1492, 33), // "SLTM_FullScatterCorrectionMac..."
QT_MOC_LITERAL(73, 1526, 34), // "SLTM_BatchScatterCorrectionMa..."
QT_MOC_LITERAL(74, 1561, 17), // "SLT_OpenPhaseData"
QT_MOC_LITERAL(75, 1579, 16), // "SLT_Export4DCBCT"
QT_MOC_LITERAL(76, 1596, 21), // "SLT_DoCouchCorrection"
QT_MOC_LITERAL(77, 1618, 26), // "SLTM_WELPCalcMultipleFiles"
QT_MOC_LITERAL(78, 1645, 25), // "SLTM_ScatterCorPerProjRef"
QT_MOC_LITERAL(79, 1671, 23), // "SLTM_LoadPerProjRefList"
QT_MOC_LITERAL(80, 1695, 18), // "SLTM_CropMaskBatch"
QT_MOC_LITERAL(81, 1714, 17), // "SLT_OutPathEdited"
QT_MOC_LITERAL(82, 1732, 22), // "SLT_SaveCurrentSetting"
QT_MOC_LITERAL(83, 1755, 14), // "SLT_CropSupInf"
QT_MOC_LITERAL(84, 1770, 20), // "init_DlgRegistration"
QT_MOC_LITERAL(85, 1791, 11), // "str_dcm_uid"
QT_MOC_LITERAL(86, 1803, 26), // "ReadBowtieFileWhileProbing"
QT_MOC_LITERAL(87, 1830, 25), // "FilterReaderType::Pointer"
QT_MOC_LITERAL(88, 1856, 9), // "proj_path"
QT_MOC_LITERAL(89, 1866, 22), // "std::tuple<bool,bool>&"
QT_MOC_LITERAL(90, 1889, 7), // "answers"
QT_MOC_LITERAL(91, 1897, 9), // "probeUser"
QT_MOC_LITERAL(92, 1907, 21), // "std::tuple<bool,bool>"
QT_MOC_LITERAL(93, 1929, 8) // "guessDir"

    },
    "MainWindow\0foo\0\0SLT_LoadRawImages\0"
    "SLT_Load3DImage\0SLT_Load3DImageShort\0"
    "SLT_LoadPlanCT_mha\0SLT_LoadPlanCT_USHORT\0"
    "SLT_LoadCBCTcorrMHA\0SLT_LoadCTrigidMHA\0"
    "SLT_LoadCTdeformMHA\0SLT_LoadNKIImage\0"
    "SLT_LoadSelectedProjFiles\0QString&\0"
    "path\0SLT_ReloadProjections\0SLT_ExportHis\0"
    "SLT_LoadImageFloat3D\0SLTM_LoadDICOMdir\0"
    "SLTM_LoadRTKoutput\0SLT_DrawRawImages\0"
    "SLT_DrawProjImages\0SLT_DrawReconImage\0"
    "SLT_FileNameHex2Dec\0SLT_MakeElektaXML\0"
    "SLT_OpenOffsetFile\0SLT_OpenGainFile\0"
    "SLT_OpenBadpixelFile\0SLT_ApplyCalibration\0"
    "SLT_SetHisDir\0SLT_OpenElektaGeomFile\0"
    "SLT_SetOutputPath\0SLT_DoReconstruction\0"
    "SLT_CopyTableToClipBoard\0SLT_DataProbeProj\0"
    "SLT_DataProbeRecon\0SLT_DrawGraph\0"
    "SLT_InitializeGraphLim\0SLT_UpdateTable\0"
    "SLT_CalculateROI_Recon\0SLT_CalculateROI_Proj\0"
    "SLT_GoForcedProbePos\0SLT_PostApplyFOVDispParam\0"
    "SLT_DoPostProcessing\0SLT_PostProcCropInv\0"
    "SLT_ExportReconUSHORT\0SLT_ExportReconSHORT_HU\0"
    "SLT_ExportALL_DCM_and_SHORT_HU_and_calc_WEPL\0"
    "SLT_DoBHC\0SLT_DoBowtieCorrection\0"
    "SLT_Export2DDose_TIF\0SLTM_Export2DDoseMapAsMHA\0"
    "SLT_ViewRegistration\0SLT_ViewHistogram\0"
    "SLT_DoScatterCorrection_APRIORI\0"
    "SLT_CalcAndSaveAngularWEPL\0"
    "SLT_DoScatterCorrectionUniform\0"
    "SLT_FileExportShortDICOM_CurrentImg\0"
    "SLT_AddConstHUToCurImg\0SLT_CropSkinUsingRS\0"
    "SLT_CropSkinUsingThreshold\0"
    "SLT_ExportAngularWEPL_byFile\0"
    "SLT_GeneratePOIData\0SLT_LoadPOIData\0"
    "SLT_StartSyncFromSharedMem\0"
    "SLT_StopSyncFromSharedMem\0SLT_TimerEvent\0"
    "SLTM_ViewExternalCommand\0SLT_MedianFilterDoNow\0"
    "SLTM_ExportProjGeometryTXT\0"
    "SLTM_ForwardProjection\0"
    "SLTM_FineResolScatterCorrectrionMacro\0"
    "SLTM_FullScatterCorrectionMacroAP\0"
    "SLTM_BatchScatterCorrectionMacroAP\0"
    "SLT_OpenPhaseData\0SLT_Export4DCBCT\0"
    "SLT_DoCouchCorrection\0SLTM_WELPCalcMultipleFiles\0"
    "SLTM_ScatterCorPerProjRef\0"
    "SLTM_LoadPerProjRefList\0SLTM_CropMaskBatch\0"
    "SLT_OutPathEdited\0SLT_SaveCurrentSetting\0"
    "SLT_CropSupInf\0init_DlgRegistration\0"
    "str_dcm_uid\0ReadBowtieFileWhileProbing\0"
    "FilterReaderType::Pointer\0proj_path\0"
    "std::tuple<bool,bool>&\0answers\0probeUser\0"
    "std::tuple<bool,bool>\0guessDir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      83,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  429,    2, 0x0a /* Public */,
       3,    0,  430,    2, 0x0a /* Public */,
       4,    0,  431,    2, 0x0a /* Public */,
       5,    0,  432,    2, 0x0a /* Public */,
       6,    0,  433,    2, 0x0a /* Public */,
       7,    0,  434,    2, 0x0a /* Public */,
       8,    0,  435,    2, 0x0a /* Public */,
       9,    0,  436,    2, 0x0a /* Public */,
      10,    0,  437,    2, 0x0a /* Public */,
      11,    0,  438,    2, 0x0a /* Public */,
      12,    1,  439,    2, 0x0a /* Public */,
      15,    0,  442,    2, 0x0a /* Public */,
      16,    0,  443,    2, 0x0a /* Public */,
      17,    0,  444,    2, 0x0a /* Public */,
      18,    0,  445,    2, 0x0a /* Public */,
      19,    0,  446,    2, 0x0a /* Public */,
      20,    0,  447,    2, 0x0a /* Public */,
      21,    0,  448,    2, 0x0a /* Public */,
      22,    0,  449,    2, 0x0a /* Public */,
      23,    0,  450,    2, 0x0a /* Public */,
      24,    0,  451,    2, 0x0a /* Public */,
      25,    0,  452,    2, 0x0a /* Public */,
      26,    0,  453,    2, 0x0a /* Public */,
      27,    0,  454,    2, 0x0a /* Public */,
      28,    0,  455,    2, 0x0a /* Public */,
      29,    0,  456,    2, 0x0a /* Public */,
      30,    0,  457,    2, 0x0a /* Public */,
      31,    0,  458,    2, 0x0a /* Public */,
      32,    0,  459,    2, 0x0a /* Public */,
      33,    0,  460,    2, 0x0a /* Public */,
      34,    0,  461,    2, 0x0a /* Public */,
      35,    0,  462,    2, 0x0a /* Public */,
      36,    0,  463,    2, 0x0a /* Public */,
      37,    0,  464,    2, 0x0a /* Public */,
      38,    0,  465,    2, 0x0a /* Public */,
      39,    0,  466,    2, 0x0a /* Public */,
      40,    0,  467,    2, 0x0a /* Public */,
      41,    0,  468,    2, 0x0a /* Public */,
      42,    0,  469,    2, 0x0a /* Public */,
      43,    0,  470,    2, 0x0a /* Public */,
      44,    0,  471,    2, 0x0a /* Public */,
      45,    0,  472,    2, 0x0a /* Public */,
      46,    0,  473,    2, 0x0a /* Public */,
      47,    0,  474,    2, 0x0a /* Public */,
      48,    0,  475,    2, 0x0a /* Public */,
      49,    0,  476,    2, 0x0a /* Public */,
      50,    0,  477,    2, 0x0a /* Public */,
      51,    0,  478,    2, 0x0a /* Public */,
      52,    0,  479,    2, 0x0a /* Public */,
      53,    0,  480,    2, 0x0a /* Public */,
      54,    0,  481,    2, 0x0a /* Public */,
      55,    0,  482,    2, 0x0a /* Public */,
      56,    0,  483,    2, 0x0a /* Public */,
      57,    0,  484,    2, 0x0a /* Public */,
      58,    0,  485,    2, 0x0a /* Public */,
      59,    0,  486,    2, 0x0a /* Public */,
      60,    0,  487,    2, 0x0a /* Public */,
      61,    0,  488,    2, 0x0a /* Public */,
      62,    0,  489,    2, 0x0a /* Public */,
      63,    0,  490,    2, 0x0a /* Public */,
      64,    0,  491,    2, 0x0a /* Public */,
      65,    0,  492,    2, 0x0a /* Public */,
      66,    0,  493,    2, 0x0a /* Public */,
      67,    0,  494,    2, 0x0a /* Public */,
      68,    0,  495,    2, 0x0a /* Public */,
      69,    0,  496,    2, 0x0a /* Public */,
      70,    0,  497,    2, 0x0a /* Public */,
      71,    0,  498,    2, 0x0a /* Public */,
      72,    0,  499,    2, 0x0a /* Public */,
      73,    0,  500,    2, 0x0a /* Public */,
      74,    0,  501,    2, 0x0a /* Public */,
      75,    0,  502,    2, 0x0a /* Public */,
      76,    0,  503,    2, 0x0a /* Public */,
      77,    0,  504,    2, 0x0a /* Public */,
      78,    0,  505,    2, 0x0a /* Public */,
      79,    0,  506,    2, 0x0a /* Public */,
      80,    0,  507,    2, 0x0a /* Public */,
      81,    0,  508,    2, 0x0a /* Public */,
      82,    0,  509,    2, 0x0a /* Public */,
      83,    0,  510,    2, 0x0a /* Public */,
      84,    1,  511,    2, 0x0a /* Public */,
      86,    2,  514,    2, 0x0a /* Public */,
      91,    1,  519,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   85,
    0x80000000 | 87, QMetaType::QString, 0x80000000 | 89,   88,   90,
    0x80000000 | 92, QMetaType::QString,   93,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->foo(); break;
        case 1: _t->SLT_LoadRawImages(); break;
        case 2: _t->SLT_Load3DImage(); break;
        case 3: _t->SLT_Load3DImageShort(); break;
        case 4: _t->SLT_LoadPlanCT_mha(); break;
        case 5: _t->SLT_LoadPlanCT_USHORT(); break;
        case 6: _t->SLT_LoadCBCTcorrMHA(); break;
        case 7: _t->SLT_LoadCTrigidMHA(); break;
        case 8: _t->SLT_LoadCTdeformMHA(); break;
        case 9: _t->SLT_LoadNKIImage(); break;
        case 10: _t->SLT_LoadSelectedProjFiles((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->SLT_ReloadProjections(); break;
        case 12: _t->SLT_ExportHis(); break;
        case 13: _t->SLT_LoadImageFloat3D(); break;
        case 14: _t->SLTM_LoadDICOMdir(); break;
        case 15: _t->SLTM_LoadRTKoutput(); break;
        case 16: _t->SLT_DrawRawImages(); break;
        case 17: _t->SLT_DrawProjImages(); break;
        case 18: _t->SLT_DrawReconImage(); break;
        case 19: _t->SLT_FileNameHex2Dec(); break;
        case 20: _t->SLT_MakeElektaXML(); break;
        case 21: _t->SLT_OpenOffsetFile(); break;
        case 22: _t->SLT_OpenGainFile(); break;
        case 23: _t->SLT_OpenBadpixelFile(); break;
        case 24: _t->SLT_ApplyCalibration(); break;
        case 25: _t->SLT_SetHisDir(); break;
        case 26: _t->SLT_OpenElektaGeomFile(); break;
        case 27: _t->SLT_SetOutputPath(); break;
        case 28: _t->SLT_DoReconstruction(); break;
        case 29: _t->SLT_CopyTableToClipBoard(); break;
        case 30: _t->SLT_DataProbeProj(); break;
        case 31: _t->SLT_DataProbeRecon(); break;
        case 32: _t->SLT_DrawGraph(); break;
        case 33: _t->SLT_InitializeGraphLim(); break;
        case 34: _t->SLT_UpdateTable(); break;
        case 35: _t->SLT_CalculateROI_Recon(); break;
        case 36: _t->SLT_CalculateROI_Proj(); break;
        case 37: _t->SLT_GoForcedProbePos(); break;
        case 38: _t->SLT_PostApplyFOVDispParam(); break;
        case 39: _t->SLT_DoPostProcessing(); break;
        case 40: _t->SLT_PostProcCropInv(); break;
        case 41: _t->SLT_ExportReconUSHORT(); break;
        case 42: _t->SLT_ExportReconSHORT_HU(); break;
        case 43: _t->SLT_ExportALL_DCM_and_SHORT_HU_and_calc_WEPL(); break;
        case 44: _t->SLT_DoBHC(); break;
        case 45: _t->SLT_DoBowtieCorrection(); break;
        case 46: _t->SLT_Export2DDose_TIF(); break;
        case 47: _t->SLTM_Export2DDoseMapAsMHA(); break;
        case 48: _t->SLT_ViewRegistration(); break;
        case 49: _t->SLT_ViewHistogram(); break;
        case 50: _t->SLT_DoScatterCorrection_APRIORI(); break;
        case 51: _t->SLT_CalcAndSaveAngularWEPL(); break;
        case 52: _t->SLT_DoScatterCorrectionUniform(); break;
        case 53: _t->SLT_FileExportShortDICOM_CurrentImg(); break;
        case 54: _t->SLT_AddConstHUToCurImg(); break;
        case 55: _t->SLT_CropSkinUsingRS(); break;
        case 56: _t->SLT_CropSkinUsingThreshold(); break;
        case 57: _t->SLT_ExportAngularWEPL_byFile(); break;
        case 58: _t->SLT_GeneratePOIData(); break;
        case 59: _t->SLT_LoadPOIData(); break;
        case 60: _t->SLT_StartSyncFromSharedMem(); break;
        case 61: _t->SLT_StopSyncFromSharedMem(); break;
        case 62: _t->SLT_TimerEvent(); break;
        case 63: _t->SLTM_ViewExternalCommand(); break;
        case 64: _t->SLT_MedianFilterDoNow(); break;
        case 65: _t->SLTM_ExportProjGeometryTXT(); break;
        case 66: _t->SLTM_ForwardProjection(); break;
        case 67: _t->SLTM_FineResolScatterCorrectrionMacro(); break;
        case 68: _t->SLTM_FullScatterCorrectionMacroAP(); break;
        case 69: _t->SLTM_BatchScatterCorrectionMacroAP(); break;
        case 70: _t->SLT_OpenPhaseData(); break;
        case 71: _t->SLT_Export4DCBCT(); break;
        case 72: _t->SLT_DoCouchCorrection(); break;
        case 73: _t->SLTM_WELPCalcMultipleFiles(); break;
        case 74: _t->SLTM_ScatterCorPerProjRef(); break;
        case 75: _t->SLTM_LoadPerProjRefList(); break;
        case 76: _t->SLTM_CropMaskBatch(); break;
        case 77: _t->SLT_OutPathEdited(); break;
        case 78: _t->SLT_SaveCurrentSetting(); break;
        case 79: _t->SLT_CropSupInf(); break;
        case 80: _t->init_DlgRegistration((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 81: { FilterReaderType::Pointer _r = _t->ReadBowtieFileWhileProbing((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< std::tuple<bool,bool>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< FilterReaderType::Pointer*>(_a[0]) = std::move(_r); }  break;
        case 82: { std::tuple<bool,bool> _r = _t->probeUser((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::tuple<bool,bool>*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 83)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 83;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 83)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 83;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
