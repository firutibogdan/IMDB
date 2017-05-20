/* Copyright 2017 Andrei Petre */

#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <cmath>

#include "include/imdb.h"
#include "include/classes.h"
#include "include/bst.h"

IMDb::IMDb() {
    // initialize what you need here.
}

IMDb::~IMDb() {}

void IMDb::add_movie(std::string movie_name,
   std::string movie_id,
   int timestamp,  // unix timestamp when movie was launched
   std::vector<std::string> categories,
   std::string director_name,
   std::vector<std::string> actor_ids) {

     movie new_movie(movie_name, movie_id, timestamp, categories,
       director_name, actor_ids);
       movies.insertKey(new_movie);
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
  mov -> add_rate(movie_id, rating);
  mov -> modify_rate(rating);
  mov ->modify_nr_rates(1);
}

void IMDb::update_rating(std::string user_id, std::string movie_id,
                         int rating) {
  movie* mov = movies.searchKey(movie_id);
  int past_rate = mov->get_user_rate(user_id);
  mov->modify_rate(-past_rate);
  mov->modify_rate(rating);
  mov->add_rate(user_id, rating);
}

void IMDb::remove_rating(std::string user_id, std::string movie_id) {
  movie *mov = this -> movies.searchKey(movie_id);
  int past_rate = mov -> get_user_rate(user_id);
  mov -> add_rate(movie_id, 0);
  mov -> modify_rate(- past_rate);
  mov -> modify_nr_rates(-1);
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
    rate /= 10;
    rate = double(rateint)/100;
    return std::to_string(rate);
}

std::string IMDb::get_longest_career_actor() {
    return "";
}

std::string IMDb::get_most_influential_director() {
    return "";
}

std::string IMDb::get_best_year_for_category(std::string category) {
    return "";
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
