#ifndef EAGLELAYERS_H
#define EAGLELAYERS_H


class EagleLayers
{
public:
    EagleLayers();


    enum SchematicLayers {
        SimResults = 88,
        SimProbes = 89,
        Modules = 90,
        Nets = 91,
        Busses = 92,
        Pins = 93,
        Symbols = 94,
        Names = 95,
        Values = 96,
        Info = 97,
        Guide = 98
    };

    enum PCBLayers {
        Top = 1,
        Route2 = 2,
        Route3 = 3,
        Route4 = 4,
        Route5 = 5,
        Route6 = 6,
        Route7 = 7,
        Route8 = 8,
        Route9 = 9,
        Route10 = 10,
        Route11 = 11,
        Route12 = 12,
        Route13 = 13,
        Route14 = 14,
        Route15 = 15,
        Bottom = 16,
        Pads = 17,
        Vias = 18,
        Unrouted = 19,
        Dimension = 20,
        tPlace = 21,
        bPlace = 22,
        tOrigins = 23,
        bOrigins = 24,
        tNames = 25,
        bNames = 26,
        tValues = 27,
        bValues = 28,
        tStop = 29,
        bStop = 30,
        tCream = 31,
        bCream = 32,
        tFinish = 33,
        bFinish = 34,
        tGlue = 35,
        bGlue = 36,
        tTest = 37,
        bTest = 38,
        tKeepout = 39,
        bKeepout = 40,
        tRestrict = 41,
        bRestrict = 42,
        vRestrict = 43,
        Drills = 44,
        Holes = 45,
        Milling = 46,
        Measures = 47,
        Document = 48,
        Reference = 49,
        tDocu = 51,
        bDocu = 52
    };
};

#endif // EAGLELAYERS_H
