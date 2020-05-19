#include <itkRGBPixel.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkGDCMImageIO.h>
#include <type_traits>

// Code crashes when the following define is enabled.
#define SHARE_IO_OBJECT
#ifndef SHARE_IO_OBJECT	
#include <itkMetaDataDictionary.h>
#include <itkMetaDataObject.h>
#endif 

/////////////////////////////////////////////////////////////////////////////////////////

const unsigned int	ImageDimension2D = 2;

typedef itk::RGBPixel<unsigned char> RGBPixelType;
typedef	itk::Image< RGBPixelType, ImageDimension2D > RGBInputImageType;
typedef	RGBInputImageType RGBOutputImageType;
typedef	itk::ImageFileReader< RGBInputImageType > RGBReaderType;
typedef	itk::ImageFileWriter< RGBOutputImageType > RGBWriterType;
typedef	itk::GDCMImageIO	ImageIOType;

/////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " DicomFile " << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << std::boolalpha << "A char is signed: " << std::is_signed<char>::value << std::endl;

	RGBReaderType::Pointer reader = RGBReaderType::New();

	std::string fileName = argv[1];

	reader->SetFileName(fileName);

	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	reader->SetImageIO(gdcmImageIO);

	gdcmImageIO->KeepOriginalUIDOn();

	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject& e)
	{
		std::cerr << "exception in file reader " << std::endl;
		std::cerr << e << std::endl;
		return false;
	}

	fileName += ".new.dcm";

	RGBWriterType::Pointer writer = RGBWriterType::New();
	writer->SetFileName(fileName);
	writer->SetInput(reader->GetOutput());

#ifdef SHARE_IO_OBJECT	
	writer->SetImageIO(gdcmImageIO);
#else  // def SHARE_IO_OBJECT
	
	// If not sharing itk::GDCMImageIO we copy the dictionary separately
	auto dict = gdcmImageIO->GetMetaDataDictionary();
	ImageIOType::Pointer gdcmImageIO1 = ImageIOType::New();
	writer->SetImageIO(gdcmImageIO1);

	gdcmImageIO1->SetMetaDataDictionary(dict);
#endif // def SHARE_IO_OBJECT

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
