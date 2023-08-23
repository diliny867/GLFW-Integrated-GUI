#pragma once

#include <glm/glm.hpp>

class BoundingBox {
public:
    BoundingBox(const float minX,const float minY,const float maxX,const float maxY): min(minX,minY),max(maxX,maxY) {}
    BoundingBox(const glm::vec2 min_,const glm::vec2 max_): min(min_),max(max_) {}
    BoundingBox():min(0),max(0){}

    glm::vec2 min;
    glm::vec2 max;

    float GetWidth() const { return max.x - min.x; }
    float GetHeight() const { return max.y - min.y; }
    glm::vec2 GetSize() const { return max - min; }
    float GetPosX() const{ return min.x; }
    float GetPosY() const{ return min.y; }
    glm::vec2 GetPosition() const{ return min; }
    glm::vec2 GetCenter() const{ return min+GetSize()/2.0f; }

    bool Intersects(const BoundingBox& other) const {
        return !(max.x < other.min.x || other.max.x < min.x || max.y < other.min.y || other.max.y < min.y);
    }

    bool ContainsPoint(const float pointX,const float pointY) const {
        return pointX >= min.x && pointX <= max.x && pointY >= min.y && pointY <= max.y;
    }
	bool ContainsPoint(const glm::vec2 point) const {
        return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
    }
};

//class BoundingBox {
//public:
//    BoundingBox(const double x_, const double y_, const double width_, const double height_): x(x_),y(y_),width(width_),height(height_) {}
//    double x;
//    double y;
//    double width;
//    double height;
//
//    bool Intersects(const BoundingBox& other) const {
//        return !(x>other.x+other.width || x+width<other.x || y>other.y+other.height || y+height<other.y);
//    }
//    //bool ContainsPoint(const double pointX, const double pointY) const {
//    //    return pointX>=x && pointX<=x+width && pointY>=y && pointY<=y+height;
//    //}
//    bool ContainsPoint(const double pointX,const double pointY) const { //to not fail when width or height less than 0
//        const bool isPointInXRange = (width >= 0) ? (pointX >= x && pointX <= x + width) : (pointX <= x && pointX >= x + width);
//        const bool isPointInYRange = (height >= 0) ? (pointY >= y && pointY <= y + height) : (pointY <= y && pointY >= y + height);
//
//        return isPointInXRange && isPointInYRange;
//    }
//
//    double GetX2() const {
//        return x+width;
//    }
//    double GetY2() const {
//        return y+height;
//    }
//};
