#include <Geode/Geode.hpp>
#include <Geode/modify/LikeItemLayer.hpp>

using namespace geode::prelude;

class $modify(LikeItemLayerHook, LikeItemLayer) {
    struct Fields {
        bool m_isProcessing = false; // Cambiado a un nombre más genérico para evitar conflictos
    };

    static void onModify(auto& self) {
        // En 2.2081, asegúrate de usar los nombres correctos de las funciones del SDK
        (void)self.setHookPriority("LikeItemLayer::onLike", Priority::First);
        (void)self.setHookPriority("LikeItemLayer::onDislike", Priority::First);
    }

    void onLike(CCObject* sender) {
        // Si el mod está desactivado en settings o ya estamos procesando el click, ejecutar normal
        if (!Mod::get()->getSettingValue<bool>("like") || m_fields->m_isProcessing) {
            m_fields->m_isProcessing = false;
            LikeItemLayer::onLike(sender);
            return;
        }

        m_fields->m_isProcessing = true;
        
        // Usamos una referencia al sender para el callback
        geode::createQuickPopup(
            "Confirm Like", 
            "Are you sure you want to like this " + getItemType() + "? You cannot undo this.", 
            "Cancel", "Ok",
            [this, sender](auto, bool confirmed) {
                if (confirmed) {
                    this->onLike(sender);
                } else {
                    m_fields->m_isProcessing = false;
                    if (Mod::get()->getSettingValue<bool>("close")) {
                        this->onClose(sender);
                    }
                }
            }
        );
    }

    void onDislike(CCObject* sender) {
        if (!Mod::get()->getSettingValue<bool>("dislike") || m_fields->m_isProcessing) {
            m_fields->m_isProcessing = false;
            LikeItemLayer::onDislike(sender);
            return;
        }

        m_fields->m_isProcessing = true;
        
        geode::createQuickPopup(
            "Confirm Dislike", 
            "Are you sure you want to dislike this " + getItemType() + "? You cannot undo this.", 
            "Cancel", "Ok",
            [this, sender](auto, bool confirmed) {
                if (confirmed) {
                    this->onDislike(sender);
                } else {
                    m_fields->m_isProcessing = false;
                    if (Mod::get()->getSettingValue<bool>("close")) {
                        this->onClose(sender);
                    }
                }
            }
        );
    }

    std::string getItemType() {
        // m_itemType sigue funcionando igual en la 2.2081
        switch(m_itemType) {
            case LikeItemType::Level: return "level";
            case LikeItemType::LevelList: return "list";
            default: return "comment";
        }
    }
};
