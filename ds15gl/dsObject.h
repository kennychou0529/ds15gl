#ifndef _DSOBJECT_H
#define _DSOBJECT_H

#include "dsModel.h"
#include <vector>

class dsObject {
public:
    // 从 XML 文件中载入模型们
    void load();

    // 绘制
    void render();

private:
    std::vector<MD2Model> models;
};

#endif
