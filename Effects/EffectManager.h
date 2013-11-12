#pragma once
#include "particle.h"
#include "Thurder.h"
#include <list>
using std::list;
//本应该用多态的，但我已经写好了，懒得改
class EffectManager {
public:
    EffectManager(void);
    ~EffectManager(void);
    void addEmitter(Emitter& e) {
		e.setPool(&pool);
        emitters.push_back(e);

    }
    void addThurder(Thurder& t) {
        thurders.push_back(t);
    }
    void render() {
		//粒子特效
        if (!emitters.empty()) {
            for (Emitter & e : emitters) {
                e.draw();
            }
            while (!emitters.empty()&&!emitters.front().isalive()) {
                emitters.pop_front();
            }
        }
		//点击效果
        if (!thurders.empty()) {
            for (Thurder t : thurders) {
                t.draw();
            }
            while (!thurders.empty()&&!thurders.front().alive()) {
                thurders.pop_front();
            }

        }
    }
private:
	ParticlePool pool;
    list<Emitter> emitters;
    list<Thurder> thurders;
	
};

