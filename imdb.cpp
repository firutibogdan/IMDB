/* Copyright 2017 Andrei Petre */

#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <cmath>
#include <climits>
#include <ctime>
#include "include/imdb.h"
#include "include/classes.h"
#include "include/bst.h"
#include "include/treap.h"

IMDb::IMDb() {
    // initialize what you need here.
    activity = new Treap<std::string, int>;
    directors = new Treap<std::string, int>;
}

IMDb::~IMDb() {
  delete activity;
  delete directors;
  for(auto it : categ) {
    delete it.second;
  }
}

void IMDb::add_movie(std::string movie_name,
   std::string movie_id,
   int timestamp,  // unix timestamp when movie was launched
   std::vector<std::string> categories,
   std::string director_name,
   std::vector<std::string> actor_ids) {
     movie new_movie(movie_name, movie_id, timestamp, categories,
       director_name, actor_ids);
     movies.insertKey(new_movie);

     for(auto it : actor_ids) {
       actor *act = this -> actor_bs(it, 0, actors.size() - 1);
       if(act -> get_first() == INT_MAX) {
         act -> set_first(timestamp);
         act -> set_last(timestamp);
         this -> activity -> insert(activity, it, 0);
       } else {
         if(act -> get_first() > timestamp) {
           act -> set_first(timestamp);
           this -> activity -> erase(activity, it);
           this -> activity -> insert(activity, it, act -> get_last() - timestamp);
         } else if(act -> get_last() < timestamp) {
           act -> set_last(timestamp);
           this -> activity -> erase(activity, it);
           this -> activity -> insert(activity, it, timestamp - act -> get_first());
         }
       }
     }

     if(this -> directors -> find(director_name)) {

       int acts = directors -> get_priority(director_name);
       director old_director = d_info[director_name];
       for(auto it : actor_ids) {
         old_director.add_known_actors(it);
       }
       if(old_director.nr_known_actors() != acts) {
         d_info[director_name] = old_director;
         directors -> erase(directors, director_name);
         directors -> insert(directors, director_name, old_director.nr_known_actors());
       }

     } else {

       director new_director(director_name);
       for(auto it : actor_ids) {
         new_director.add_known_actors(it);
       }
       d_info[director_name] = new_director;
       directors -> insert(directors, director_name, actor_ids.size());

     }

     for(auto it : categories) {
       if(categ[it] == nullptr) {
         categ[it] = new Treap<int, double>;
       }
     }
}

void IMDb::add_user(std::string user_id, std::string name) {
  user new_user(user_id, name);
  if(this -> users.empty()) {
    this -> users.push_back(new_user);
    return ;
  }
  int index = this -> users.size();
  if(user_id > this -> users[index - 1].get_user_id()) {
    this -> users.push_back(new_user);
  } else {
    for(auto it = this -> users.begin(); it != this -> users.end(); ++it) {
      if(it -> get_user_id() > user_id) {
        this -> users.insert(it, new_user);
        break;
      }
    }
  }
}

void IMDb::add_actor(std::string actor_id, std::string name) {

    actor new_actor(actor_id, name);
    if(actors.empty()) {
      actors.push_back(new_actor);
      return;
    }
    int index = actors.size();
    if(actor_id > actors[index - 1].get_actor_id()) {
      actors.push_back(new_actor);
    } else {
      for(auto it = actors.begin(); it != actors.end(); ++it) {
        if (it->get_actor_id() > actor_id) {
          actors.insert(it, new_actor);
          break;
        }
    }
  }
}

void IMDb::add_rating(std::string user_id, std::string movie_id, int rating) {
  movie *mov = this -> movies.searchKey(movie_id);
  std::string old_rating = get_rating(movie_id);
  double d_rate;
  if(old_rating == "none") {
    d_rate = 0;
  } else {
    d_rate = std::stod(old_rating);
  }
  mov -> add_rate(user_id, rating);
  mov -> modify_rate(rating);
  mov -> modify_nr_rates(1);

  std::vector<std::string> cat_list = mov -> get_categories();
  int year = mov -> get_timestamp();
  time_t startTime_t = (time_t) year;
  std::string startTime = asctime(localtime(&startTime_t));
  startTime = startTime.substr(startTime.find('\n') - 4, 4);
  year = std::stoi(startTime);

  for(auto it : cat_list) {
    if(categ[it] -> find(year)) {
      std::unordered_map<std::string, bool> aux = categ[it] -> get_hash(year);
      double old_rate = categ[it] -> get_priority(year);

      aux[movie_id] = 1;
      old_rate *= aux.size();
      old_rate -= d_rate;
      old_rate += std::stod(get_rating(movie_id));

      categ[it] -> erase(categ[it], year);
      categ[it] -> insert(categ[it], year, old_rate / double(aux.size()));
      categ[it] -> set_hash(year, aux);

    } else {
      categ[it] -> insert(categ[it], year, std::stod(get_rating(movie_id)));
      std::unordered_map<std::string, bool> aux;
      aux[movie_id] = 1;
      categ[it] -> set_hash(year, aux);
    }
  }
}

void IMDb::update_rating(std::string user_id, std::string movie_id,
                         int rating) {
  movie* mov = movies.searchKey(movie_id);
  int past_rate = mov->get_user_rate(user_id);
  std::string old_rating = get_rating(movie_id);
  double d_rate;
  if(old_rating == "none") {
    d_rate = 0;
  } else {
    d_rate = std::stod(old_rating);
  }
  mov->modify_rate(-past_rate);
  mov->modify_rate(rating);
  mov->add_rate(user_id, rating);

  std::vector<std::string> cat_list = mov -> get_categories();
  int year = mov -> get_timestamp();
  time_t startTime_t = (time_t) year;
  std::string startTime = asctime(localtime(&startTime_t));
  startTime = startTime.substr(startTime.find('\n') - 4, 4);
  year = std::stoi(startTime);

  for(auto it : cat_list) {
    std::unordered_map<std::string, bool> aux = categ[it] -> get_hash(year);
    double old_rate = categ[it] -> get_priority(year);

    old_rate *= aux.size();
    old_rate -= d_rate;
    old_rate += std::stod(get_rating(movie_id));

    categ[it] -> erase(categ[it], year);
    categ[it] -> insert(categ[it], year, old_rate / double(aux.size()));
    categ[it] -> set_hash(year, aux);
  }
}

void IMDb::remove_rating(std::string user_id, std::string movie_id) {
  movie *mov = this -> movies.searchKey(movie_id);
  int past_rate = mov -> get_user_rate(user_id);
  std::string old_rating = get_rating(movie_id);
  double d_rate;
  if(old_rating == "none") {
    d_rate = 0;
  } else {
    d_rate = std::stod(old_rating);
  }
  mov -> add_rate(movie_id, 0);
  mov -> modify_rate(- past_rate);
  mov -> modify_nr_rates(-1);

  std::vector<std::string> cat_list = mov -> get_categories();
  int year = mov -> get_timestamp();
  time_t startTime_t = (time_t) year;
  std::string startTime = asctime(localtime(&startTime_t));
  startTime = startTime.substr(startTime.find('\n') - 4, 4);
  year = std::stoi(startTime);

  for(auto it : cat_list) {
    std::unordered_map<std::string, bool> aux = categ[it] -> get_hash(year);
    double old_rate = categ[it] -> get_priority(year);

    old_rate *= aux.size();
    old_rate -= d_rate;
    if(mov -> get_nr_rates() != 0) {
      old_rate += std::stod(get_rating(movie_id));
    } else {
      aux.erase(movie_id);
    }

    categ[it] -> erase(categ[it], year);
    if(aux.size() != 0) {
      categ[it] -> insert(categ[it], year, old_rate / double(aux.size()));
      categ[it] -> set_hash(year, aux);
    }
  }
}

std::string IMDb::get_rating(std::string movie_id) {
  movie *mov = movies.searchKey(movie_id);
    int rate_sum = mov->get_rate_sum();
    int nr_rates = mov->get_nr_rates();
    if(nr_rates == 0)
    return "none";
    double rate = double(rate_sum)/double(nr_rates);
    rate *=1000;
    int rateint = floor(rate);
    if(rateint%10 >= 5) rateint+=10;
    rateint /= 10;
    rate = double(rateint)/100;
    std::string result = std::to_string(rate);
    if(result[1] == '.') result.resize(4);
    else result.resize(5);
    return result;
}

std::string IMDb::get_longest_career_actor() {
    if(this -> activity -> isNil()) {
      return "none";
    }
    return this -> activity -> peek();
}

std::string IMDb::get_most_influential_director() {
  if(this -> directors -> isNil()) {
    return "none";
  }
  return this -> directors -> peek();
}

std::string IMDb::get_best_year_for_category(std::string category) {
  if(categ[category] == nullptr) {
    return "none";
  } else {
    if(this -> categ[category] -> isNil()) {
      return "none";
    }
    return std::to_string(this -> categ[category] -> peek());
  }
}

std::string IMDb::get_2nd_degree_colleagues(std::string actor_id) {
    return "";
}

std::string IMDb::get_top_k_most_recent_movies(int k) {
    return "";
}

std::string IMDb::get_top_k_actor_pairs(int k) {
    return "";
}

std::string IMDb::get_top_k_partners_for_actor(int k, std::string actor_id) {
    return "";
}

std::string IMDb::get_top_k_most_popular_movies(int k) {
    return "";
}

std::string IMDb::get_avg_rating_in_range(int start, int end) {
    return "";
}

actor *IMDb::actor_bs(std::string actor_id, int left, int right) {
  if(left > right) {
    return nullptr;
  }
  int mij = (left + right) / 2;
  if(actors[mij].get_actor_id() == actor_id) {
    return &actors[mij];
  }
  if(actors[mij].get_actor_id() < actor_id) {
    return actor_bs(actor_id, mij + 1, right);
  } else {
    return actor_bs(actor_id, left, mij - 1);
  }
}
