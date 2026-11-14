#pragma once
#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
namespace sumi {
class Funnel {
 std::vector<std::string> stages_;
 struct Progress {std::size_t next=0;std::int64_t last=0;bool started=false;};
 std::map<std::string,Progress> users_;
 std::vector<unsigned> counts_;
 std::size_t maxUsers_;
public:
 explicit Funnel(std::vector<std::string> stages,std::size_t maxUsers=128):stages_(stages),counts_(stages.size(),0),maxUsers_(maxUsers){
  if(stages.empty()||stages.size()>16||!maxUsers||maxUsers>4096)throw std::invalid_argument("invalid limits");
  for(std::size_t i=0;i<stages.size();++i){if(stages[i].empty()||stages[i].size()>32)throw std::invalid_argument("stage");
   for(std::size_t j=0;j<i;++j)if(stages[i]==stages[j])throw std::invalid_argument("duplicate stage");}
 }
 bool ingest(const std::string& user,const std::string& stage,std::int64_t timestamp){
  if(user.empty()||user.size()>32||stage.empty())return false;
  auto it=users_.find(user);if(it==users_.end()){if(users_.size()>=maxUsers_)return false;it=users_.emplace(user,Progress{}).first;}
  Progress &p=it->second;
  if(p.started&&timestamp<p.last)return false;
  p.started=true;p.last=timestamp;
  if(p.next<stages_.size()&&stage==stages_[p.next]){++counts_[p.next];++p.next;return true;}
  return false;
 }
 const std::vector<unsigned>& counts()const{return counts_;}
 std::size_t users()const{return users_.size();}
};
}
