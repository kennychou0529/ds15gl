#ifndef _DSOBJECT_H
#define _DSOBJECT_H

#include "dsModel.h"
#include <vector>

class dsObject {
public:
    // 从 XML 文件中载入模型们
    void load(const std::string& object_name);

    // 绘制
    void render();

private:
    std::vector<MD2Model> models;
    GLfloat translate;
    GLfloat scale;
};

#endif
