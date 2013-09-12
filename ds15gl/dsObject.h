#ifndef _DSOBJECT_H
#define _DSOBJECT_H

#include "dsModel.h"
#include <vector>

class dsObject {
public:
    // �� XML �ļ�������ģ����
    void load(const std::string& object_name);

    // ����
    void render();

private:
    std::vector<MD2Model> models;
    GLfloat translate;
    GLfloat scale;
};

#endif
