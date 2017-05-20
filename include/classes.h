/* Copyright 2017 Andrei Petre */

#ifndef SD_TEMA3_INCLUDE_CLASSES_H_
#define SD_TEMA3_INCLUDE_CLASSES_H_

#include <string>
#include <vector>

class movie {
 public:

 private:
   std::string movie_id;
   std::string movie_name;
   int timestamp;
   string director_name;
   std::vector<std::string> categories;
   std::vector<std::string> actor_ids;

};

class user {
 public:

 private:
   std::string user_id;
   std::string name;

};

class actor {
 public:

 private:
   std::string actor_id;
   std::string name;

};

#endif  // SD_TEMA3_INCLUDE_CLASSES_H_
