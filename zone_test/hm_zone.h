//
// Created by Wei00161@umn.edu on 2020/5/15.
//

#ifndef LEVELDB_HM_ZONE_H
#define LEVELDB_HM_ZONE_H
#include "zone_namespace.h"
#include <fstream>
//TODO write pointer position look up
using namespace std;
namespace leveldb {

    class HmZone : public Zone {
    public:
        HmZone(fstream& fs):modify_zone_(fs){};
        ~HmZone(){};
        //need to assign: ZoneID, write_pointer, ....
        HmZone(fstream& fs,size_t id);

        //based on Zone id to open file
        //set C++ put pointer according to the write_pointer in zone
        Status OpenZone() override;

        Status CloseZone() override;

        Status FinishZone() override;

        ZoneInfo ReportZone() override;

        Status ResetWritePointer() override;

        Status ZoneRead(ZoneAddress addr,  char* data) override;

        Status ZoneWrite(ZoneAddress addr, const char* data) override;

        string ToString() const {

            string str = to_string(zoneInfo_.id);

            str+=";";
            str+=to_string(zoneInfo_.write_pointer);

            str+=";";
            str+=TypeStr[zoneInfo_.zone_type];

            str+=";";
            str+=ConditionStr[zoneInfo_.zone_condition];

            str+=";";
            str+=StateStr[zoneInfo_.zone_state];
            return str;
        }

    private:
        fstream& modify_zone_;
    };


    class HmZoneNamespace : public ZoneNamespace {
    public:
        HmZoneNamespace();
        ~HmZoneNamespace()= default;

        size_t GetZoneCount() override{
            return zones_.size();
        }

        // Create a new zone.
        Status NewZone() override;

        Status GetZone(int id, Zone &res_zone) override{
            Status status;
            auto it = zones_.find(id);
            if (it != zones_.end()) {
                res_zone = it->second;
                status = Status::OK();
            } else {
                status = Status::NotFound("[Zone_namespace.h] [RemoveZone] no zone to get, zone id: "+ to_string(id));
            }
            return status;
        }

        Status RemoveZone(int id) override{
            Status status;
            auto it = zones_.find(id);
            if (it != zones_.end()) {
                zones_.erase(it);
                status = Status::OK();
            } else {
                status = Status::NotFound("[Zone_namespace.h] [RemoveZone] no zone to remove, zone id: "+ to_string(id));
            }
            return status;
        }

        Status InitZNS(const char* dir_name) override;
        Status InitZone(const char *path, const char *filename,  char *filepath) override;
        Status Write(ZoneAddress addr, const char* data) override;
        Status Read(ZoneAddress addr,  char* data) override;
    private:
        int next_zone_id_ = 0;
        std::map<int, HmZone> zones_;
    };
}
#endif //LEVELDB_HM_ZONE_H
