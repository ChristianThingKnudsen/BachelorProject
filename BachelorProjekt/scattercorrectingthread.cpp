#include "scattercorrectingthread.h"

#include <QThread>
#include <QtCore>

#include "qyklabel.h"
#include <qfiledialog.h>

#include "cbctrecon.h"
#include "cbctregistration.h"
#include "scui.h"
#include "OpenCL/ImageFilters.hpp"

enum enCOLOR {
  RED,
  GREEN,
};

ScatterCorrectingThread::ScatterCorrectingThread(Scui *parent) : QThread(dynamic_cast<QObject*>(parent))// Constructur
{
    this->m_parent = parent;
    this->m_cbctrecon = parent->m_cbctrecon.get();
    this->m_cbctregistration = parent->m_cbctregistration.get();
}
void ScatterCorrectingThread::run(){ // Run Method
    emit Signal_FixedImageSelected(QString("RAW_CBCT"));
    emit Signal_UpdateLabelCor(QString("Raw CBCT"));
    emit Signal_MovingImageSelected(QString("MANUAL_RIGID_CT"));
    emit Signal_UpdateProgressBarSC(10);
    this->SLT_ManualMoveByDCMPlanOpen();
    this->SLT_ConfirmManualRegistration();
    this->SLT_DoRegistrationRigid();
}

void ScatterCorrectingThread::SLT_ManualMoveByDCMPlanOpen() { // Method is only needed in this class for testing reasons
  auto filePath = m_parent->DosisPlanPath;

  if (filePath.length() < 1) {
    return;
  }

  const auto planIso = m_cbctregistration->GetIsocenterDCM_FromRTPlan(filePath);

  if (fabs(planIso.x) < 0.001 && fabs(planIso.y) < 0.001 &&
      fabs(planIso.z) < 0.001) {
    std::cout
        << "Warning!!!! Plan iso is 0 0 0. Most likely not processed properly"
        << std::endl;
  } else {
    std::cout << "isocenter was found: " << planIso.x << ", " << planIso.y
              << ", " << planIso.z << std::endl;
  }

  if (m_cbctrecon->m_spRefCTImg == nullptr ||
      m_cbctrecon->m_spManualRigidCT == nullptr) {
      std::cerr << "No moving image available for isocenter move\n";
    return;
  }
  m_parent->ImageManualMoveOneShot(static_cast<float>(planIso.x),static_cast<float>(planIso.y),static_cast<float>(planIso.z));
}

void ScatterCorrectingThread::SLT_ConfirmManualRegistration() { // Confirms the manual registration
  if (m_parent->m_spFixedImg == nullptr || m_parent->m_spMovingImg == nullptr) {
    return;
  }

  auto p_parent = m_cbctregistration->m_pParent;
  if (p_parent->m_spRefCTImg == nullptr ||
      p_parent->m_spManualRigidCT == nullptr) {
    return;
  }

  // Apply post processing for raw CBCT image and generate
  std::cout << "Preprocessing for CBCT" << std::endl;

  auto originBefore = p_parent->m_spRefCTImg->GetOrigin();
  auto originAfter = p_parent->m_spManualRigidCT->GetOrigin();

  double fShift[3];
  fShift[0] = originBefore[0] - originAfter[0]; // DICOM
  fShift[1] = originBefore[1] - originAfter[1];
  fShift[2] = originBefore[2] - originAfter[2];

  const auto trn_vec = FloatVector{static_cast<float>(-fShift[0]),
                                   static_cast<float>(-fShift[1]),
                                   static_cast<float>(-fShift[2])};

  m_cbctregistration->m_pParent->m_structures
      ->ApplyVectorTransform_InPlace<PLAN_CT>(trn_vec);

  if (true){// True because we want to crop (from checkBoxCropBkgroundCT)
    auto structures =
        m_cbctregistration->m_pParent->m_structures->get_ss(RIGID_CT);

    // RIGID_CT structs should be created above
    const auto voi_name = QString("BODY");// Crop by using the body structure (from comboBox_VOItoCropBy)
    if (structures != nullptr) {
      const auto voi = structures->get_roi_ref_by_name(voi_name.toStdString());
      OpenCL_crop_by_struct_InPlace(p_parent->m_spRawReconImg, voi);
    }
    auto update_message = QString("CBCT cropped outside of ") + voi_name;
  }

  // Export final xform file
  auto filePathXform =
      m_cbctregistration->m_strPathPlastimatch + "/" + "xform_manual.txt";

  std::ofstream fout;
  fout.open(filePathXform.toLocal8Bit().constData());
  fout << "#Custom Transform" << std::endl;
  fout << "#Transform: Translation_only" << std::endl;
  fout << "Parameters: " << fShift[0] << " " << fShift[1] << " " << fShift[2]
       << std::endl;

  fout.close();
  std::cout << "Writing manual registration transform info is done."
            << std::endl;
}

void ScatterCorrectingThread::SLT_DoRegistrationRigid() // plastimatch auto registration
{
  // 1) Save current image files
   // m_spFixedImg replaced with m_spRawReconImg and m_spMovingImg replaced with m_spRefCTImg
  if (m_cbctrecon->m_spRawReconImg == nullptr || m_cbctrecon->m_spRefCTImg == nullptr) {
    return;
  }

  // Before the registration, remove background and fill the air-bubble of the
  // cbct  1) calculate manual shift value  PreProcess_CBCT(); // remove skin
  // and fill bubble before registration

  if (this->m_cbctrecon->m_spRefCTImg == nullptr ||
      this->m_cbctrecon->m_spManualRigidCT == nullptr) {
    return;
  }

  /*- Make a synthetic std::vector field according to the translation
        plastimatch synth-vf --fixed [msk_skin.mha] --output
  [xf_manual_trans.mha] --xf-trans "[origin diff (raw - regi)]" plastimatch
  synth-vf --fixed E:\PlastimatchData\DicomEg\OLD\msk_skin.mha --output
  E:\PlastimatchData\DicomEg\OLD\xf_manual_trans.mha --xf-trans "21 -29 1"

        - Move the skin contour according to the std::vector field
        plastimatch warp --input [msk_skin.mha] --output-img
  [msk_skin_manRegi.mha] --xf [xf_manual_trans.mha] plastimatch warp --input
  E:\PlastimatchData\DicomEg\OLD\msk_skin.mha --output-img
  E:\PlastimatchData\DicomEg\OLD\msk_skin_manRegi.mha --xf
  E:\PlastimatchData\DicomEg\OLD\xf_manual_trans.mha*/

  std::cout << "1: writing temporary files" << std::endl;

  // Both image type: Unsigned Short
  auto filePathFixed =
      m_cbctregistration->m_strPathPlastimatch + "/" + "fixed_rigid.mha";
  auto filePathMoving =
      m_cbctregistration->m_strPathPlastimatch + "/" + "moving_rigid.mha";
  auto filePathOutput =
      m_cbctregistration->m_strPathPlastimatch + "/" + "output_rigid.mha";
  const auto filePathXform =
      m_cbctregistration->m_strPathPlastimatch + "/" + "xform_rigid.txt";
  auto filePathROI = m_cbctregistration->m_strPathPlastimatch + "/" +
                     "fixed_roi_rigid.mha"; // optional

  using writerType = itk::ImageFileWriter<UShortImageType>;

  auto writer1 = writerType::New();
  writer1->SetFileName(filePathFixed.toLocal8Bit().constData());
  writer1->SetUseCompression(true);
  writer1->SetInput(this->m_parent->m_spFixedImg);

  auto writer2 = writerType::New();
  writer2->SetFileName(filePathMoving.toLocal8Bit().constData());
  writer2->SetUseCompression(true);
  writer2->SetInput(this->m_parent->m_spMovingImg);

  writer1->Update();
  writer2->Update();
  // In Andreases code this was checked so we make it true (checkBoxUseROIForRigid)
  if (true){// True (see checkBoxUseROIForRigid)
    std::cout << "Creating a ROI mask for Rigid registration " << std::endl;
    auto strFOVGeom = QString("0.0,0.0,190.0");//Hardcoded value (from lineEditFOVPos)

    auto strListFOV = strFOVGeom.split(",");
    if (strListFOV.count() == 3) {
      const auto FOV_DcmPosX = strListFOV.at(0).toFloat(); // mm
      const auto FOV_DcmPosY = strListFOV.at(1).toFloat();
      const auto FOV_Radius = strListFOV.at(2).toFloat();

      // Create Image using FixedImage sp

      // Image Pointer here
      UShortImageType::Pointer spRoiMask;
      AllocateByRef<UShortImageType, UShortImageType>(m_parent->m_spFixedImg, spRoiMask);
      this->m_cbctrecon->GenerateCylinderMask(spRoiMask, FOV_DcmPosX,
                                                   FOV_DcmPosY, FOV_Radius);

      auto writer3 = writerType::New();
      writer3->SetFileName(filePathROI.toLocal8Bit().constData());
      writer3->SetUseCompression(true);
      writer3->SetInput(spRoiMask);
      writer3->Update();
    }
    std::cout << "2.A:  ROI-based Rigid body registration will be done"
              << std::endl;
  } else {
    filePathROI = QString("");
  }
  emit Signal_UpdateProgressBarSC(20);
  // Preprocessing
  // 2) move CT-based skin mask on CBCT based on manual shift
  //  if (m_strPathCTSkin)

  // m_strPathCTSkin is prepared after PreProcessCT()

  std::cout << "2: Creating a plastimatch command file" << std::endl;

  const auto fnCmdRegisterRigid = QString("cmd_register_rigid.txt");
  auto pathCmdRegister =
      m_cbctregistration->m_strPathPlastimatch + "/" + fnCmdRegisterRigid;

  const auto strDummy = QString("");

  auto mse = true;
  if(m_parent->RegionChosen==0){
      mse = true;
  }else{
      mse = false;
  }

  auto cuda = false;
  if(this->m_parent->m_UseCUDA){
      cuda = true;
  }
  if(this->m_parent->m_UseOpenCL){
      cuda = false;
  }
  auto GradOptionStr = QString("0.7,0.7,0.7");//Hardcoded value (from lineEditGradOption)
  // For Cropped patients, FOV mask is applied.

  m_cbctregistration->GenPlastiRegisterCommandFile(
      pathCmdRegister, filePathFixed, filePathMoving, filePathOutput,
      filePathXform, PLAST_RIGID, strDummy, strDummy, strDummy, filePathROI,
      mse, cuda, GradOptionStr);

  std::string str_command_filepath = pathCmdRegister.toLocal8Bit().constData();

  m_cbctregistration->CallingPLMCommand(str_command_filepath);

  std::cout << "5: Reading output image-CT" << std::endl;

  auto reader = itk::ImageFileReader<UShortImageType>::New();
  reader->SetFileName(filePathOutput.toLocal8Bit().constData());
  reader->Update();
  this->m_cbctrecon->m_spAutoRigidCT = reader->GetOutput();

  std::cout << "6: Reading is completed" << std::endl;

  const QFile xform_file(filePathXform);

  // If rigid structure already exists copy it to plan-ct structure before
  // applying rigid again.
  if (!m_cbctregistration->m_pParent->m_structures->is_ss_null<RIGID_CT>()) {
    auto rigid_ss = std::make_unique<Rtss_modern>(
        *(m_cbctregistration->m_pParent->m_structures->get_ss(RIGID_CT)));
    m_cbctregistration->m_pParent->m_structures->set_planCT_ss(
        std::move(rigid_ss));
  }

  const auto transform_success =
      m_cbctregistration->m_pParent->m_structures->ApplyTransformTo<PLAN_CT>(
          xform_file);
  if (transform_success) {
    std::cout << "7: Contours registered" << std::endl;
  }

  m_cbctregistration->m_strPathXFAutoRigid = filePathXform; // for further use
  emit Signal_UpdateProgressBarSC(30);
  SLT_DoRegistrationDeform();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_MovingImageSelected
ctType ScatterCorrectingThread::get_ctType(const QString &selText) { // Returns CT type
  if (selText.compare("REF_CT") == 0) {
    return PLAN_CT;
  }
  if (selText.compare("MANUAL_RIGID_CT") == 0 ||
      selText.compare("AUTO_RIGID_CT") == 0) {
    return RIGID_CT;
  }
  if (selText.compare("DEFORMED_CT1") == 0 ||
      selText.compare("DEFORMED_CT2") == 0 ||
      selText.compare("DEFORMED_CT3") == 0 ||
      selText.compare("DEFORMED_CT_FINAL") == 0) {
    return DEFORM_CT;
  }
  return PLAN_CT;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by set_points_by_slice()
template <enCOLOR color> auto get_qtpoint_vector(qyklabel *window) { // Sets color for WEPL
  switch (color) {
  case RED:
    return &window->m_vPt;
  case GREEN:
  default:
    return &window->m_vPt_green;
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Is called by SLT_DrawImageWhenSliceChange()
template <typename ImageBase, enPLANE plane, enCOLOR color>
auto set_points_by_slice(qyklabel *window, Rtss_roi_modern *voi, // Sets points for WEPL
                         std::array<double, 3> curPhysPos,
                         typename ImageBase::SpacingType imgSpacing,
                         typename ImageBase::PointType imgOriginFixed,
                         typename ImageBase::SizeType imgSize) {
  auto *Wnd_contour = get_qtpoint_vector<color>(window);
  Wnd_contour->clear();

  auto wnd_size = window->size();
  auto wnd_height = wnd_size.rheight();
  const auto wnd_width = wnd_size.rwidth();
  auto x_scale = 1.0 / static_cast<double>(wnd_width);
  auto y_scale = 1.0 / static_cast<double>(wnd_height);
  switch (plane) {
  case PLANE_AXIAL:
    x_scale *= imgSpacing[0] * imgSize[0];
    y_scale *= imgSpacing[1] * imgSize[1];
    break;
  case PLANE_FRONTAL:
    x_scale *= imgSpacing[0] * imgSize[0];
    y_scale *= imgSpacing[2] * imgSize[2];
    break;
  case PLANE_SAGITTAL:
    x_scale *= imgSpacing[1] * imgSize[1];
    y_scale *= imgSpacing[2] * imgSize[2];
  }

  for (auto contour : voi->pslist) {
    if (contour.coordinates.empty()) {
      continue;
    }
    const auto first_point = contour.coordinates.at(0);
    // Axial
    if (first_point.z > curPhysPos[0] - imgSpacing[2] &&
        first_point.z < curPhysPos[0] + imgSpacing[2] && plane == PLANE_AXIAL) {
      for (auto point : contour.coordinates) {
        Wnd_contour->emplace_back((point.x - imgOriginFixed[0]) / x_scale,
                                  (point.y - imgOriginFixed[1]) / y_scale);
      }
    }
    for (auto &point : contour.coordinates) {
      // Frontal
      if (point.y > curPhysPos[1] - imgSpacing[1] &&
          point.y < curPhysPos[1] + imgSpacing[1] && plane == PLANE_FRONTAL) {
        Wnd_contour->emplace_back((point.x - imgOriginFixed[0]) / x_scale,
                                  wnd_height -
                                      (point.z - imgOriginFixed[2]) / y_scale);
      }
      // Sagittal
      if (point.x > curPhysPos[2] - imgSpacing[0] &&
          point.x < curPhysPos[2] + imgSpacing[0] && plane == PLANE_SAGITTAL) {
        Wnd_contour->emplace_back((point.y - imgOriginFixed[1]) / x_scale,
                                  wnd_height -
                                      (point.z - imgOriginFixed[2]) / y_scale);
      }
    }
  }

  window->m_bDrawPoints = true;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void ScatterCorrectingThread::SLT_DoRegistrationDeform() {
  if (m_cbctrecon->m_spRawReconImg == nullptr || m_cbctrecon->m_spRefCTImg == nullptr) {
    return;
  }
  emit Signal_FixedImageSelected(QString("RAW_CBCT"));
  emit Signal_MovingImageSelected(QString("AUTO_RIGID_CT"));
  const auto bPrepareMaskOnly = !true;// True because we want to crop (from checkBoxCropBkgroundCT)

  std::cout << "0: DoRegistrationDeform: writing temporary files" << std::endl;

  // Both image type: Unsigned Short
  auto filePathFixed =
      m_cbctregistration->m_strPathPlastimatch + "/" + "fixed_deform.mha";
  auto filePathMoving =
      m_cbctregistration->m_strPathPlastimatch + "/" + "moving_deform.mha";
  auto filePathROI = m_cbctregistration->m_strPathPlastimatch + "/" +
                     "fixed_roi_DIR.mha"; // optional

  const auto filePathOutput =
      m_cbctregistration->m_strPathPlastimatch + "/" + "output_deform.mha";
  const auto filePathXform =
      m_cbctregistration->m_strPathPlastimatch + "/" + "xform_deform.txt";

  auto filePathOutputStage1 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage1.mha";
  auto filePathOutputStage2 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage2.mha";
  auto filePathOutputStage3 = m_cbctregistration->m_strPathPlastimatch + "/" +
                              "output_deform_stage3.mha";

  using writerType = itk::ImageFileWriter<UShortImageType>;

  auto writer1 = writerType::New();
  writer1->SetFileName(filePathFixed.toLocal8Bit().constData());
  writer1->SetUseCompression(true);
  writer1->SetInput(m_parent->m_spFixedImg);

  auto writer2 = writerType::New();
  writer2->SetFileName(filePathMoving.toLocal8Bit().constData());
  writer2->SetUseCompression(true);
  writer2->SetInput(m_parent->m_spMovingImg);
  writer1->Update();
  writer2->Update();
  emit Signal_UpdateProgressBarSC(40);
  // Create a mask image based on the fixed sp image
  if (true){//True (see checkBoxUseROIForDIR)
    std::cout << "Creating a ROI mask for DIR.. " << std::endl;

    auto strFOVGeom = QString("0.0,0.0,190.0");//Hardcoded value (from lineEditFOVPos)

    auto strListFOV = strFOVGeom.split(",");
    if (strListFOV.count() == 3) {
      const auto FOV_DcmPosX = strListFOV.at(0).toFloat(); // mm
      const auto FOV_DcmPosY = strListFOV.at(1).toFloat();
      const auto FOV_Radius = strListFOV.at(2).toFloat();

      // Create Image using FixedImage sp

      // Image Pointer here
      UShortImageType::Pointer spRoiMask;
      AllocateByRef<UShortImageType, UShortImageType>(m_parent->m_spFixedImg, spRoiMask);
      m_cbctregistration->m_pParent->GenerateCylinderMask(
          spRoiMask, FOV_DcmPosX, FOV_DcmPosY, FOV_Radius);

      auto writer3 = writerType::New();
      writer3->SetFileName(filePathROI.toLocal8Bit().constData());
      writer3->SetUseCompression(true);
      writer3->SetInput(spRoiMask);
      writer3->Update();
    }
  }

  auto filePathFixed_proc = filePathFixed;

  std::cout << "1: DoRegistrationDeform: CBCT pre-processing before deformable "
               "registration"
            << std::endl;

  QFileInfo info1(m_cbctregistration->m_strPathCTSkin_autoRegi);
  QFileInfo info2(m_cbctregistration->m_strPathXFAutoRigid);

  if (!info1.exists() || !info2.exists()) {
    std::cout << "Fatal error! no CT skin is found or no XF auto file found. "
                 "Preprocessing will not be done. Proceeding."
              << std::endl;
  } else {
    filePathFixed_proc = m_cbctregistration->m_strPathPlastimatch + "/" +
                         "fixed_deform_proc.mha";
    // skin removal and bubble filling : output file = filePathFixed_proc
    const auto bBubbleRemoval = false;//False (see checkBoxFillBubbleCT)
    const auto skinExp = 8.0;//Hardcoded value (from lineEditCBCTSkinCropBfDIR)

    const auto iBubThresholdUshort = 500;//Hardcoded value (from spinBoxBkDetectCT)
    const auto iBubFillUshort = 1000;//Hardcoded value (from spinBoxBubFillCT)

    m_cbctregistration->ProcessCBCT_beforeDeformRegi(
        filePathFixed, m_cbctregistration->m_strPathCTSkin_autoRegi,
        filePathFixed_proc, m_cbctregistration->m_strPathXFAutoRigid,
        bBubbleRemoval, bPrepareMaskOnly, skinExp, iBubThresholdUshort,
        iBubFillUshort); // bubble filling yes

    if (bPrepareMaskOnly) {
      filePathFixed_proc = filePathFixed;
    }
  }

  std::cout << "2: DoRegistrationDeform: Creating a plastimatch command file"
            << std::endl;
  emit Signal_UpdateProgressBarSC(50);
  const auto fnCmdRegisterRigid = QString("cmd_register_deform.txt");
  auto pathCmdRegister =
      m_cbctregistration->m_strPathPlastimatch + "/" + fnCmdRegisterRigid;

  auto strDeformableStage1 = QString("2,2,1,30,0.00001,0.005,20");//Hardcoded value (from lineEditArgument1), original param: 7, add output path
  auto strDeformableStage2 = QString("");//See lineEditArgument2
  auto strDeformableStage3 = QString("");//See lineEditArgument3

  strDeformableStage1.append(", ").append(filePathOutputStage1);
  strDeformableStage2.append(", ").append(filePathOutputStage2);
  strDeformableStage3.append(", ").append(filePathOutputStage3);

  auto mse = true;
  if(m_parent->RegionChosen == 0){
      mse = true;
  }else{
      mse = false;
  }
  auto cuda = false;
  if(this->m_parent->m_UseCUDA){
       cuda = true;
  }
  if(this->m_parent->m_UseOpenCL){
       cuda = false;
  }
  auto GradOptionStr = QString("0.7,0.7,0.7");//Hardcoded value (from lineEditGradOption)
  // For Cropped patients, FOV mask is applied.
  if (true){//See checkBoxUseROIForDIR
    m_cbctregistration->GenPlastiRegisterCommandFile(
        pathCmdRegister, filePathFixed_proc, filePathMoving, filePathOutput,
        filePathXform, PLAST_BSPLINE, strDeformableStage1, strDeformableStage2,
        strDeformableStage3, filePathROI, mse, cuda, GradOptionStr);
  } else {
    m_cbctregistration->GenPlastiRegisterCommandFile(
        pathCmdRegister, filePathFixed_proc, filePathMoving, filePathOutput,
        filePathXform, PLAST_BSPLINE, strDeformableStage1, strDeformableStage2,
        strDeformableStage3, QString(), mse, cuda, GradOptionStr);
  }

  std::string str_command_filepath = pathCmdRegister.toLocal8Bit().constData();

  m_cbctregistration->CallingPLMCommand(str_command_filepath);

  std::cout << "5: DoRegistrationDeform: Registration is done" << std::endl;
  std::cout << "6: DoRegistrationDeform: Reading output image" << std::endl;

  using readerType = itk::ImageFileReader<UShortImageType>;

  auto readerDefSt1 = readerType::New();

  auto tmpFileInfo = QFileInfo(filePathOutputStage1);
  if (tmpFileInfo.exists()) {
    readerDefSt1->SetFileName(filePathOutputStage1.toLocal8Bit().constData());
    readerDefSt1->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT1 = readerDefSt1->GetOutput();
  }

  auto readerDefSt2 = readerType::New();
  tmpFileInfo = QFileInfo(filePathOutputStage2);
  if (tmpFileInfo.exists()) {
    readerDefSt2->SetFileName(filePathOutputStage2.toLocal8Bit().constData());
    readerDefSt2->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT2 = readerDefSt2->GetOutput();
  }

  auto readerDefSt3 = readerType::New();
  tmpFileInfo = QFileInfo(filePathOutputStage3);
  if (tmpFileInfo.exists()) {
    readerDefSt3->SetFileName(filePathOutputStage3.toLocal8Bit().constData());
    readerDefSt3->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT3 = readerDefSt3->GetOutput();
  }

  auto readerFinCBCT = readerType::New();
  tmpFileInfo = QFileInfo(filePathFixed_proc); // cropped image Or not
  if (tmpFileInfo.exists()) {
    readerFinCBCT->SetFileName(filePathFixed_proc.toLocal8Bit().constData());
    readerFinCBCT->Update();
    m_cbctregistration->m_pParent->m_spRawReconImg = readerFinCBCT->GetOutput();
  } else {
    std::cout << "No filePathFixed_proc is available. Exit the function"
              << std::endl;
    return;
  }
  // Puncturing should be done for final Deform image
  auto strPathDeformCTFinal = filePathOutput;

  QFileInfo tmpBubFileInfo(m_cbctregistration->m_strPathMskCBCTBubble);

  if (false && tmpBubFileInfo.exists()) {//False (see checkBoxFillBubbleCT)
    std::cout << "6B: final puncturing according to the CBCT bubble"
              << std::endl;

    strPathDeformCTFinal =
        m_cbctregistration->m_strPathPlastimatch + "/deformCTpuncFin.mha";

    const auto enMaskOp = MASK_OPERATION_FILL;
    auto input_fn = filePathOutput;
    auto mask_fn = m_cbctregistration->m_strPathMskCBCTBubble;
    auto output_fn = strPathDeformCTFinal;

    // soft tissue
    const auto iBubblePunctureVal = 0; // 0 = air. deformed CT is now already a USHORT image
    const auto mask_value = iBubblePunctureVal;
    m_cbctregistration->plm_mask_main(enMaskOp, input_fn, mask_fn, output_fn,
                                      static_cast<float>(mask_value));
  }

  auto readerDeformFinal = readerType::New();
  tmpFileInfo = QFileInfo(strPathDeformCTFinal);
  if (tmpFileInfo.exists()) {
    readerDeformFinal->SetFileName(
        strPathDeformCTFinal.toLocal8Bit().constData());
    readerDeformFinal->Update();
    m_cbctregistration->m_pParent->m_spDeformedCT_Final =
        readerDeformFinal->GetOutput();
  } else {
    std::cout << "No final output is available. Exit the function" << std::endl;
    return;
  }

  std::cout << "7: DoRegistrationDeform: Reading is completed" << std::endl;

  const QFile xform_file(filePathXform);
  m_cbctregistration->m_pParent->m_structures->ApplyTransformTo<RIGID_CT>(
      xform_file);

  std::cout << "8: Contours deformed" << std::endl;

  std::cout << "FINISHED!: Deformable image registration. Proceed to scatter "
               "correction"
            << std::endl;

  SLT_IntensityNormCBCT();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void ScatterCorrectingThread::SLT_IntensityNormCBCT() { // Normalising CBCT
  emit Signal_FixedImageSelected(QString("RAW_CBCT"));
  emit Signal_MovingImageSelected(QString("DEFORMED_CT_FINAL"));
  emit Signal_UpdateProgressBarSC(60);
  const auto fROI_Radius = 30;//Hardcoded value (from lineEditNormRoiRadius)

  std::cout << "Intensity is being analyzed...Please wait." << std::endl;

  float intensitySDFix = 0.0;
  float intensitySDMov = 0.0;
  const auto meanIntensityFix = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spRawReconImg, fROI_Radius, &intensitySDFix);
  const auto meanIntensityMov = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spDeformedCT_Final, fROI_Radius, &intensitySDMov);

  std::cout << "Mean/SD for Fixed = " << meanIntensityFix << "/"
            << intensitySDFix << std::endl;
  std::cout << "Mean/SD for Moving = " << meanIntensityMov << "/"
            << intensitySDMov << std::endl;

  AddConstHU(m_cbctrecon->m_spRawReconImg, static_cast<int>(meanIntensityMov - meanIntensityFix));

  std::cout << "Intensity shifting is done! Added value = "
            << static_cast<int>(meanIntensityMov - meanIntensityFix)
            << std::endl;
  auto update_message =
      QString("Added_%1")
          .arg(static_cast<int>(meanIntensityMov - meanIntensityFix));

  emit Signal_UpdateLabelCor(QString("Norm Raw CBCT"));
  emit Signal_FixedImageSelected(QString("RAW_CBCT"));

  SLT_DoScatterCorrection_APRIORI();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void ScatterCorrectingThread::SLT_IntensityNormCBCT_COR_CBCT() { // Normalises the Corrected CBCT
  emit Signal_FixedImageSelected(QString("COR_CBCT"));
  emit Signal_MovingImageSelected(QString("DEFORMED_CT_FINAL"));
  const auto fROI_Radius = 30;//Hardcoded value (from lineEditNormRoiRadius)
  std::cout << "Intensity is being analyzed...Please wait." << std::endl;

  float intensitySDFix = 0.0;
  float intensitySDMov = 0.0;
  const auto meanIntensityFix = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spScatCorrReconImg, fROI_Radius, &intensitySDFix);
  const auto meanIntensityMov = m_cbctregistration->m_pParent->GetMeanIntensity(
      m_cbctrecon->m_spDeformedCT_Final, fROI_Radius, &intensitySDMov);

  std::cout << "Mean/SD for Fixed = " << meanIntensityFix << "/"
            << intensitySDFix << std::endl;
  std::cout << "Mean/SD for Moving = " << meanIntensityMov << "/"
            << intensitySDMov << std::endl;

  AddConstHU(m_cbctrecon->m_spScatCorrReconImg, static_cast<int>(meanIntensityMov - meanIntensityFix));

  std::cout << "Intensity shifting is done! Added value = "
            << static_cast<int>(meanIntensityMov - meanIntensityFix)
            << std::endl;
  auto update_message =
      QString("Added_%1")
          .arg(static_cast<int>(meanIntensityMov - meanIntensityFix));

  emit Signal_FixedImageSelected(QString("RAW_CBCT"));
  emit Signal_MovingImageSelected(QString("RAW_CBCT"));
  emit Signal_PassFixedImg();
  emit Signal_UpdateLabelRaw(QString("Norm Raw CBCT"));
  emit Signal_FixedImageSelected(QString("COR_CBCT"));
  emit Signal_MovingImageSelected(QString("COR_CBCT"));
  emit Signal_UpdateLabelCor(QString("Norm Cor CBCT"));
  const auto cur_ct2 = get_ctType("COR_CBCT");
  emit Signal_UpdateVOICombobox(cur_ct2);
  emit Signal_UpdateProgressBarSC(100);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
// In Andreases code this method is called when Scatter correct button is pushed. Find out where to implement this!
void ScatterCorrectingThread::SLT_DoScatterCorrection_APRIORI() { // Scatter correcting method
  emit Signal_UpdateProgressBarSC(70);
  if ((this->m_cbctrecon->m_spRefCTImg == nullptr &&
       m_parent->m_spMovingImg == nullptr) ||
      m_cbctrecon->m_spRawReconImg ==nullptr){
    std::cerr
        << "Error!: No ref or no fixed image for forward projection is found."
        << "\n";
    return;
  }

  const auto bExportProj_Fwd = false;//False (see checkBox_ExportFwd)
  const auto bExportProj_Scat = false;//False (see checkBox_ExportScat)
  const auto bExportProj_Cor = false;//False (see checkBox_ExportCor)

  std::cerr << "Custom Geometry: \n";
  m_cbctrecon->m_spCustomGeometry->Print(std::cerr);
  auto spProjImg3DFloat = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
              m_parent->m_spFixedImg, this->m_cbctrecon->m_spCustomGeometry,
              bExportProj_Fwd, false);
  if(this->m_parent->m_UseCUDA){
      spProjImg3DFloat =
          this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
              m_parent->m_spFixedImg, this->m_cbctrecon->m_spCustomGeometry,
              bExportProj_Fwd, true);
  }
  if(this->m_parent->m_UseOpenCL){
      spProjImg3DFloat =
          this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
              m_parent->m_spFixedImg, this->m_cbctrecon->m_spCustomGeometry,
              bExportProj_Fwd, false);
  }
  emit Signal_UpdateProgressBarSC(80);
  FloatImageType::Pointer p_projimg;
  if (m_parent->m_spMovingImg != nullptr){
      if(this->m_parent->m_UseCUDA){
          p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
              m_parent->m_spMovingImg, this->m_cbctrecon->m_spCustomGeometry,
              bExportProj_Fwd,
              true);
      }
      if(this->m_parent->m_UseOpenCL){
          p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
              m_parent->m_spMovingImg, this->m_cbctrecon->m_spCustomGeometry,
              bExportProj_Fwd,
              false);
      }

  } else if (this->m_cbctrecon->m_spRefCTImg != nullptr) {
    std::cerr << "No Moving image in Registration is found. Ref CT image will "
                 "be used instead"
              << "\n";
    if(this->m_parent->m_UseCUDA){
        p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
            this->m_cbctrecon->m_spRefCTImg, this->m_cbctrecon->m_spCustomGeometry,
            bExportProj_Fwd,
            true); // final moving image
    }
    if(this->m_parent->m_UseOpenCL){
        p_projimg = this->m_cbctrecon->ForwardProjection_master<UShortImageType>(
            this->m_cbctrecon->m_spRefCTImg, this->m_cbctrecon->m_spCustomGeometry,
            bExportProj_Fwd,
            false); // final moving image
    }
  }

  // YKTEMP
  auto projsize = p_projimg->GetBufferedRegion().GetSize();
  std::cout << "ProjImgCT Size = " << projsize[0] << ", " << projsize[1] << ", "
            << projsize[2] << "\n";
  std::cout << "ProjImgCT origin = " << p_projimg->GetOrigin()[0] << ", "
            << p_projimg->GetOrigin()[1] << ", " << p_projimg->GetOrigin()[2]
            << "\n";
  std::cout << "ProjImgCT spacing = " << p_projimg->GetSpacing()[0] << ", "
            << p_projimg->GetSpacing()[1] << ", " << p_projimg->GetSpacing()[2]
            << std::endl;

  const auto scaMedian = 12.0;//Hardcoded value (from lineEdit_scaMedian)
  const auto scaGaussian = 1.5;//Hardcoded value (from lineEdit_scaGaussian)
  std::cout << "Generating scatter map is ongoing..." << std::endl;

  this->m_cbctrecon->GenScatterMap_PriorCT(
      spProjImg3DFloat, p_projimg, this->m_cbctrecon->m_spProjImgScat3D,
      scaMedian, scaGaussian,
      bExportProj_Scat);

  std::cout << "To account for the mAs values, the intensity scale factor of "
            << GetRawIntensityScaleFactor(this->m_cbctrecon->m_strRef_mAs,
                                          this->m_cbctrecon->m_strCur_mAs)
            << "was multiplied during scatter correction to avoid negative "
               "scatter"
            << std::endl;

  p_projimg->Initialize(); // memory saving

  std::cout << "Scatter correction is in progress..." << std::endl;

  const auto postScatMedianSize = 3;//Hardcoded value (from lineEdit_scaPostMedian)

  this->m_cbctrecon->ScatterCorr_PrioriCT(spProjImg3DFloat,
                                          this->m_cbctrecon->m_spProjImgScat3D,
                                          this->m_cbctrecon->m_spProjImgCorr3D,
                                          postScatMedianSize, bExportProj_Cor);

  this->m_cbctrecon->m_spProjImgScat3D->Initialize(); // memory saving
  emit Signal_UpdateProgressBarSC(90);
  std::cout << "AfterCorrectionMacro is ongoing..." << std::endl;

  this->m_cbctrecon->SetMaxAndMinValueOfProjectionImage(); // update min max projection image

  auto fdk_options = getFDKoptions();
  if(this->m_parent->m_UseCUDA){
      this->m_cbctrecon->AfterScatCorrectionMacro(true,false,false,fdk_options);
  }
  if(this->m_parent->m_UseOpenCL){
      this->m_cbctrecon->AfterScatCorrectionMacro(false,true,false,fdk_options);
  }

  // Skin removal (using CT contour w/ big margin)
  std::cout
      << "Post  FDK reconstruction is done. Moving on to post skin removal"
      << std::endl;

  const auto voi_name = QString("BODY");//this->m_dlgRegistration->ui.comboBox_VOItoCropBy->currentText();

  // 20151208 Removal of high intensity skin mask
  // Main issue: raw CBCT projection includes mask, deformed CT doesn't include
  // mask. In case of weight loss, mask signal is independent from skin contour,
  // but deformed CT cannot have that signal.  Therefore, after the subtraction
  // (CBCTcor projections), there is always a big peak. DIR quality doesn't
  // matter because it cannot 'create' mask signal anyway.  Assumption: near the
  // skin contour, this kind of discrepancy is not expected.
  // m_pDlgRegistration->ThermoMaskRemovingCBCT(m_spRawReconImg,
  // m_spScatCorrReconImg, threshold_HU);

  emit Signal_UpdateLabelCor(QString("Cor CBCT"));
  emit Signal_FixedImageSelected("RAW_CBCT");
  emit Signal_MovingImageSelected("COR_CBCT");

  std::cout << "Updating ReconImage..";
  auto updated_text = QString("Scatter corrected CBCT");

  std::cout << "FINISHED!Scatter correction: CBCT DICOM files are saved"
            << std::endl;

  SLT_IntensityNormCBCT_COR_CBCT();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
FDK_options ScatterCorrectingThread::getFDKoptions() const { // FDK options
    FDK_options fdk_options;
    fdk_options.TruncCorFactor = 1.0;//Hardcoded value(from trail and error, see lineEdit_Ramp_TruncationCorrection)
    fdk_options.HannCutX = 5.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_HannCut)
    fdk_options.HannCutY = 5.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_HannCutY)
    fdk_options.CosCut = 0.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_CosineCut)
    fdk_options.HammCut = 0.0;//Hardcoded value (from trail and error, see lineEdit_Ramp_Hamming)
    fdk_options.displacedDetectorFilter = true;//Hardcoded value (from trail and error, see checkBox_UseDDF)
    fdk_options.updateAfterDDF = false;//Hardcoded value (from trail and error, see checkBox_UpdateAfterFiltering)
    fdk_options.ParkerShortScan = true;//Hardcoded value (from trail and error, see checkBox_UsePSSF)
    fdk_options.ct_spacing[0] = 1;//Hardcoded value (from trail and error, see lineEdit_outImgSp_AP)
    fdk_options.ct_spacing[1] = 1;//Hardcoded value (from trail and error, see lineEdit_outImgSp_SI)
    fdk_options.ct_spacing[2] = 1;//Hardcoded value (from trail and error, see lineEdit_outImgSp_LR)
    fdk_options.ct_size[0] = 400;//Hardcoded value (from trail and error, see lineEdit_outImgDim_AP)
    fdk_options.ct_size[1] = 200;//Hardcoded value (from trail and error, see lineEdit_outImgDim_SI)
    fdk_options.ct_size[2] = 400;//Hardcoded value (from trail and error, see lineEdit_outImgDim_LR)
    fdk_options.medianRadius[0] = 0;//Hardcoded value, radius along x (from trail and error, see lineEdit_PostMedSizeX)
    fdk_options.medianRadius[1] = 0;//Hardcoded value, radius along y (from trail and error, see lineEdit_PostMedSizeY)
    fdk_options.medianRadius[2] = 1;//Hardcoded value, radius along z (from trail and error, see lineEdit_PostMedSizeZ)
    fdk_options.medianFilter = true;//Hardcoded value (from trail and error, see checkBox_PostMedianOn)
    fdk_options.outputFilePath = QString("");//Hardcoded value (from trail and error, see lineEdit_OutputFilePath)
    return fdk_options;
}


