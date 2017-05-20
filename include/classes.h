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
   std::string director_name;
   std::vector<std::string> categories;
   std::vector<std::string> actor_ids;

};

class user {
 public:
   user(std::string user_id, std::string name) {
     this -> user_id = user_id;
     this -> name = name;
   }
   ~user(){}
   std::string get_user_id() {
     return user_id;
   }

 private:
   std::string user_id;
   std::string name;

};

class actor {
 public:
   actor(std::string actor_id, std::string name) {
     this -> actor_id = actor_id;
     this -> name = name;
     first = last = 0;
   }
   ~actor(){}
   std::string get_actor_id() {
     return actor_id;
   }

 private:
   std::string actor_id;
   std::string name;
   int first, last;

};

#endif  // SD_TEMA3_INCLUDE_CLASSES_H_
