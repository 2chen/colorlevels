#include <sys/stat.h>

#include <libgen.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <sstream>

#include <Magick++.h>
#include <colorLevels.h>

#include <stdexcept>
#include <cerrno>
#include <memory>

#include <QApplication>
#include <QCommandLineParser>

#include "imageviewer.h"

//int
//main(
//    int argc,
//    char *argv[])
//{
//	Magick::InitializeMagick(*argv);
	
//	ColorLevels::Options options;
//	try {
//		options = ColorLevels::procargs(argc, argv);
//	} catch (std::exception &e) {
//		std::cerr << ColorLevels::getUsage(argv[0]) << "\n\nERROR: " <<
//		    e.what() << '.' << std::endl;
//		return (EXIT_FAILURE);
//	}
	
//    Magick::Blob blob;
//	struct stat sb;
//	ColorLevels::Statistics stats;
//	for (int fileNum = optind; fileNum < argc; fileNum++) {
//		if (stat(argv[fileNum], &sb)) {
//			std::cerr << basename(argv[fileNum]) << " -- " <<
//			    std::strerror(errno) << '.' << std::endl;
//		} else {
//			try {
//                stats = ColorLevels::getColorLevels(options, argv[fileNum], &blob);
//				std::cout << basename(argv[fileNum]) << " " <<
//				    stats << std::endl;
//			} catch (std::exception &e) {
//				std::cerr << e.what() << std::endl;
//			}
//		}
//	}

//    argc = 1;
//    QApplication app(argc, argv);
//    QGuiApplication::setApplicationDisplayName(ImageViewer::tr("Image Viewer"));
//    QCommandLineParser commandLineParser;
//    commandLineParser.addHelpOption();
//    commandLineParser.addPositionalArgument(ImageViewer::tr("[file]"), ImageViewer::tr("Image file to open."));
//    commandLineParser.process(QCoreApplication::arguments());
//    ImageViewer imageViewer;
//    if (!commandLineParser.positionalArguments().isEmpty()
//        && !imageViewer.loadFile(commandLineParser.positionalArguments().front())) {
//        return -1;
//    }

//    imageViewer.show();
//    imageViewer.loadBlob(&blob);
////    imageViewer.loadFile("/Users/yxing/Downloads/colorlevels/test.tif");
////    imageViewer.loadFile("/Users/yxing/Downloads/entity.svg.png");
//    return app.exec();
//    //return (EXIT_SUCCESS);
//}

/*******************************************************************************
 * Setup.
 ******************************************************************************/

ColorLevels::Options
ColorLevels::procargs(
    int argc,
    char *argv[])
{
	Options options;
	
	int opt;
	while ((opt = getopt(argc, argv, "b:e:f:g:h:m:r:p:t:vw:")) != -1) {
		switch (opt) {
		case 'f':
			options.redCoordinate =
			    ColorLevels::convertStringToCoordinate(optarg);
			options.redCoordinateSet = true;
			break;
		case 'g':
			options.purpleCoordinate =
			    ColorLevels::convertStringToCoordinate(optarg);
			options.purpleCoordinateSet = true;
			break;
		case 'h':
			options.backgroundCoordinate =
			    ColorLevels::convertStringToCoordinate(optarg);
			options.backgroundCoordinateSet = true;
			break;
		case 'm':
			switch (optarg[0]) {
			case 'o':
                options.drawMaskOverOriginalImage = true;
				break;
			case 't':
				options.drawMaskOverTransparent = true;
				break;
			default:
				throw std::logic_error("Invalid option for "
				    "[-m]ask output");
			}
			break;
		case 'r':
			if (!ColorLevels::checkImageMagickColor(optarg))
				throw std::logic_error("Invalid color for -r");
			options.redColor = optarg;
			break;
		case 'p':
			if (!ColorLevels::checkImageMagickColor(optarg))
				throw std::logic_error("Invalid color for -p");
			options.purpleColor = optarg;
			break;
		case 't':
			options.redThreshold = std::atof(optarg);
			break;
		case 'e':
			options.purpleThreshold = std::atof(optarg);
			break;
		case 'b':
			options.backgroundThreshold = std::atof(optarg);
			break;
		case 'v':
			options.verbose = true;
			break;
		case 'w':
			if (!ColorLevels::checkImageMagickColor(optarg))
				throw std::logic_error("Invalid color for -w");
			options.backgroundColor = optarg;
			break;
		default:
			throw std::logic_error("Invalid argument");
		}
	}
	if (optind >= argc)
		throw std::logic_error("No image provided");
	
	if (options.verbose) {
		std::cout << "Mask over original image == " <<
		    (options.drawMaskOverOriginalImage ? "TRUE" : "FALSE") <<
		    std::endl;
		std::cout << "Mask over transparent background == " <<
		    (options.drawMaskOverTransparent ? "TRUE" : "FALSE") <<
		    std::endl;
		if (options.redCoordinateSet)
			std::cout << "Red coordinate == " <<
			    options.redCoordinate << std::endl;
		else
			std::cout << "Red color == " << options.redColor <<
			    std::endl;
		std::cout << "Red threshold == " << options.redThreshold <<
		    std::endl;
		if (options.purpleCoordinateSet)
			std::cout << "Purple coordinate == " <<
			    options.purpleCoordinate << std::endl;
		else
			std::cout << "Purple color == " <<
			    options.purpleColor << std::endl;
		std::cout << "Purple threshold == " <<
		    options.purpleThreshold << std::endl;
		if (options.backgroundCoordinateSet)
			std::cout << "Background coordinate == " <<
			    options.backgroundCoordinate << std::endl;
		else
			std::cout << "Background color == " <<
			    (options.backgroundColor == "" ? "auto-detected" : 
			    options.backgroundColor) << std::endl;
		std::cout << "Background threshold == " <<
		    options.backgroundThreshold << std::endl;
		std::cout << "Verbose == TRUE" << std::endl;
		std::cout << std::endl;
	}
	
	return (options);
}

std::string
ColorLevels::getUsage(
    const std::string &binName)
{
	Options options;
	
	std::string usage = "Usage: " + binName + " ";
	usage += "[-mo] [-mt] [-r \"sRGB\"] [-p \"sRGB\"] [-w \"sRGB\"] "
	    "[-t 10] [-e 10] [-b 10] [-f 344,124] [-g 235,321] "
	    "[-h 0,0] image [image ...]\n\n";
	    
	usage += "-f <coordinate>\tPoint in image that should be used for "
	    "red\n";
	usage += "-g <coordinate>\tPoint in image that should be used for "
	    "purple\n";
	usage += "-h <coordinate>\tPoint in image that should be used for "
	    "background\n";
	usage += "-m\t\tOutput [m]ask over [o]riginal image or [t]ransparent "
	    "background\n";
	usage += "-r <color>\tRed sRGB color (Default: " + options.redColor + 
	    ")\n";
	usage += "-p <color>\tPurple sRGB color (Default: " +
	    options.purpleColor + ")\n";
	usage += "-w <color>\tBackground sRGB color (Default: auto-detected)\n";
	usage += "-t <threshold>\tCIEDE2000 threshold for red (Default: ";
	std::stringstream ss;
	ss << options.redThreshold;
	usage += ss.str() + ")\n";
	usage += "-e <threshold>\tCIEDE2000 threshold for purple (Default: ";
	ss.str("");
	ss << options.purpleThreshold;
	usage += ss.str() + ")\n";
	usage += "-b <threshold>\tCIEDE2000 threshold for background "
		"(Default: ";
	ss.str("");
	ss << options.backgroundThreshold;
	usage += ss.str() +")\n";
	usage += "-v\t\tBe verbose";
	
	usage += "\n\n";
	usage += "Coordinate values (-f, -g, -h) take precedence over color "
	    "values (-r, -p, -w).";
	
	return (usage);
}

bool
ColorLevels::checkImageMagickColor(
    const std::string &colorStr)
{
	try {
		auto color = Magick::Color(colorStr);
		return (true);
	} catch (...) {
		return (false);
	}
}

/*******************************************************************************
 * Operations.
 ******************************************************************************/

ColorLevels::Statistics
ColorLevels::getColorLevels(
    ColorLevels::Options options,
    std::string imagePath,
    Magick::Blob* blob)
{
	Magick::Image image;
	try {
		image.read(imagePath);
	} catch (Magick::Exception &e) {
		throw std::runtime_error(e.what());
	}

	/* For simplicity, only support sRGB */
	if (image.colorSpace() != Magick::sRGBColorspace)
		throw std::logic_error("Unsupported colorspace");
	/* Convert sRGB to Lab colorspace */
	image.colorSpace(Magick::ColorspaceType::LabColorspace);

	ColorLevels::Statistics imageStats;
	imageStats.width = image.size().width();
	imageStats.height = image.size().height();
	imageStats.redPixels = 0;
	imageStats.purplePixels = 0;
	imageStats.backgroundPixels = 0;
	const Magick::PixelPacket *pixels = image.getConstPixels(
	    0, 0, imageStats.width, imageStats.height);

	/* Setup image masks */
	Magick::Image maskTransparentImage, maskOriginalImage;
	Magick::PixelPacket *maskTransparentPixels, *maskOriginalPixels;
	if (options.drawMaskOverTransparent) {
		maskTransparentImage = Magick::Image(Magick::Geometry(
		    imageStats.width, imageStats.height),
		    Magick::Color("rgba(0,0,0,0)"));
		maskTransparentPixels = maskTransparentImage.getPixels(0, 0,
		    imageStats.width, imageStats.height);
	}
	if (options.drawMaskOverOriginalImage) {
		maskOriginalImage = Magick::Image(imagePath);
		maskOriginalPixels = maskOriginalImage.getPixels(0, 0,
		    imageStats.width, imageStats.height);
	}
	
	/* Sanity check color coordinates */
	if (options.redCoordinateSet) {
		if (options.redCoordinate.x >= imageStats.width)
			throw std::runtime_error("Invalid red X value.");
		if (options.redCoordinate.y >= imageStats.height)
			throw std::runtime_error("Invalid red Y value.");
	}
	if (options.purpleCoordinateSet) {
		if (options.purpleCoordinate.x >= imageStats.width)
			throw std::runtime_error("Invalid purple X value.");
		if (options.purpleCoordinate.y >= imageStats.height)
			throw std::runtime_error("Invalid purple Y value.");
	}
	if (options.backgroundCoordinateSet) {
		if (options.backgroundCoordinate.x >= imageStats.width)
			throw std::runtime_error("Invalid background X value.");
		if (options.backgroundCoordinate.y >= imageStats.height)
			throw std::runtime_error("Invalid background Y value.");
	}
	
	/* Setup colors */
	const ColorLevels::LAB red = options.redCoordinateSet ?
	    ColorLevels::LABColorToLAB(static_cast<Magick::ColorRGB>(
	        pixels[(imageStats.width * options.redCoordinate.x) +
	        options.redCoordinate.y])) :
	    ColorLevels::getLABValueOfRGBColor(
	        Magick::ColorRGB(options.redColor));
	if (options.redCoordinateSet && options.verbose)
		std::cout << "Red color: " << red << std::endl;
	const ColorLevels::LAB purple = options.purpleCoordinateSet ?
	    ColorLevels::LABColorToLAB(static_cast<Magick::ColorRGB>(
	        pixels[(imageStats.width * options.purpleCoordinate.x) +
	        options.purpleCoordinate.y])) :
	    ColorLevels::getLABValueOfRGBColor(
	        Magick::ColorRGB(options.purpleColor));
	if (options.purpleCoordinateSet && options.verbose)
		std::cout << "Purple color: " << purple << std::endl;
	const Magick::ColorRGB redMaskColor("#00FF00");
	const Magick::ColorRGB purpleMaskColor("#0000FF");
    const Magick::ColorRGB backgroundMaskColor("#000000");

	/* backgroundColor() returns an sRGB value, even in LAB colorspace */
	const ColorLevels::LAB backgroundColor = 
	    options.backgroundCoordinateSet ?
	    ColorLevels::LABColorToLAB(static_cast<Magick::ColorRGB>(
	        pixels[(imageStats.width * options.backgroundCoordinate.x) +
	        options.backgroundCoordinate.y])) :
	    ColorLevels::getLABValueOfRGBColor(
	        options.backgroundColor == "" ? image.backgroundColor() :
	        Magick::ColorRGB(options.backgroundColor));
	if ((options.backgroundCoordinateSet || 
	    options.backgroundColor == "") && options.verbose)
		std::cout << "Background color: " << backgroundColor <<
		    std::endl;
	
	for (size_t row = 0; row < imageStats.height; row++) {
		for (size_t col = 0; col < imageStats.width; col++) {
			/* Get color of current pixel */
			size_t offset = (imageStats.width * row) + col;
			ColorLevels::LAB currentPixelLAB =
			    ColorLevels::LABColorToLAB(
			    static_cast<Magick::ColorRGB>(pixels[offset]));
			    
			/* Check pixel for background */
			if (ColorLevels::CIEDE2000(backgroundColor, 
			    currentPixelLAB) < options.backgroundThreshold) {
				imageStats.backgroundPixels++;
				if (options.drawMaskOverTransparent)
					maskTransparentPixels[offset] =
					    backgroundMaskColor;
				if (options.drawMaskOverOriginalImage)
					maskOriginalPixels[offset] =
					    backgroundMaskColor;
			/* Check pixel for red */
			} else if (ColorLevels::CIEDE2000(red,
			    currentPixelLAB) < options.redThreshold) {
				imageStats.redPixels++;
				if (options.drawMaskOverTransparent)
					maskTransparentPixels[offset] =
					    redMaskColor;
				if (options.drawMaskOverOriginalImage)
					maskOriginalPixels[offset] =
					    redMaskColor;
			/* Check pixel for purple */
			} else if (ColorLevels::CIEDE2000(purple,
			    currentPixelLAB) < options.purpleThreshold) {
				imageStats.purplePixels++;
				if (options.drawMaskOverTransparent)
					maskTransparentPixels[offset] =
					    purpleMaskColor;
				if (options.drawMaskOverOriginalImage)
					maskOriginalPixels[offset] =
					    purpleMaskColor;
			}
		}
	}

	/* Save masks */	
	if (options.drawMaskOverTransparent) {
        maskTransparentImage.syncPixels();
        maskOriginalImage.write(blob);
//		maskTransparentImage.write(
//		    ColorLevels::basenameWithoutExtension(imagePath) +
//		    "_mask_transparent.png");
	}
	if (options.drawMaskOverOriginalImage) {
		maskOriginalImage.syncPixels();
        maskOriginalImage.write(blob);
//		maskOriginalImage.write(
//		    ColorLevels::basenameWithoutExtension(imagePath) +
//		    "_mask_original.png");
	}
    std::cout << "ok?" << blob->length() << "\n";
	return (imageStats);
}

Magick::ColorRGB
ColorLevels::colorAtPixel(Magick::Image* image, int x, int y) {
    int width = image->size().width();
    int height = image->size().height();
    const Magick::PixelPacket* pixels = image->getConstPixels(0, 0, width, height);

    return static_cast<Magick::ColorRGB>(pixels[width*y + x]);
}

QSet<int>*
ColorLevels::matchingPixels(Magick::Image* image, Magick::ColorRGB color, double threshold) {
    int width = image->size().width();
    int height = image->size().height();
    const Magick::PixelPacket* pixels = image->getConstPixels(0, 0, width, height);

    QSet<int>* matchingPixels = new QSet<int>;
    const ColorLevels::LAB referenceColor = ColorLevels::LABColorToLAB(color);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int offset = width*y + x;
            ColorLevels::LAB currentPixelLAB =
                ColorLevels::LABColorToLAB(
                static_cast<Magick::ColorRGB>(pixels[offset]));

            if (ColorLevels::CIEDE2000(referenceColor, currentPixelLAB) < threshold)
                matchingPixels->insert(offset);
        }
    }

    return matchingPixels;
}

Magick::PixelPacket*
ColorLevels::getPixels(Magick::Image* image) {
    int width = image->size().width();
    int height = image->size().height();
    Magick::PixelPacket* pixels = image->getPixels(0, 0, width, height);

    return pixels;
}

/*******************************************************************************
 * Conversions.
 ******************************************************************************/
 
std::string
ColorLevels::basenameWithoutExtension(
    const std::string &path)
{
    std::string filename(basename((char*)path.c_str()));

	if (filename.length() == 0)
		throw std::runtime_error("Error calculating basename");
	
	std::string::size_type pos = filename.find_last_of('.');
	if (pos == std::string::npos)
		return (filename);
	
	return (filename.substr(0, pos));
}

constexpr double
ColorLevels::deg2Rad(
    const double deg)
{
	return (deg * (M_PI / 180.0));
}

constexpr double
ColorLevels::rad2Deg(
    const double rad)
{
	return ((180.0 / M_PI) * rad);
}

ColorLevels::LAB
ColorLevels::LABColorToLAB(
    const Magick::ColorRGB &magickLAB)
{
	return (LAB{
	    magickLAB.red() * 100,
	    (magickLAB.green() * 256) - 100, 
	    (magickLAB.blue() * 256) - 100
	});
}

ColorLevels::LAB
ColorLevels::getLABValueOfRGBColor(
    const Magick::ColorRGB &RGBColor)
{
	/* Make 1x1 image of RGBColor in sRGB colorspace */
	Magick::Image image(Magick::Geometry(1, 1), RGBColor);
	
	/* Translate image to LAB colorspace */
	image.colorSpace(Magick::ColorspaceType::LabColorspace);
	
	/* Extract LAB color */
	const Magick::PixelPacket *pixels = image.getConstPixels(0, 0, 1, 1);
	const Magick::ColorRGB magickLABColor(pixels[0]);
	
	return (ColorLevels::LABColorToLAB(magickLABColor));
}

ColorLevels::Coordinate
ColorLevels::convertStringToCoordinate(
    const std::string &str)
{
	std::string::size_type commaPos = str.find_first_of(',');
	if (commaPos == std::string::npos)
		throw std::runtime_error("No comma found");
	
	Coordinate coordinate;
	coordinate.x = std::atoi(str.substr(0, commaPos).c_str());
	coordinate.y = std::atoi(str.substr(commaPos + 1,
	    str.size() - commaPos - 1).c_str());
	    
	return (coordinate);
}

double
ColorLevels::CIEDE2000(
    const LAB &lab1,
    const LAB &lab2)
{
	/* 
	 * "For these and all other numerical/graphical 􏰀delta E00 values
	 * reported in this article, we set the parametric weighting factors
	 * to unity(i.e., k_L = k_C = k_H = 1.0)." (Page 27).
	 */
	const double k_L = 1.0, k_C = 1.0, k_H = 1.0;
	const double deg360InRad = ColorLevels::deg2Rad(360.0);
	const double deg180InRad = ColorLevels::deg2Rad(180.0);
	const double pow25To7 = 6103515625.0; /* pow(25, 7) */
	
	/*
	 * Step 1 
	 */
	/* Equation 2 */
	double C1 = sqrt((lab1.a * lab1.a) + (lab1.b * lab1.b));
	double C2 = sqrt((lab2.a * lab2.a) + (lab2.b * lab2.b));
	/* Equation 3 */
	double barC = (C1 + C2) / 2.0;
	/* Equation 4 */
	double G = 0.5 * (1 - sqrt(pow(barC, 7) / (pow(barC, 7) + pow25To7)));
	/* Equation 5 */
	double a1Prime = (1.0 + G) * lab1.a;
	double a2Prime = (1.0 + G) * lab2.a;
	/* Equation 6 */
	double CPrime1 = sqrt((a1Prime * a1Prime) + (lab1.b * lab1.b));
	double CPrime2 = sqrt((a2Prime * a2Prime) + (lab2.b * lab2.b));
	/* Equation 7 */
	double hPrime1;
	if (lab1.b == 0 && a1Prime == 0)
		hPrime1 = 0.0;
	else
		hPrime1 = atan2(lab1.b, a1Prime);
		/* 
		 * This must be converted to a hue angle in degrees between 0 
		 * and 360 by addition of 2􏰏 to negative hue angles.
		 */
		if (hPrime1 < 0)
			hPrime1 += deg360InRad;
	double hPrime2;
	if (lab2.b == 0 && a2Prime == 0)
		hPrime2 = 0.0;
	else
		hPrime2 = atan2(lab2.b, a2Prime);
		/* 
		 * This must be converted to a hue angle in degrees between 0 
		 * and 360 by addition of 2􏰏 to negative hue angles.
		 */
		if (hPrime2 < 0)
			hPrime2 += deg360InRad;
	
	/*
	 * Step 2
	 */
	/* Equation 8 */
	double deltaLPrime = lab2.l - lab1.l;
	/* Equation 9 */
	double deltaCPrime = CPrime2 - CPrime1;
	/* Equation 10 */
	double deltahPrime;
	double CPrimeProduct = CPrime1 * CPrime2;
	if (CPrimeProduct == 0)
		deltahPrime = 0;
	else {
		/* Avoid the fabs() call */
		deltahPrime = hPrime2 - hPrime1;
		if (deltahPrime < -deg180InRad)
			deltahPrime += deg360InRad;
		else if (deltahPrime > deg180InRad)
			deltahPrime -= deg360InRad;
	}
	/* Equation 11 */
	double deltaHPrime = 2.0 * sqrt(CPrimeProduct) *
	    sin(deltahPrime / 2.0);
	
	/*
	 * Step 3
	 */
	/* Equation 12 */
	double barLPrime = (lab1.l + lab2.l) / 2.0;
	/* Equation 13 */
	double barCPrime = (CPrime1 + CPrime2) / 2.0;
	/* Equation 14 */
	double barhPrime, hPrimeSum = hPrime1 + hPrime2;
	if (CPrime1 * CPrime2 == 0) {
		barhPrime = hPrimeSum;
	} else {
		if (fabs(hPrime1 - hPrime2) <= deg180InRad)
			barhPrime = hPrimeSum / 2.0;
		else {
			if (hPrimeSum < deg360InRad)
				barhPrime = (hPrimeSum + deg360InRad) / 2.0;
			else
				barhPrime = (hPrimeSum - deg360InRad) / 2.0;
		}
	}
	/* Equation 15 */
	double T = 1.0 - (0.17 * cos(barhPrime - ColorLevels::deg2Rad(30.0))) +
	    (0.24 * cos(2.0 * barhPrime)) +
	    (0.32 * cos((3.0 * barhPrime) + ColorLevels::deg2Rad(6.0))) - 
	    (0.20 * cos((4.0 * barhPrime) - ColorLevels::deg2Rad(63.0)));
	/* Equation 16 */
	double deltaTheta = ColorLevels::deg2Rad(30.0) *
	    exp(-pow((barhPrime - deg2Rad(275.0)) / deg2Rad(25.0), 2.0));
	/* Equation 17 */
	double R_C = 2.0 * sqrt(pow(barCPrime, 7.0) /
	    (pow(barCPrime, 7.0) + pow25To7));
	/* Equation 18 */
	double S_L = 1 + ((0.015 * pow(barLPrime - 50.0, 2.0)) /
	    sqrt(20 + pow(barLPrime - 50.0, 2.0)));
	/* Equation 19 */
	double S_C = 1 + (0.045 * barCPrime);
	/* Equation 20 */
	double S_H = 1 + (0.015 * barCPrime * T);
	/* Equation 21 */
	double R_T = (-sin(2.0 * deltaTheta)) * R_C;
	
	/* Equation 22 */
	double deltaE = sqrt(
	    pow(deltaLPrime / (k_L * S_L), 2.0) +
	    pow(deltaCPrime / (k_C * S_C), 2.0) +
	    pow(deltaHPrime / (k_H * S_H), 2.0) + 
	    (R_T * (deltaCPrime / (k_C * S_C)) * (deltaHPrime / (k_H * S_H))));
	
	return (deltaE);
}

/*******************************************************************************
 * Operators.
 ******************************************************************************/

std::ostream&
operator<<(
    std::ostream &s,
    const ColorLevels::LAB &labColor)
{
	return (s << "CIELAB(" << labColor.l << "," << labColor.a << "," <<
	    labColor.b << ")");
}

std::ostream&
operator<<(
    std::ostream &s,
    const ColorLevels::Coordinate &coordinate)
{
	return (s << "(" << coordinate.x << "," << coordinate.y << ")");
}

std::ostream&
operator<<(
    std::ostream &s,
    const ColorLevels::Statistics &stats)
{
	/* Compute percentage of binned pixels */
	double redPercentage = 0, purplePercentage = 0;
	double denominator = ((stats.width * stats.height) - 
	    stats.backgroundPixels);    
	if (denominator != 0) {
		redPercentage = (stats.redPixels / denominator) * 100.0;
		purplePercentage = (stats.purplePixels / denominator) * 100.0;
	}
	
	s << "(" << stats.width << "x" << stats.height << ") -- ";
	s << "Red: " << stats.redPixels << " (" << std::setprecision(2) <<
	    std::fixed << redPercentage << "%), Purple: " <<
	    std::setprecision(0) << stats.purplePixels << " (" <<
	    std::setprecision(2) << purplePercentage << "%), ";
	s << "Background: " << stats.backgroundPixels << ", Total: " <<
	    (stats.width * stats.height) << ".";
	    
	return (s);
}
