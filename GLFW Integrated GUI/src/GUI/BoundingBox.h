#pragma once

class BoundingBox {
public:
    BoundingBox(const double x_, const double y_, const double width_, const double height_): x(x_),y(y_),width(width_),height(height_) {}
    double x;
    double y;
    double width;
    double height;

    bool Intersects(const BoundingBox& other) const {
        return !(x>other.x+other.width || x+width<other.x || y>other.y+other.height || y+height<other.y);
    }
    bool ContainsPoint(const double pointX, const double pointY) const {
        return pointX>=x && pointX<=x+width && pointY>=y && pointY<=y+height;
    }

    double GetX2() const {
        return x+width;
    }
    double GetY2() const {
        return y+height;
    }
};