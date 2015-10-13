#ifndef __GPF_COLOR_LEVELS_H__
#define __GPF_COLOR_LEVELS_H__

#ifdef _MSC_VER

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else
#include <stdint.h>
#endif

#include <Magick++.h>
#include <QSet>

#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#endif

/** Namespace containing all functions for the colorLevels program */
namespace ColorLevels
{
	/***********************************************************************
	 * Types.
	 **********************************************************************/
	 
	/** Coordinate */
	struct Coordinate
	{
		/** X coordinate */
		uint64_t x;
		/** Y coordinate */
		uint64_t y;
	};
	/** Convenience definition for struct Coordinate */
	typedef struct Coordinate Coordinate;
	 
	/** Statistics from calculating color levels */
	struct Statistics
	{
		/** Width of image */
		size_t width;
		/** Height of image */
		size_t height;
		/** Number of pixels comprising the "background" */
		size_t backgroundPixels = 0;
		/** Number of pixels within threshold of the red color */
		size_t redPixels = 0;
		/** Number of pixels within threshold of the purple color */
		size_t purplePixels = 0;
	};
	/** Convenience definition for struct Statistics */
	typedef struct Statistics Statistics;
	
	/** Options used during getColorLevels() */
	struct Options
	{
		/** Output a mask image overtop the original pixels */
		bool drawMaskOverOriginalImage = false;
		/** Output a mask image overtop a transparent background */
		bool drawMaskOverTransparent = false;
		/** Color for background */
		std::string backgroundColor = "";
		/** Delta-E 2000 threshold for background color */
		double backgroundThreshold = 10;
		/** Whether or not backgroundCoordinate is set */
		bool backgroundCoordinateSet = false;
		/** Coordinate to use for background color (takes precedence) */
		Coordinate backgroundCoordinate;
		/** Color for red */
		std::string redColor = "#85180f";
		/** Delta-E 2000 threshold for red color */
		double redThreshold = 13;
		/** Whether or not redCoordinate is set */
		bool redCoordinateSet = false;
		/** Coordinate to use for red color (takes precedence) */
		Coordinate redCoordinate;
		/** Color for purple */
		std::string purpleColor = "#A0523B";
		/** Delta-E threshold for purple color */
		double purpleThreshold = 8;
		/** Whether or not purpleCoordinate is set */
		bool purpleCoordinateSet = false;
		/** Coordinate to use for purple color (takes precedence) */
		Coordinate purpleCoordinate;
		/** Be verbose */
		bool verbose = false;
	};
	/** Convenience definition for struct Options */
	typedef struct Options Options;
	
	/** A color in CIELAB colorspace */
	struct LAB
	{
		/** Lightness */
		double l;
		/** Color-opponent a dimension */
		double a;
		/** Color-opponent b dimension */
		double b;
	};
	/** Convenience definition for struct LAB */
	typedef struct LAB LAB;
	
    /****
     * YICHEN BABY
     * ***/

    Magick::ColorRGB
    colorAtPixel(Magick::Image* image, int x, int y);

    QSet<int>*
    matchingPixels(Magick::Image* image, Magick::ColorRGB color, double threshold);

    Magick::PixelPacket*
    getPixels(Magick::Image* image);

	/***********************************************************************
	 * Setup.
	 **********************************************************************/
	 
	/**
	 * @brief
	 * Parse command line arguments.
	 *
	 * @param argc
	 * argc from main().
	 * @param argv
	 * argv from main()
	 *
	 * @return
	 * Populated struct Options with defaults replaced by command-line
	 * options.
	 *
	 * @throw std::logic_exception
	 * Bad argument.
	 */
	Options
	procargs(
	    int argc,
	    char *argv[]);
	    
	/**
	 * @brief
	 * Obtain usage string.
	 *
	 * @param binName
	 * Name of the program being executed.
	 *
	 * @return
	 * Usage string.
	 */
	std::string
	getUsage(
	    const std::string &binName);
	    
	/**
	 * @brief
	 * Check to see if a string is a valid ImageMagick color.
	 *
	 * @param colorStr
	 * String to use as an ImageMagick color.
	 *
	 * @return
	 * true if colorStr can be used, false otherwise.
	 */
	bool
	checkImageMagickColor(
	    const std::string &colorStr);

	/***********************************************************************
	 * Operations.
	 **********************************************************************/

	/**
	 * @brief
	 * Perform color matching for image.
	 * @details
	 * For simplicity, the image must be in the sRGB colorspace, because
	 * the program only has routines for converting from sRGB to LAB.
	 *
	 * @param options
	 * Options for color matching.
	 * @param imagePath
	 * Path to image on which to perform color matching.
	 *
	 * @throw std::runtime_exception
	 * Cannot open image
	 * @throw std::logic_error
	 * imagePath not in sRGB colorspace.
	 */
	ColorLevels::Statistics
    getColorLevels(ColorLevels::Options options,
        std::string imagePath, Magick::Blob* blob);
	
	/**
	 * @brief
	 * Obtain Delta-E 2000 value.
	 * @details
	 * Based on the paper "The CIEDE2000 Color-Difference Formula: 
	 * Implementation Notes, Supplementary Test Data, and Mathematical 
	 * Observations" by Gaurav Sharma, Wencheng Wu, and Edul N. Dalal,
	 * from http://www.ece.rochester.edu/~gsharma/ciede2000/.
	 *
	 * @param lab1
	 * First color in LAB colorspace.
	 * @param lab2
	 * Second color in LAB colorspace.
	 *
	 * @return
	 * Delta-E difference between lab1 and lab2.
	 */
	double
	CIEDE2000(
	    const LAB &lab1,
	    const LAB &lab2);
	    
	/***********************************************************************
	 * Conversions.
	 **********************************************************************/
	
	/**
	 * @brief
	 * Remove the last .ext from a file path.
	 *
	 * @param path 
	 * Path to a file with a dot extension.
	 *
	 * @return
	 * basename(filepath) without dot extension.
	 *
	 * @throw std::runtime_exception
	 * Error from basename().
	 */
	std::string
	basenameWithoutExtension(
	    const std::string &path);
	
    	/**
    	 * @brief
    	 * Convert degrees to radians.
    	 *
    	 * @param deg
    	 * Angle in degrees.
    	 *
    	 * @return
    	 * deg in radians.
    	 */
    const double
    deg2Rad(
	    const double deg);
	
	/**
    	 * @brief
    	 * Convert radians to degrees.
    	 *
    	 * @param rad
    	 * Angle in radians.
    	 *
    	 * @return
    	 * rad in degrees.
    	 */
    const double
    rad2Deg(
	    const double rad);
	
	/**
	 * @brief
	 * Convert ImageMagick color in LAB colorspace to LAB struct
	 * used in calculations in this program.
	 *
	 * @param magickLAB
	 * An ImageMagick color in LAB colorspace.
	 *
	 * @return
	 * Representation of magickLAB as a struct LAB.
	 */
	LAB
	LABColorToLAB(
	    const Magick::ColorRGB &magickLAB);
	
	/**
	 * @brief
	 * Convert an ImageMagick color from sRGB colorspace to the LAB
	 * colorspace.
	 *
	 * @param RGBColor
	 * ImageMagick color in sRGB colorspace
	 *
	 * @return
	 * RGBColor in LAB colorspace.
	 */
	LAB
	getLABValueOfRGBColor(
	    const Magick::ColorRGB &RGBColor);
	
	/**
	 * @brief
	 * Convert CSV string to a coordinate.
	 *
	 * @param str
	 * String consisting of two numbers separated by a single comma.
	 *
	 * @return
	 * Coordinate representation of str.
	 * 
	 * @throw
	 * str is not as expected.
	 */
	Coordinate
	convertStringToCoordinate(
	    const std::string &str);

#ifdef RUN_TEST	    
	/***********************************************************************
	 * Test.
	 **********************************************************************/

	/** Print a the test dataset run from the CIEDE2000 paper */
	void
	testCIEDE2000();
#endif /* RUN_TEST */
}

/*******************************************************************************
 * Conversions.
 ******************************************************************************/

/**
 * @brief
 * LAB output stream operator.
 *
 * @param s
 * Output stream.
 * @param labColor
 * Color to output.
 *
 * @return
 * s with labColor appended.
 */
std::ostream&
operator<<(
    std::ostream &s,
    const ColorLevels::LAB &labColor);
    
/**
 * @brief
 * Coordinate output stream operator.
 *
 * @param s
 * Output stream
 * @param coordinate
 * Coordinate to append to s.
 *
 * @return
 * s with coordinate appended.
 */
std::ostream&
operator<<(
    std::ostream &s,
    const ColorLevels::Coordinate &coordinate);
    
/**
 * @brief
 * Statistics output stream operator.
 *
 * @param s
 * Output stream
 * @param stats
 * Statistics to append to s.
 *
 * @return
 * s with stats appended.
 */
std::ostream&
operator<<(
    std::ostream &s,
    const ColorLevels::Statistics &stats);
    
#endif /* __GPF_COLOR_LEVELS_H__ */
