/* Copyright 2017 Andrei Petre */

#ifndef SD_TEMA3_INCLUDE_CLASSES_H_
#define SD_TEMA3_INCLUDE_CLASSES_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <climits>

class movie {
 public:
   movie() {}

   movie(std::string movie_name,
      std::string movie_id,
      int timestamp,
      std::vector<std::string> categories,
      std::string director_name,
      std::vector<std::string> actor_ids) {
        this -> movie_id = movie_id;
        this -> movie_name = movie_name;
        this -> timestamp = timestamp;
        this -> director_name = director_name;
        this -> categories = categories;
        this -> actor_ids = actor_ids;
        this -> rate_sum = 0;
        this -> nr_rates = 0;
   }

   ~movie(){}

   std::string get_movie_id() {
     return movie_id;
   }

   std::string get_movie_name() {
     return movie_name;
   }

   int get_timestamp() {
     return timestamp;
   }

   std::string get_director_name() {
     return director_name;
   }

   std::vector<std::string> get_categories() {
     return categories;
   }

   std::vector<std::string> get_actor_ids() {
     return actor_ids;
   }

   int get_rate_sum() {
     return rate_sum;
   }

   int get_nr_rates() {
     return nr_rates;
   }

   void modify_rate(int x) {
     rate_sum += x;
   }

   void modify_nr_rates(int x) {
     nr_rates += x;
   }

   void add_rate(std::string user_id, int rating) {
     user_rates[user_id] = rating;
   }

   int get_user_rate(std::string user_id) {
     return user_rates[user_id];
   }

 private:
   std::string movie_id;
   std::string movie_name;
   int timestamp;
   std::string director_name;
   std::vector<std::string> categories;
   std::vector<std::string> actor_ids;
   int rate_sum;
   int nr_rates;
   std::unordered_map<std::string, int> user_rates;

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
     first = INT_MAX;
     last = 0;
   }

   ~actor(){}

   std::string get_actor_id() {
     return actor_id;
   }

   int get_first() {
     return first;
   }

   int get_last() {
     return last;
   }

   void set_first(int x) {
     this -> first = x;
   }

   void set_last(int x) {
     this -> last = x;
   }

 private:
   std::string actor_id;
   std::string name;
   int first, last;

};

class director {
  public:
    director(){}
    
    director(std::string director_name) {
      this -> director_name = director_name;
    }

    ~director(){}

    void add_known_actors(std::string actor_id) {
      known_actors[actor_id] = true;
    }

    int nr_known_actors() {
      return known_actors.size();
    }

  private:
    std::string director_name;
    std::unordered_map<std::string, bool> known_actors;

};

#endif  // SD_TEMA3_INCLUDE_CLASSES_H_
