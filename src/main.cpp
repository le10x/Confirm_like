#include <Geode/Geode.hpp>
#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyInfoLayer, InfoLayer) {
    // En 2.2081, el nombre correcto suele ser onLikeItem
    void onLikeItem(CCObject* sender) {
        int tag = sender->getTag();
        bool isLike = (tag == 1);

        bool shouldConfirmLike = Mod::get()->getSettingValue<bool>("confirm-like");
        bool shouldConfirmDislike = Mod::get()->getSettingValue<bool>("confirm-dislike");

        if ((isLike && shouldConfirmLike) || (!isLike && shouldConfirmDislike)) {
            std::string levelName = m_level->m_levelName;
            std::string accion = isLike ? "dar <cg>Like</c>" : "dar <cr>Dislike</c>";
            std::string mensaje = "¿Seguro que quieres " + accion + " a <cy>" + levelName + "</c>?";

            // CORRECCIÓN: Se requiere pasar 'this' como primer argumento (delegate)
            auto alert = FLAlertLayer::create(
                this, 
                "Confirmar", 
                mensaje, 
                "No", 
                "Sí"
            );
            
            alert->setTag(tag);
            alert->show();
        } else {
            // Llamada original corregida
            InfoLayer::onLikeItem(sender);
        }
    }

    // El delegate ahora funciona correctamente porque pasamos 'this' arriba
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        if (btn2) {
            auto dummy = CCNode::create();
            dummy->setTag(alert->getTag());
            InfoLayer::onLikeItem(dummy);
        }
    }
};
