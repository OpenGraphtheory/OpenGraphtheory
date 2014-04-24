
#ifndef __OPENGRAPHTHEORY_VISUALIZE_COLOR_H
    #define __OPENGRAPHTHEORY_VISUALIZE_COLOR_H

    namespace OpenGraphtheory
    {
        namespace Visualization
        {

            class Color
            {
                public:
                    unsigned short Red;
                    unsigned short Green;
                    unsigned short Blue;
                    unsigned short Transparency;

                    Color();
                    Color(int color);
                    Color(unsigned short R, unsigned short G, unsigned short B);
                    Color(unsigned short R, unsigned short G, unsigned short B, unsigned short Alpha);
                    Color(const Color& c);
                    void operator=(const Color& c);
                    bool operator==(const Color& c);
                    bool operator!=(const Color& c);

                    int ToInt();
                    static Color DefaultColors[];
                    static const int NumDefaultColors = 8;
            };

        }
    }

#endif
