#include <Geode/Geode.hpp>
#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyInfoLayer, InfoLayer) {
    // Usamos 'onLikeItem' que es el nombre más común en bindings de Android para 2.2081
    void onLikeItem(CCObject* sender) {
        // Tag 1 = Like, Tag 0 o diferente = Dislike
        int tag = sender->getTag();
        bool isLike = (tag == 1);

        // Ajustes
        bool shouldConfirmLike = Mod::get()->getSettingValue<bool>("confirm-like");
        bool shouldConfirmDislike = Mod::get()->getSettingValue<bool>("confirm-dislike");

        // Si el sender tiene un tag especial (p.ej. 69), es nuestra propia llamada de confirmación
        if (tag == 69 || tag == 70) {
            // Restauramos el tag original y ejecutamos
            sender->setTag(tag == 69 ? 1 : 0);
            InfoLayer::onLikeItem(sender);
            return;
        }

        if ((isLike && shouldConfirmLike) || (!isLike && shouldConfirmDislike)) {
            std::string levelName = m_level->m_levelName;
            std::string accion = isLike ? "dar <cg>Like</c>" : "dar <cr>Dislike</c>";

            auto alert = FLAlertLayer::create(
                this, 
                "Confirmar", 
                "¿Seguro que quieres " + accion + " a <cy>" + levelName + "</c>?", 
                "No", 
                "Sí"
            );
            
            // Guardamos 69 para Like y 70 para Dislike para identificarlo en el callback
            alert->setTag(isLike ? 69 : 70); 
            alert->show();
        } else {
            InfoLayer::onLikeItem(sender);
        }
    }

    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        if (btn2) {
            // Creamos el sender ficticio con nuestro tag de control
            auto dummy = CCNode::create();
            dummy->setTag(alert->getTag());
            this->onLikeItem(dummy);
        }
    }
};
