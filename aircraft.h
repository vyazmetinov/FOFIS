#pragma once
#include <cmath>
#include <vector>
#include <algorithm>

class Point {
public:
    double lat;
    double lon;
    double height;
    Point(double lat, double lon, double height) : lat(lat), lon(lon), height(height) {}
    Point() : lat(0.0), lon(0.0), height(0.0) {}
};

static inline double distance_point_to_segment(const Point& A, const Point& B, const Point& P) {
    double ABx = B.lat - A.lat;
    double ABy = B.lon - A.lon;
    double ABz = B.height - A.height;

    double APx = P.lat - A.lat;
    double APy = P.lon - A.lon;
    double APz = P.height - A.height;

    double len_sq = ABx*ABx + ABy*ABy + ABz*ABz;
    if (len_sq == 0.0) {
        // Вырожденный сегмент: расстояние до точки A
        double dx = APx, dy = APy, dz = APz;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }

    double t = (APx*ABx + APy*ABy + APz*ABz) / len_sq;
    if (t < 0.0) t = 0.0;
    else if (t > 1.0) t = 1.0;

    double cx = A.lat + ABx * t;
    double cy = A.lon + ABy * t;
    double cz = A.height + ABz * t;

    double dx = P.lat - cx;
    double dy = P.lon - cy;
    double dz = P.height - cz;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

class Aircraft {
    int id;
    double vel;
    double accel;
public:
    Point pos;

    Aircraft(int id, double vel, double accel, Point pos)
        : id(id), vel(vel), accel(accel), pos(pos) {}

    Aircraft() : id(0), vel(0.0), accel(0.0), pos() {}

    int is_intruder(const std::vector<Point>& tunnel, double width) const {
        if (tunnel.size() == 0) {
            return 1;
        }
        if (tunnel.size() == 1) {
            return distance_point_to_segment(tunnel[0], tunnel[0], pos) > width ? 1 : 0;
        }

        double min_dist = std::numeric_limits<double>::infinity();
        for (size_t i = 0; i + 1 < tunnel.size(); ++i) {
            double d = distance_point_to_segment(tunnel[i], tunnel[i+1], pos);
            if (d <= width) {
                return 0;
            }
            if (d < min_dist) min_dist = d;
        }
        return (min_dist > width) ? 1 : 0;
    }
};