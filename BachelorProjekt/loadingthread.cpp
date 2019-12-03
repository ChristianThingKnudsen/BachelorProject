#include "loadingthread.h"

#include <QThread>
#include <QtCore>
#include <qfiledialog.h>
#include "mha_io.h"
#include "cbctrecon_io.h"
#include "scui.h"

LoadingThread::LoadingThread(Scui *parent) : QThread(dynamic_cast<QObject*>(parent))//Construtor
{
    this->m_parent = parent; // Object of scui (Same object)
    this->m_cbctrecon = parent->m_cbctrecon.get(); // Object of cbctrecon (Same object)
    this->m_cbctregistration = parent->m_cbctregistration.get(); // Object of cbctregistration (Same object)
}

void LoadingThread::run(){ // Method which is called when the start method is called on the thread
    this->SLT_SetHisDir();
    this->SLT_LoadSelectedProjFiles(m_parent->CBCTPath);
}

void LoadingThread::SLT_SetHisDir() // Initialize all image buffer
{
  // Initializing..
  auto dirPath = m_parent->CBCTPath; // Path for CBCT projections

  if (dirPath.length() <= 1) { // If no directory chosen, jump out
    return;
  }

  auto tmp_dcm_uid = this->m_cbctrecon->m_strDCMUID;
  this->m_cbctrecon->SetProjDir(dirPath);
  init_DlgRegistration(tmp_dcm_uid);
  this->m_cbctrecon->m_strDCMUID = tmp_dcm_uid;

  float kVp = 0.0;
  float mA = 0.0;
  float ms = 0.0;
  GetXrayParamFromINI(this->m_cbctrecon->m_strPathElektaINI, kVp, mA, ms);

  VEC3D couch_trans = {-999, -999,
                       -999}; // mm. In the text file, these values are in cm.
  VEC3D couch_rot = {-999, -999,
                     -999}; // mm. In the text file, these values are in cm.

  const auto res = GetCouchShiftFromINIXVI(
      this->m_cbctrecon->m_strPathElektaINIXVI2, &couch_trans, &couch_rot);

  if (res) {
    const auto strTransX = QString::number(couch_trans.x, 'f', 1);
    const auto strTransY = QString::number(couch_trans.y, 'f', 1);
    const auto strTransZ = QString::number(couch_trans.z, 'f', 1);
    const auto strTransAll = strTransX + "," + strTransY + "," + strTransZ;

    const auto strRotX = QString::number(couch_rot.x, 'f', 1);
    const auto strRotY = QString::number(couch_rot.y, 'f', 1);
    const auto strRotZ = QString::number(couch_rot.z, 'f', 1);

    const auto strRotAll = strRotX + "," + strRotY + "," + strRotZ;
  }

  this->m_cbctrecon->m_vSelectedFileNames.clear();

  std::cout << "Push Load button to load projection images" << std::endl;
}

void LoadingThread::init_DlgRegistration(QString &str_dcm_uid) const// init dlgRegistration. Sets pointers en scui to zero
{
    QString strDCMUID = str_dcm_uid;
    m_cbctregistration->SetPlmOutputDir(strDCMUID);

    const UShortImageType::Pointer spNull;
    // unlink all of the pointers
    auto &p_parent = m_cbctrecon;
    p_parent->m_spRefCTImg = spNull;
    p_parent->m_spManualRigidCT =
        spNull; // copied from RefCTImg; ID: RefCT --> Moving Img, cloned
    p_parent->m_spAutoRigidCT = spNull; // ID: AutoRigidCT
    p_parent->m_spDeformedCT1 = spNull; // Deformmation will be carried out based
                                        // on Moving IMage of GUI //AutoDeformCT1
    p_parent->m_spDeformedCT2 = spNull; // AutoDeformCT2
    p_parent->m_spDeformedCT3 = spNull; // AutoDeformCT3
    p_parent->m_spDeformedCT_Final = spNull; // AutoDeformCT3

}
//Is needed for the next method SLT_LoadSelectedProjFiles()
std::tuple<bool, bool> LoadingThread::probeUser(const QString) {
  auto dirPath = m_parent->CTPath;

  auto dcm_success = false;
  if (!(dirPath.length() <= 1)) {

    if (this->m_cbctrecon->ReadDicomDir(dirPath)) {

      this->m_cbctrecon->RegisterImgDuplication(REGISTER_REF_CT,
                                                REGISTER_MANUAL_RIGID);
      dcm_success = true;
    }
  }
  auto instaRecon = true;
  return std::make_tuple(instaRecon, dcm_success);
}

FilterReaderType::Pointer
LoadingThread::ReadBowtieFileWhileProbing(const QString &proj_path,
                                            std::tuple<bool, bool> &answers) { // Function for reading the bowtie filter

  auto bowtiereader =
      FilterReaderType::New(); // we use is because we need the projections to
                               // be in the same unit (order of magnitude)

  QDir guessDir(proj_path + QString("/../"));

  const auto calDir(proj_path + QString("/Calibrations/"));

  QString bowtiePath;

  switch (this->m_cbctrecon->m_projFormat) {
  case XIM_FORMAT:
    //Hardcoded value
    bowtiePath = proj_path+QString("\\Calibrations\\AIR-Full-Bowtie-100KV-Scattergrid-SAD-SID_0\\Current\\FilterBowtie.xim"); // Hardcoded value
    std::cout << bowtiePath.toStdString() << std::endl;
    if (bowtiePath.length() > 1) {
      std::cout << "loading bowtie-filter..." << std::endl;
      std::vector<std::string> filepath;
      filepath.push_back(bowtiePath.toStdString());
      bowtiereader->SetFileNames(filepath);
      emit Signal_UpdateProgressBarLoad(20); // Signal to update the progressbar on scui
      std::thread calc_thread_bowtie(
          [&bowtiereader] { bowtiereader->Update(); });
      answers = probeUser(guessDir.absolutePath());
      calc_thread_bowtie.join();
    } else {
      answers = probeUser(
          guessDir.absolutePath()); // looks ugly, but allows threading
    }
    break;
  default:
    answers = probeUser(guessDir.absolutePath()); // ^^^
    break;
  }
  if (bowtiePath.length() > 1) {
    return bowtiereader;
  }
  return nullptr;
}

void LoadingThread::SLT_LoadSelectedProjFiles(QString &path) // Main loading fuction for projection images
{
  auto dirPath = path;

  if (!QFile::exists(dirPath)) {
    std::cout << "Projection file directory was not found. Retry." << std::endl;
    return;
  }

  auto names = this->m_cbctrecon->GetProjFileNames(dirPath);

  if (this->m_cbctrecon->m_projFormat == HIS_FORMAT &&
      !this->m_cbctrecon->IsFileNameOrderCorrect(names)) {
    std::cout << "Check the file name order" << std::endl;
  }

  std::cout << "File name order was cross-checked and found to be OK!"
            << std::endl;

  const auto fullCnt = names.size();
  if (fullCnt <= 0) {
    std::cout << "No projection file was found. Retry." << std::endl;
    return;
  }

  std::cout << fullCnt << "  projection files were found." << std::endl;

  // 2) Elekta Geometry file
  const auto geomPath = this->m_cbctrecon->m_strPathGeomXML;// (from lineEdit_ElektaGeomPath)
  QFileInfo geomFileInfo(geomPath);
  //! QFile::exists(geomPath)

  if (!this->m_cbctrecon->LoadGeometry(geomFileInfo, names)) {
      if (!this->m_cbctrecon->m_strError.isEmpty()) {

      }
   }

  const auto &p_geometry = this->m_cbctrecon->m_spFullGeometry;
  const auto iFullGeoDataSize = p_geometry->GetGantryAngles().size();
  if (iFullGeoDataSize < 1) {
    std::cout << "Not enough projection image (should be > 0)" << std::endl;
    return;
  }

  if (iFullGeoDataSize != fullCnt) {
    if (this->m_cbctrecon->m_projFormat != XIM_FORMAT) {
      std::cout << "Size of geometry data and file numbers are not same! Check "
                   "and retry"
                << std::endl;
      return;
    }
  }

  auto angle_gaps = p_geometry->GetAngularGaps(p_geometry->GetSourceAngles());

  auto sum_gap =
      std::accumulate(std::begin(angle_gaps), std::end(angle_gaps), 0.0);
  sum_gap /= itk::Math::pi * 180.0;
  const auto mean_gap = sum_gap / angle_gaps.size();

  std::cout << "AngularGaps Sum (deg):" << sum_gap
            << ", Mean (deg): " << mean_gap << std::endl;
  double gantryAngelIntervalValue = 30; //Hardcoded value (from lineEdit_ManualProjAngleGap)
  const auto gantryAngleInterval = gantryAngelIntervalValue;

  const auto exclude_ids = this->m_cbctrecon->GetExcludeProjFiles(false, gantryAngleInterval);// false (see Radio_ManualProjAngleGap)

  this->m_cbctrecon->LoadSelectedProj(exclude_ids, names);

  // Reads the cone beam projections
  using ReaderType = rtk::ProjectionsReader<FloatImageType>;
  auto reader = ReaderType::New();
  reader->SetFileNames(this->m_cbctrecon->m_vSelectedFileNames);
  std::thread calc_thread([&reader]() { reader->Update(); });

  std::cout << "Reader detached from main thread" << std::endl;

  // After reading the whole file, HIS header should be saved
  this->m_cbctrecon->saveHisHeader();

  //  Insta Recon, Dcm read
  const auto geopath = geomFileInfo.absolutePath();
  std::tuple<bool, bool> answers;
  auto bowtie_reader = ReadBowtieFileWhileProbing(geopath, answers);

  calc_thread.join();
  emit Signal_UpdateProgressBarLoad(40); // Signal to update the progressbar on scui
  std::cout << "Reader re-attached to main thread" << std::endl;

  this->m_cbctrecon->m_spProjImg3DFloat =
      reader->GetOutput(); // 1024 1024, line integ image
  auto &proj = this->m_cbctrecon->m_spProjImg3DFloat;

  const auto bowtie_proj = bowtie_reader->GetOutput();
  if (bowtie_reader != nullptr) {
    ApplyBowtie(proj, bowtie_proj);
  }

  if (this->m_cbctrecon->m_projFormat == HND_FORMAT) {
    std::cout << "Fitted bowtie-filter correction ongoing..." << std::endl;
    SLT_DoBowtieCorrection();
  }

  saveImageAsMHA<FloatImageType>(this->m_cbctrecon->m_spProjImg3DFloat);
  auto res_factor = 0.5;//Hardcoded value (from lineEdit_DownResolFactor)
  if (!this->m_cbctrecon->ResampleProjections(res_factor)) { // 0.5
    // reset factor if image was not resampled
    res_factor =1.0;// (from lineEdit_DownResolFactor)
  }

  this->m_cbctrecon->m_spProjImgRaw3D =
      this->m_cbctrecon->ConvertLineInt2Intensity_ushort(
          this->m_cbctrecon->m_spProjImg3DFloat);

  this->m_cbctrecon
      ->SetMaxAndMinValueOfProjectionImage(); // update min max projection image

  emit Signal_UpdateProgressBarLoad(60); // Signal to update the progressbar on scui
  if (!std::get<0>(answers)) { // instaRecon
    std::cout
        << "FINISHED!: Loading projection files. Proceed to reconstruction"
        << std::endl;
  } else {
    SLT_DoReconstruction();
  }
}
// Is implemented in SLT_LoadSelectedProjFiles()
void LoadingThread::SLT_DoBowtieCorrection() { // Function for applying the bowtiefilter on the raw projections.
  if (this->m_cbctrecon->m_spProjImg3DFloat == nullptr) {
    return;
  }

  if (this->m_cbctrecon->m_projFormat != HND_FORMAT) {
    std::cout
        << "Bow tie filtering should not be used for His data or Xim data!!"
        << std::endl;
    return;
  }
  QString comboBox_fBTcor_String = "1.4571;2.4506;2.6325;0.0095;4.1181"; //Hardcoded value (from comboBox_fBTcor)
  const auto strList = comboBox_fBTcor_String.split(';');

  this->m_cbctrecon->BowtieByFit(false,strList);// False (see checkBox_Fullfan)

  this->m_cbctrecon->SetMaxAndMinValueOfProjectionImage();
  std::cout << "Bow-tie correction done." << std::endl;
}
// Is implemented in SLT_LoadSelectedProjFiles()
void LoadingThread::SLT_DoReconstruction() { // Functions responsible for the reconstruction
  const auto fdk_options = m_parent->getFDKoptions();
  itk::TimeProbe reconTimeProbe;
  reconTimeProbe.Start();
  emit Signal_UpdateProgressBarLoad(80); // Signal to update the progressbar on scui
  if (this->m_parent->m_UseCUDA) {
    this->m_cbctrecon->DoReconstructionFDK<CUDA_DEVT>(REGISTER_RAW_CBCT,fdk_options);
  } else if (this->m_parent->m_UseOpenCL) {
    this->m_cbctrecon->DoReconstructionFDK<OPENCL_DEVT>(REGISTER_RAW_CBCT,fdk_options);
  } else {
    this->m_cbctrecon->DoReconstructionFDK<CPU_DEVT>(REGISTER_RAW_CBCT,fdk_options);
  }

  reconTimeProbe.Stop();
  std::cout << "It took " << reconTimeProbe.GetMean() << ' '
            << reconTimeProbe.GetUnit() << std::endl;

  auto size = m_parent->m_cbctrecon->m_spCrntReconImg->GetBufferedRegion().GetSize();
  m_parent->UpdateReconImage(this->m_cbctrecon->m_spCrntReconImg); // Updates the recon image
  emit Signal_DisconnectSlider(); // Signal to disconnect the slider
  const auto initVal = qRound((size[2] - 1) / 2.0); // Finding the middle of the image to use this as the slider value
  emit Signal_ReConnectSlider(initVal); // Signal to reconnect the slider
  emit Signal_UpdateSlider(size[2] - 1); // Signal to update the slider
  emit Signal_UpdateProgressBarLoad(100); // Signal to update the progressbar on scui
  emit Signal_LThreadIsDone(); // Signal which indicates that the loading threads run method has finished
}
