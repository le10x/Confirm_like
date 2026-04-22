#include <Geode/Geode.hpp>
#include <Geode/modify/InfoLayer.hpp>

using namespace geode::prelude;

class $modify(MyInfoLayer, InfoLayer) {
    void onLike(CCObject* sender) {
        // Obtenemos el tag: 1 para Like, 0 para Dislike
        int tag = sender->getTag();
        bool isLike = (tag == 1);

        // Leemos la configuración
        bool shouldConfirmLike = Mod::get()->getSettingValue<bool>("confirm-like");
        bool shouldConfirmDislike = Mod::get()->getSettingValue<bool>("confirm-dislike");

        // Verificamos si la configuración pide confirmación para esta acción
        if ((isLike && shouldConfirmLike) || (!isLike && shouldConfirmDislike)) {
            
            // Obtenemos el nombre del nivel desde la variable interna de InfoLayer
            std::string levelName = m_level->m_levelName;
            
            // Construimos el mensaje dinámico
            std::string accion = isLike ? "dar <cg>Like</c>" : "dar <cr>Dislike</c>";
            std::string mensaje = "¿Estás seguro de que quieres " + accion + " a <cy>" + levelName + "</c>?";

            auto alert = FLAlertLayer::create(
                "Confirmar Voto", 
                mensaje, 
                "Cancelar", 
                "Aceptar"
            );
            
            alert->setTargetLayer(this);
            alert->setTag(tag); // Guardamos el tipo de voto para el callback
            alert->show();
        } else {
            // Si la opción está desactivada, votar directamente
            InfoLayer::onLike(sender);
        }
    }

    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        if (btn2) {
            // Creamos un CCNode temporal con el tag correcto para pasárselo a la función original
            auto dummy = CCNode::create();
            dummy->setTag(alert->getTag());
            InfoLayer::onLike(dummy);
        }
    }
};
