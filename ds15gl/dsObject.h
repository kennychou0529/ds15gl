#ifndef _DSOBJECT_H
#define _DSOBJECT_H

#include "dsModel.h"
#include <vector>

class dsObject {
public:
    // �� XML �ļ�������ģ����
    void load();

    // ����
    void render();

private:
    std::vector<MD2Model> models;
};

#endif
