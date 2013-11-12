#pragma once
#include "particle.h"
#include "Thurder.h"
#include <list>
using std::list;
//��Ӧ���ö�̬�ģ������Ѿ�д���ˣ����ø�
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
		//������Ч
        if (!emitters.empty()) {
            for (Emitter & e : emitters) {
                e.draw();
            }
            while (!emitters.empty()&&!emitters.front().isalive()) {
                emitters.pop_front();
            }
        }
		//���Ч��
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

