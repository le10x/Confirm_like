#include <Geode/Geode.hpp>
#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

// Modificamos la clase InfoLayer para interceptar el botón de Like
class $modify(MyInfoLayer, InfoLayer) {
    
    // Sobrescribimos la función que se ejecuta al presionar Like
    // Nota: El nombre de la función puede variar según los headers (onLike es común)
    void onLike(CCObject* sender) {
        // Creamos una alerta de confirmación
        auto alert = FLAlertLayer::create(
            "Confirmar", 
            "¿Estás seguro de que quieres dar <cg>Like</c> a este nivel?", 
            "Cancelar", 
            "Aceptar"
        );
        
        // Configuramos el "Target" para que, al presionar "Aceptar", se llame a la función original
        alert->setTargetLayer(this);
        // El tag ayuda a identificar la acción en el callback
        alert->setTag(69); 
        alert->show();
    }

    // Callback que recibe la respuesta del FLAlertLayer
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        // btn2 es true si se presionó el botón de la derecha ("Aceptar")
        if (btn2 && alert->getTag() == 69) {
            // Llamamos a la función original de la clase base
            InfoLayer::onLike(nullptr);
        }
    }
};
