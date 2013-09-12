#ifndef _DSOBJECT_H
#define _DSOBJECT_H

#include "dsModel.h"
#include <vector>
#include <map>
#include <utility>

// 专门用来处理地图上的静态物体
class DSObject {
public:
    // 从 XML 文件中载入模型们
    void load(const std::string& object_name);

    // 绘制
    void render() const;

private:
    std::vector<MD2Model> models;
    GLfloat translate;
    GLfloat scale;

};

class DSObjectManager {
public:
    void addObject(const std::string& object_name);

//private:
    std::map<std::string, DSObject> objects;

};

#endif
