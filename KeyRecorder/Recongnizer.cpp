#include "Recongnizer.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <QDebug>

Recongnizer::Recongnizer()
{

}

Recongnizer::~Recongnizer()
{

}

QString Recongnizer::DoRecongnize(const char* data, int size)
{
    char* outText;

    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path

    char* strPath = "D:\\Program Files\\Tesseract-OCR\\tessdata";
    if (api->Init(strPath, "chi_sim")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
	}

	// Open input image with leptonica library
    // Pix* image = pixRead("test3.png");
	Pix* image = pixReadMem((const l_uint8*)data, size);
	api->SetImage(image);
	// Get OCR result
	outText = api->GetUTF8Text();
    qDebug("OCR output:\n%s", outText);

    QString strOut = QString(outText);

    // Destroy used object and release memory
    api->End();
    delete[] outText;
    pixDestroy(&image);

    return strOut;
}
