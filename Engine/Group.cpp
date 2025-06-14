#include <stdexcept>
#include <utility>

#include "Group.hpp"
#include "IControl.hpp"
#include "IObject.hpp"
#include "Utils/Config.hpp" // for TILE_SIZE
#include <iostream>
#include <vector>

namespace Engine {
    void Group::addObject(bool shouldDelete, IObject *obj) {
        objects.emplace_back(shouldDelete, obj);
        obj->objectIterator = std::prev(objects.end());
    }
    void Group::addControl(bool shouldDelete, IControl *ctrl) {
        controls.emplace_back(shouldDelete, ctrl);
        ctrl->controlIterator = std::prev(controls.end());
    }
    void Group::insertObject(bool shouldDelete, IObject *obj, std::list<std::pair<bool, IObject *>>::iterator it) {
        objects.emplace(it, shouldDelete, obj);
        obj->objectIterator = std::prev(it);
    }
    Group::~Group() {
        Clear();
    }
    void Group::Clear() {
        for (auto &it : objects) {
            if (it.first) delete it.second;
        }
        objects.clear();
        for (auto &it : controls) {
            if (it.first) delete it.second;
        }
        controls.clear();
    }
    void Group::ClearObjects() {
        // delete only the objects list (not controls)
        for (auto &it : objects) {
            if (it.first) delete it.second;
        }
        objects.clear();
    }
    void Group::Update(float deltaTime) {
        for (auto it = objects.begin(); it != objects.end();) {
            auto preIt = it++;
            if (preIt->second->Visible)
                preIt->second->Update(deltaTime);
        }
    }
    void Group::Draw() const {
        for (auto &it : objects) {
            if (it.second->Visible)
                it.second->Draw();
        }
    }
    void Group::OnKeyDown(int keyCode) {
        std::vector<IControl*> copy;
        copy.reserve(controls.size());
        for (auto &c : controls) copy.push_back(c.second);
        for (auto *ctrl : copy) {
            ctrl->OnKeyDown(keyCode);
        }
    }
    void Group::OnKeyUp(int keyCode) {
        std::vector<IControl*> copy;
        copy.reserve(controls.size());
        for (auto &c : controls) copy.push_back(c.second);
        for (auto *ctrl : copy) {
            ctrl->OnKeyUp(keyCode);
        }
    }
    void Group::OnMouseDown(int button, int mx, int my) {
        std::vector<IControl*> copy;
        copy.reserve(controls.size());
        for (auto &c : controls) copy.push_back(c.second);
        for (auto *ctrl : copy) {
            ctrl->OnMouseDown(button, mx, my);
        }
    }
    void Group::OnMouseUp(int button, int mx, int my) {
        std::vector<IControl*> copy;
        copy.reserve(controls.size());
        for (auto &c : controls) copy.push_back(c.second);
        for (auto *ctrl : copy) {
            ctrl->OnMouseUp(button, mx, my);
        }
    }
    void Group::OnMouseMove(int mx, int my) {
        std::vector<IControl*> copy;
        copy.reserve(controls.size());
        for (auto &c : controls) copy.push_back(c.second);
        for (auto *ctrl : copy) {
            ctrl->OnMouseMove(mx, my);
        }
    }
    void Group::OnMouseScroll(int mx, int my, int delta) {
        std::vector<IControl*> copy;
        copy.reserve(controls.size());
        for (auto &c : controls) copy.push_back(c.second);
        for (auto *ctrl : copy) {
            ctrl->OnMouseScroll(mx, my, delta);
        }
    }
    void Group::RemoveObject(std::list<std::pair<bool, IObject *>>::iterator it) {
        if (it->first) delete it->second;
        objects.erase(it);
    }
    void Group::RemoveControl(std::list<std::pair<bool, IControl *>>::iterator it) {
        if (it->first) delete it->second;
        controls.erase(it);
    }
    void Group::RemoveControlObject(std::list<std::pair<bool, IControl *>>::iterator ctrlIt, std::list<std::pair<bool, IObject *>>::iterator objIt) {
        RemoveControl(ctrlIt);
        RemoveObject(objIt);
    }
    void Group::AddNewObject(IObject *obj) {
        addObject(true, obj);
    }
    void Group::InsertNewObject(IObject *obj, std::list<std::pair<bool, IObject *>>::iterator it) {
        insertObject(true, obj, it);
    }
    void Group::AddNewControl(IControl *ctrl) {
        addControl(true, ctrl);
    }
    void Group::AddNewControlObject(IControl *ctrl) {
        if (!dynamic_cast<IObject *>(ctrl))
            throw std::invalid_argument("The control must inherit both IObject and IControl.");
        addObject(false, dynamic_cast<IObject *>(ctrl));
        addControl(true, ctrl);
    }
    void Group::AddRefObject(IObject &obj) {
        addObject(false, &obj);
    }
    void Group::InsertRefObject(IObject &obj, std::list<std::pair<bool, IObject *>>::iterator it) {
        insertObject(false, &obj, it);
    }
    void Group::AddRefControl(IControl &ctrl) {
        addControl(false, &ctrl);
    }
    void Group::AddRefControlObject(IControl &ctrl) {
        if (!dynamic_cast<IObject *>(&ctrl))
            throw std::invalid_argument("The control must inherit both IObject and IControl.");
        addObject(false, dynamic_cast<IObject *>(&ctrl));
        addControl(false, &ctrl);
    }
    void Group::CleanUpOffScreen() {
        // Define the left boundary. An object is considered off-screen
        // if its right edge has passed this point. We use a small buffer.
        const float leftBound = -TILE_SIZE; 

        // Use a safe loop to iterate and erase.
        // We manually control the iterator's advancement.
        for (auto it = objects.begin(); it != objects.end(); /* no increment here */) {
            // Check if the object's right edge is to the left of our boundary.
            if (it->second->Position.x + it->second->Size.x < leftBound) {
                // This object is off-screen. Remove it.
                // The 'erase' method returns an iterator to the next valid element.
                if (it->first) {
                    delete it->second; // Free the memory if the Group owns this object.
                }
                it = objects.erase(it); // Erase the object from the list and update the iterator.
            } else {
                // If we didn't erase the object, manually advance the iterator to the next one.
                ++it;
            }
        }
    }
    std::list<IObject *> Group::GetObjects() {
        std::list<IObject *> list;
        for (auto &it : objects)
            list.push_back(it.second);
        return list;
    }
    std::list<IControl *> Group::GetControls() {
        std::list<IControl *> list;
        for (auto &it : controls)
            list.push_back(it.second);
        return list;
    }
}
