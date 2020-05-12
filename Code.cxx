#include <itkRGBPixel.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkGDCMImageIO.h>
//#include <itkExtractImageFilter.h>

const unsigned int	ImageDimension2D = 2;

typedef itk::RGBPixel<char> RGBPixelType;
typedef	itk::Image< RGBPixelType, ImageDimension2D > RGBInputImageType;
typedef	RGBInputImageType RGBOutputImageType;
typedef	itk::ImageFileReader< RGBInputImageType > RGBReaderType;
typedef	itk::ImageFileWriter< RGBOutputImageType > RGBWriterType;
typedef	itk::GDCMImageIO	ImageIOType;

int
main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " DicomFile " << std::endl;
		return EXIT_FAILURE;
	}

	RGBReaderType::Pointer reader = RGBReaderType::New();

	std::string fileName = argv[1];

	reader->SetFileName(fileName);

	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	reader->SetImageIO(gdcmImageIO);

	gdcmImageIO->KeepOriginalUIDOn();

	fileName += ".out.dcm";

	RGBWriterType::Pointer writer = RGBWriterType::New();
	writer->SetFileName(fileName);
	writer->SetInput(reader->GetOutput());
	writer->UseInputMetaDataDictionaryOff();
	//writer->UseInputMetaDataDictionaryOn();
	writer->SetImageIO(gdcmImageIO);

	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & e)
	{
		std::cerr << "exception in file writer " << std::endl;
		std::cerr << e << std::endl;
		return false;
	}


}