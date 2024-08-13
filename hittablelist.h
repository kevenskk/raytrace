#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittableobjects.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittableList : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittableList() {}
    hittableList(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const ray& r, interval tmintmax, hitRecord& rec) const override {
        hitRecord tempRec;
        bool succesfulHit = false;
        auto closestHit = tmintmax.max;


        for (const auto& object : objects) {
        if (object->hit(r, interval(tmintmax.min, closestHit), tempRec)) {

                succesfulHit = true;
                closestHit = tempRec.t;
                rec = tempRec;
            }
        }

        return succesfulHit;
    }
};

#endif