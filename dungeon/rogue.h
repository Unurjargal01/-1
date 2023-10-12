#pragma once

#include "dungeon.h"

#include <stdexcept>
#include <vector>
#include <queue>
#include <set>

// Looks Like BFS or DFS. But BFS might be good choice to terminate earlier.
// Check Room and Get Keys;
bool Check(const Room* room, std::set<std::string>* new_keys, std::set<Door*>* new_doors) {
    // Checks whether the room is final.
    if (room->IsFinal()) {
        return true;
    }
    // Picks up the keys;
    for (size_t i = 0; i < room->NumKeys(); ++i) {
        new_keys->insert(room->GetKey(i));
    }
    // Finds the doors;
    for (size_t i = 0; i < room->NumDoors(); ++i) {
        new_doors->insert(room->GetDoor(i));
    }
    return false;
}

void Opens(std::set<std::string>& keys, std::set<Door*>& doors, std::set<const Room*>& rooms,
           std::set<const Room*>* new_rooms) {
    for (auto& door : doors) {
        const Room* opened_room;
        bool pos = false;
        if (door->IsOpen()) {
            // opened_room = door->GoThrough();
            pos = true;
        } else {
            for (auto& key : keys) {
                // if opens add to opened_room
                if ((door->TryOpen(key))) {
                    // opened_room = door->GoThrough();
                    pos = true;
                    break;
                }
            }
        }
        if (pos) {
            opened_room = door->GoThrough();
        }
        if (pos && (rooms.find(opened_room) == rooms.end())) {
            new_rooms->insert(opened_room);
        }
    }
}
const Room* FindFinalRoom(const Room* starting_room) {
    std::set<std::string> keys;
    std::set<const Room*> rooms{starting_room};
    std::set<Door*> old_doors;
    std::set<const Room*> new_rooms;

    new_rooms.insert(starting_room);
    while (!new_rooms.empty()) {
        std::set<const Room*> new_temp_rooms;
        std::set<std::string> new_keys;
        std::set<Door*> new_doors;

        // Gathers new keys and information about new doors;
        for (auto& room : new_rooms) {
            if (Check(room, &new_keys, &new_doors)) {
                return room;
            }
        }
        // try to use new_keys to new doors;
        Opens(new_keys, new_doors, rooms, &new_temp_rooms);
        // try to use new_keys to old_doors;
        Opens(new_keys, old_doors, rooms, &new_temp_rooms);
        // try to use old_keys to new_doors;
        Opens(keys, new_doors, rooms, &new_temp_rooms);
        // attach new_keys to old_keys;
        keys.insert(new_keys.begin(), new_keys.end());

        old_doors.insert(new_doors.begin(), new_doors.end());
        rooms.insert(new_rooms.begin(), new_rooms.end());

        // new_temp_rooms become new_rooms;
        new_rooms = new_temp_rooms;
        new_keys.clear();
        new_doors.clear();
    }
    return nullptr;
}
