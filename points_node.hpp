//
// Created by loic on 19/04/2020.
//

#ifndef LD46_POINTS_NODE_HPP
#define LD46_POINTS_NODE_HPP

#include <entities/scenenode.hpp>

class PointsNode : public SceneNode
{
public:
    unsigned int getCategory() const override;

    void addPoint()
    {
        mPoints++;
        mHasChanged = true;
    }

    int points() const { return mPoints; }

    bool poll()
    {
        if (mHasChanged) {
            mHasChanged = false;
            return true;
        }

        return false;
    }

private:
    bool mHasChanged = true;
    int  mPoints = 0;
};


#endif //LD46_POINTS_NODE_HPP
