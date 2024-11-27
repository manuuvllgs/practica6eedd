//
// Created by Manuel Villegas Aceituno on 29/10/24.
//

#ifndef UTM_H
#define UTM_H


class UTM {
    float _lat, _lon;

public:
    float lat() const {
        return _lat;
    }

    float lon() const {
        return _lon;
    }

    UTM(float lat, float lon)
        : _lat(lat),
          _lon(lon) {
    }
};


#endif //UTM_H
