//#ifndef MAINWINDOW_IO_H
#define MAINWINDOW_IO_H
/*All IO functions used with cbctrecon*/
#ifdef OF
#undef OF
#endif // OF

//#include "gdcmAttribute.h"

//#include "cbctrecon_config.h"
//#include "cbctrecon_types.h"

//class QXmlStreamReader;
//class QDir;
//struct Rtss_modern;
/*
class mainwindow_io
{
public:
    mainwindow_io();
};

#endif // MAINWINDOW_IO_H
*/

template <typename ImageType>
void saveImageAsMHA(typename ImageType::Pointer &image,
                    std::string filename = "Projections.mha") {
  using ImageWriterType = itk::ImageFileWriter<ImageType>;
  typename ImageWriterType::Pointer writer = ImageWriterType::New();
  writer->SetInput(image);
  writer->SetFileName(filename);
  // writer->SetUseCompression(true); // quite expensive in CPU time
  // ITK's zlib and native-compiled zlib are almost equally fast:
  // Being about 75 seconds slower than without compression (in test with
  // alderson head scan)
  // (maybe an option to change compression rate will appear some day)
  writer->Update();
}


