#include <Geode/Geode.hpp>
#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyInfoLayer, InfoLayer) {
    void onLike(CCObject* sender) {
        int tag = sender->getTag();
        bool isLike = (tag == 1);

        bool shouldConfirmLike = Mod::get()->getSettingValue<bool>("confirm-like");
        bool shouldConfirmDislike = Mod::get()->getSettingValue<bool>("confirm-dislike");

        // Si el tag es 69 o 70, significa que ya confirmamos y queremos ejecutar la acción original
        if (tag == 69 || tag == 70) {
            sender->setTag(tag == 69 ? 1 : 0);
            return InfoLayer::onLike(sender);
        }

        // Mostrar alerta según la configuración
        if ((isLike && shouldConfirmLike) || (!isLike && shouldConfirmDislike)) {
            std::string actionType = isLike ? "like" : "dislike";
            std::string message = "Are you sure you want to " + actionType + "?";

            auto alert = FLAlertLayer::create(
                this, 
                "Confirm", 
                message, 
                "Cancel", 
                "Yes"
            );
            
            // Usamos 69 para Like y 70 para Dislike como identificadores internos
            alert->setTag(isLike ? 69 : 70);
            alert->show();
        } else {
            InfoLayer::onLike(sender);
        }
    }

    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        if (btn2) {
            auto dummy = CCNode::create();
            dummy->setTag(alert->getTag());
            this->onLike(dummy);
        }
    }
};
