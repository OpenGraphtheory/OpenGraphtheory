
#include "../../Headers/visualize/color.h"
using namespace OpenGraphtheory::Visualization;


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!  ATTENTION!!! IF YOU CHANGE THIS, YOU MUST ALSO  !!
// !!       CHANGE NumDefaultColors IN color.h         !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
Color Color::DefaultColors[] =
{
    Color(0xE5, 0x00, 0x00), // red
    Color(0xF3, 0xD5, 0x10), // yellow
    Color(0x34, 0xCD, 0x20), // green
    Color(0x1C, 0x23, 0xEB), // dark blue
    Color(0xF3, 0x7D, 0x23), // orange
    Color(0x15, 0xCD, 0xC7), // light blue
    Color(0x87, 0x22, 0xE5), // purple
    Color(0xFF, 0x33, 0xFF)  // pink
};

Color::Color()
{
    Red = 0;
    Green = 0;
    Blue = 0;
    Transparency = 0;
}

Color::Color(int value)
{
    Blue = value % 256; value /= 256;
    Green = value % 256; value /= 256;
    Red = value % 256; value /= 256;
    Transparency = value % 256;
}

Color::Color(unsigned short R, unsigned short G, unsigned short B)
{
    Red = R;
    Green = G;
    Blue = B;
    Transparency = 0;
}

Color::Color(unsigned short R, unsigned short G, unsigned short B, unsigned short Alpha)
{
    Red = R;
    Green = G;
    Blue = B;
    Transparency = Alpha;
}

Color::Color(const Color& c)
{
    Red = c.Red;
    Green = c.Green;
    Blue = c.Blue;
    Transparency = c.Transparency;
}

int Color::ToInt()
{
    return (((int)Transparency * 256 + Red) * 256 + Green) * 256 + Blue;
}

void Color::operator=(const Color& c)
{
    Red = c.Red;
    Green = c.Green;
    Blue = c.Blue;
    Transparency = c.Transparency;
}

bool Color::operator==(const Color& c)
{
    return Red == c.Red
            && Green == c.Green
            && Blue == c.Blue
            && Transparency == c.Transparency;
}

bool Color::operator!=(const Color& c)
{
    return Red != c.Red
            || Green != c.Green
            || Blue != c.Blue
            || Transparency != c.Transparency;
}
