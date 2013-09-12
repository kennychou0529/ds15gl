#include "dsObject.h"
#include "tinyxml2.h"

void dsObject::load(const std::string& object_name) {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("objects.xml");

    // Find the root element <objects>
    auto xml_root = doc.FirstChildElement();

    // Try to find the node <object name="...">
    auto xml_object = xml_root->FirstChildElement("object");
    for (; xml_object != nullptr; xml_object = xml_object->NextSiblingElement()) {
        if (xml_object->Attribute(object_name.c_str()) == object_name) {
            break;
        }
    }

    if (xml_object == nullptr) {
        throw "Cannot find object in objects.xml";
    }

    for (
        auto xml_model = xml_object->FirstChildElement("model");
        xml_model != nullptr;
        xml_model = xml_model->NextSiblingElement()
    ) {
        models.push_back(MD2Model());
        auto new_model = models.end() - 1;
        new_model->load(
            xml_model->FirstChildElement("tris")->GetText(),
            xml_model->FirstChildElement("skin")->GetText()
        );
    }

}

void dsObject::render() {
    glPushMatrix();
    for (auto & model : models) {
        model.renderFrame(0);
    }
    glPopMatrix();
}
