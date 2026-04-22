#include <Geode/Geode.hpp>
#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyInfoLayer, InfoLayer) {
    // Usamos el nombre estándar 'onLike'
    void onLike(CCObject* sender) {
        // Obtenemos el tag: 1 es Like, 0 es Dislike
        int tag = sender->getTag();
        bool isLike = (tag == 1);

        // Leemos la configuración
        bool shouldConfirmLike = Mod::get()->getSettingValue<bool>("confirm-like");
        bool shouldConfirmDislike = Mod::get()->getSettingValue<bool>("confirm-dislike");

        // Si la configuración coincide con la acción, pedimos confirmación
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
            
            alert->setTag(tag);
            alert->show();
        } else {
            // USAMOS ESTA SINTAXIS PARA EVITAR EL ERROR DE COMPILACIÓN
            m_fields->m_confirming = false; // (opcional)
            return InfoLayer::onLike(sender);
        }
    }

    // Manejador del clic en la alerta
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        if (btn2) {
            // Si presionó "Sí", llamamos a la función original pasándole el tag
            auto dummy = CCNode::create();
            dummy->setTag(alert->getTag());
            this->onLike(dummy);
        }
    }
};
