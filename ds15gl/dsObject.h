#ifndef _DSOBJECT_H
#define _DSOBJECT_H

#include "dsModel.h"
#include <vector>
#include <map>
#include <utility>

// ר�����������ͼ�ϵľ�̬����
class DSObject {
public:
    // �� XML �ļ�������ģ����
    void load(const std::string& object_name);

    // ����
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
