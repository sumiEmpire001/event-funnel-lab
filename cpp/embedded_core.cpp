#include <stdint.h>
#include <stddef.h>
#include <limits.h>
namespace embedded_funnel {
enum Status { OK, INVALID, CAPACITY, OUT_OF_ORDER };
struct Event { uint32_t user; uint8_t stage; uint32_t tick; };
template<size_t USERS,size_t STAGES> class Funnel {
 struct User {uint32_t id;uint32_t last_tick;uint8_t next;};
 User users_[USERS];size_t used_;uint32_t counts_[STAGES];bool fault_;
public:
 Funnel():used_(0),fault_(false){for(size_t i=0;i<STAGES;++i)counts_[i]=0;}
 Status ingest(Event e){
  if(fault_)return INVALID;
  if(!e.user||e.stage>=STAGES){fault_=true;return INVALID;}
  size_t i=0;for(;i<used_;++i)if(users_[i].id==e.user)break;
  if(i==used_){if(used_==USERS){fault_=true;return CAPACITY;}users_[used_++]=User{e.user,0,0};}
  User &u=users_[i];
  if(e.tick<u.last_tick){fault_=true;return OUT_OF_ORDER;}
  u.last_tick=e.tick;
  if(e.stage==u.next){if(counts_[e.stage]==UINT32_MAX){fault_=true;return CAPACITY;}++counts_[e.stage];++u.next;}
  return OK;
 }
 uint32_t reached(size_t stage)const{return stage<STAGES?counts_[stage]:0;}
 bool faulted()const{return fault_;}
 void reset(){used_=0;fault_=false;for(size_t i=0;i<STAGES;++i)counts_[i]=0;}
};
}
#ifdef EMBEDDED_TEST
#include <assert.h>
int main(){using namespace embedded_funnel;Funnel<2,3> f;assert(f.ingest({1,0,10})==OK);assert(f.ingest({1,0,11})==OK);assert(f.ingest({1,2,12})==OK);assert(f.ingest({1,1,13})==OK);assert(f.reached(0)==1&&f.reached(1)==1&&f.reached(2)==0);assert(f.ingest({1,2,12})==OUT_OF_ORDER);assert(f.faulted());f.reset();assert(f.ingest({2,0,0})==OK);assert(f.ingest({3,0,0})==OK);assert(f.ingest({4,0,0})==CAPACITY);return 0;}
#endif
