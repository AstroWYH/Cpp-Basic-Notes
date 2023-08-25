```cpp
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "libbase64.h"
#include "some_processor_logger.pb.h"

using namespace std;
using namespace adu::perception::foreground;

int g_frame_cnt = 0;
/***********************************************************/
class Object {
 public:
  int id_ = 99;
  double speed_ = 18.5;
  string name_ = "object";
};
/***********************************************************/
class Radar {
 public:
  void Init() { radar_logger_ = make_shared<RadarLogger>(); }
  void Process(shared_ptr<DataFusionLogger> data_fusion_logger) {
    cout << "Radar Start, frame cnt: "
         << "[" << g_frame_cnt << "]" << endl;

    radar_logger_->set_radar_int(g_frame_cnt + 103);
    radar_logger_->set_radar_double(g_frame_cnt + 103.5);
    radar_logger_->set_radar_string("set radar");
    data_fusion_logger->add_radar_logger()->CopyFrom(move(*radar_logger_));

    cout << "Radar End, frame cnt: "
         << "[" << g_frame_cnt << "]" << endl;
  }

 private:
  int radar_int_ = 2;
  double radar_double_ = 2.0;
  string radar_string_ = "Radar";

 private:
  shared_ptr<RadarLogger> radar_logger_;
};
/***********************************************************/
class DataFusion {
 public:
  void Init() {
    radar_ = make_shared<Radar>();
    radar_->Init();

    data_fusion_logger_ = make_shared<DataFusionLogger>();

    for (int i = 0; i < 3; i++) {
      data_fusion_objects_.emplace_back(Object());
    }
  }
  void Process(shared_ptr<SomeProcessorLogger> some_process_logger) {
    data_fusion_logger_->Clear();
    cout << "DataFusion Start, frame cnt: "
         << "[" << g_frame_cnt << "]" << endl;

    data_fusion_logger_->set_data_fusion_int(g_frame_cnt + 102);
    data_fusion_logger_->set_data_fusion_double(g_frame_cnt + 102.5);
    data_fusion_logger_->set_data_fusion_string("set data fusion");

    /***********************************************************/
    for (int track = 0; track < 2; track++) {
      int data_fusion_track = track;
      DataFusionTrack* ptr_data_fusion_track =
          data_fusion_logger_->add_data_fusion_track();
      ptr_data_fusion_track->set_data_fusion_track(g_frame_cnt + track + 500);

      for (auto obj : data_fusion_objects_) {
        DataFusionObject* ptr_data_fusion_object =
            ptr_data_fusion_track->add_data_fusion_object();
        ptr_data_fusion_object->set_data_fusion_object_id(g_frame_cnt + track +
                                                          10000);
        ptr_data_fusion_object->set_data_fusion_object_speed(g_frame_cnt +
                                                             track + 10000.8);
        ptr_data_fusion_object->set_data_fusion_object_name("object name");
      }
    }
    /***********************************************************/

    radar_->Process(data_fusion_logger_);
    some_process_logger->add_data_fusion_logger()->CopyFrom(
        move(*data_fusion_logger_));

    cout << "DataFusion End, frame cnt: "
         << "[" << g_frame_cnt << "]" << endl;
  }

 private:
  int data_fusion_int_ = 1;
  double data_fusion_double_ = 1.0;
  string data_fusion_string_ = "DataFusion";

  vector<Object> data_fusion_objects_;

 private:
  shared_ptr<Radar> radar_;

 private:
  shared_ptr<DataFusionLogger> data_fusion_logger_;
};
/***********************************************************/
class SomeProcessor {
 public:
  void Init() {
    data_fusion_ = make_shared<DataFusion>();
    data_fusion_->Init();

    some_process_logger_ = make_shared<SomeProcessorLogger>();
  }
  void Process() {
    some_process_logger_->Clear();
    cout << "SomeProcessor Start, frame cnt: "
         << "[" << g_frame_cnt << "]" << endl;

    some_process_logger_->set_some_processor_int(g_frame_cnt + 101);
    some_process_logger_->set_some_processor_double(g_frame_cnt + 101.5);
    some_process_logger_->set_some_processor_string("set some processor");
    data_fusion_->Process(some_process_logger_);

    // DELAY_LOGGER(some_process_logger_);
    cout << "SomeProcessor End, frame cnt: "
         << "[" << g_frame_cnt << "]" << endl;
    cout << some_process_logger_->DebugString() << endl;
  }

 private:
  int some_processor_int_ = 0;
  double some_processor_double_ = 0.0;
  string some_processor_string_ = "SomeProcessor";

 private:
  shared_ptr<DataFusion> data_fusion_;

 private:
  shared_ptr<SomeProcessorLogger> some_process_logger_;
};
/***********************************************************/
int main() {
  shared_ptr<SomeProcessor> some_processor = make_shared<SomeProcessor>();
  some_processor->Init();

  // run 5 times
  while (g_frame_cnt++ < 5) {
    some_processor->Process();
    cout << "--------------------------------------------------------" <<
    endl;
  }

// #ifdef USE_SSE_ACCELERATE
//   cout << "USE_SSE_ACCELERATE" << endl;
// #endif
//   cout << "hahaha" << endl;

  return 0;
}
/***********************************************************/
// std::string serialize_str;
// person.SerializeToString(&serialize_str);

// std::cout << "Serialized Data: " << serialize_str << std::endl;

// size_t out_length = 0;
// size_t string_len = serialize_str.size();
// size_t out_size = string_len * 2;

// char base64_char[out_size];
// base64_encode(serialize_str.c_str(), string_len, base64_char, &out_length,
//               BASE64_FORCE_AVX2);

// cout << "base64_char: " << base64_char << std::endl;
//////////////
SomeProcessor Start, frame cnt: [1]
DataFusion Start, frame cnt: [1]
Radar Start, frame cnt: [1]
Radar End, frame cnt: [1]
DataFusion End, frame cnt: [1]
SomeProcessor End, frame cnt: [1]
some_processor_int: 102
some_processor_double: 102.5
some_processor_string: "set some processor"
data_fusion_logger {
  data_fusion_int: 103
  data_fusion_double: 103.5
  data_fusion_string: "set data fusion"
  radar_logger {
    radar_int: 104
    radar_double: 104.5
    radar_string: "set radar"
  }
  data_fusion_track {
    data_fusion_track: 501
    data_fusion_object {
      data_fusion_object_id: 10001
      data_fusion_object_speed: 10001.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10001
      data_fusion_object_speed: 10001.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10001
      data_fusion_object_speed: 10001.8
      data_fusion_object_name: "object name"
    }
  }
  data_fusion_track {
    data_fusion_track: 502
    data_fusion_object {
      data_fusion_object_id: 10002
      data_fusion_object_speed: 10002.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10002
      data_fusion_object_speed: 10002.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10002
      data_fusion_object_speed: 10002.8
      data_fusion_object_name: "object name"
    }
  }
}

--------------------------------------------------------
SomeProcessor Start, frame cnt: [2]
DataFusion Start, frame cnt: [2]
Radar Start, frame cnt: [2]
Radar End, frame cnt: [2]
DataFusion End, frame cnt: [2]
SomeProcessor End, frame cnt: [2]
some_processor_int: 103
some_processor_double: 103.5
some_processor_string: "set some processor"
data_fusion_logger {
  data_fusion_int: 104
  data_fusion_double: 104.5
  data_fusion_string: "set data fusion"
  radar_logger {
    radar_int: 105
    radar_double: 105.5
    radar_string: "set radar"
  }
  data_fusion_track {
    data_fusion_track: 502
    data_fusion_object {
      data_fusion_object_id: 10002
      data_fusion_object_speed: 10002.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10002
      data_fusion_object_speed: 10002.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10002
      data_fusion_object_speed: 10002.8
      data_fusion_object_name: "object name"
    }
  }
  data_fusion_track {
    data_fusion_track: 503
    data_fusion_object {
      data_fusion_object_id: 10003
      data_fusion_object_speed: 10003.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10003
      data_fusion_object_speed: 10003.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10003
      data_fusion_object_speed: 10003.8
      data_fusion_object_name: "object name"
    }
  }
}

--------------------------------------------------------
SomeProcessor Start, frame cnt: [3]
DataFusion Start, frame cnt: [3]
Radar Start, frame cnt: [3]
Radar End, frame cnt: [3]
DataFusion End, frame cnt: [3]
SomeProcessor End, frame cnt: [3]
some_processor_int: 104
some_processor_double: 104.5
some_processor_string: "set some processor"
data_fusion_logger {
  data_fusion_int: 105
  data_fusion_double: 105.5
  data_fusion_string: "set data fusion"
  radar_logger {
    radar_int: 106
    radar_double: 106.5
    radar_string: "set radar"
  }
  data_fusion_track {
    data_fusion_track: 503
    data_fusion_object {
      data_fusion_object_id: 10003
      data_fusion_object_speed: 10003.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10003
      data_fusion_object_speed: 10003.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10003
      data_fusion_object_speed: 10003.8
      data_fusion_object_name: "object name"
    }
  }
  data_fusion_track {
    data_fusion_track: 504
    data_fusion_object {
      data_fusion_object_id: 10004
      data_fusion_object_speed: 10004.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10004
      data_fusion_object_speed: 10004.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10004
      data_fusion_object_speed: 10004.8
      data_fusion_object_name: "object name"
    }
  }
}

--------------------------------------------------------
SomeProcessor Start, frame cnt: [4]
DataFusion Start, frame cnt: [4]
Radar Start, frame cnt: [4]
Radar End, frame cnt: [4]
DataFusion End, frame cnt: [4]
SomeProcessor End, frame cnt: [4]
some_processor_int: 105
some_processor_double: 105.5
some_processor_string: "set some processor"
data_fusion_logger {
  data_fusion_int: 106
  data_fusion_double: 106.5
  data_fusion_string: "set data fusion"
  radar_logger {
    radar_int: 107
    radar_double: 107.5
    radar_string: "set radar"
  }
  data_fusion_track {
    data_fusion_track: 504
    data_fusion_object {
      data_fusion_object_id: 10004
      data_fusion_object_speed: 10004.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10004
      data_fusion_object_speed: 10004.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10004
      data_fusion_object_speed: 10004.8
      data_fusion_object_name: "object name"
    }
  }
  data_fusion_track {
    data_fusion_track: 505
    data_fusion_object {
      data_fusion_object_id: 10005
      data_fusion_object_speed: 10005.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10005
      data_fusion_object_speed: 10005.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10005
      data_fusion_object_speed: 10005.8
      data_fusion_object_name: "object name"
    }
  }
}

--------------------------------------------------------
SomeProcessor Start, frame cnt: [5]
DataFusion Start, frame cnt: [5]
Radar Start, frame cnt: [5]
Radar End, frame cnt: [5]
DataFusion End, frame cnt: [5]
SomeProcessor End, frame cnt: [5]
some_processor_int: 106
some_processor_double: 106.5
some_processor_string: "set some processor"
data_fusion_logger {
  data_fusion_int: 107
  data_fusion_double: 107.5
  data_fusion_string: "set data fusion"
  radar_logger {
    radar_int: 108
    radar_double: 108.5
    radar_string: "set radar"
  }
  data_fusion_track {
    data_fusion_track: 505
    data_fusion_object {
      data_fusion_object_id: 10005
      data_fusion_object_speed: 10005.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10005
      data_fusion_object_speed: 10005.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10005
      data_fusion_object_speed: 10005.8
      data_fusion_object_name: "object name"
    }
  }
  data_fusion_track {
    data_fusion_track: 506
    data_fusion_object {
      data_fusion_object_id: 10006
      data_fusion_object_speed: 10006.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10006
      data_fusion_object_speed: 10006.8
      data_fusion_object_name: "object name"
    }
    data_fusion_object {
      data_fusion_object_id: 10006
      data_fusion_object_speed: 10006.8
      data_fusion_object_name: "object name"
    }
  }
}
```
