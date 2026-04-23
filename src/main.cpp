#include <Geode/Geode.hpp>
#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyInfoLayer, InfoLayer) {
    // Usamos la firma exacta que espera el compilador para evitar el error de 'no member'
    void onLike(CCObject* sender) {
        int tag = sender->getTag();
        bool isLike = (tag == 1);

        // Si el tag es de confirmación (69 o 70), ejecutamos la función original directamente
        if (tag == 69 || tag == 70) {
            sender->setTag(tag == 69 ? 1 : 0);
            return InfoLayer::onLike(sender);
        }

        // Leer configuraciones
        bool shouldConfirmLike = Mod::get()->getSettingValue<bool>("confirm-like");
        bool shouldConfirmDislike = Mod::get()->getSettingValue<bool>("confirm-dislike");

        if ((isLike && shouldConfirmLike) || (!isLike && shouldConfirmDislike)) {
            std::string action = isLike ? "like" : "dislike";
            
            auto alert = FLAlertLayer::create(
                this, 
                "Confirm", 
                "Are you sure you want to " + action + "?", 
                "Cancel", 
                "Yes"
            );
            
            alert->setTag(isLike ? 69 : 70);
            alert->show();
        } else {
            // Si no hay confirmación, llamar a la original
            InfoLayer::onLike(sender);
        }
    }

    // Manejador del botón de la alerta
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        if (btn2) {
            auto dummy = CCNode::create();
            dummy->setTag(alert->getTag());
            this->onLike(dummy);
        }
    }
};
