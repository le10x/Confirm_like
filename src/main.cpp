#include <Geode/Geode.hpp>
#include <Geode/modify/LikeItemLayer.hpp>

using namespace geode::prelude;

class $modify(MyLikeItemLayer, LikeItemLayer) {
    // Definimos Fields para evitar el error de recursión infinita
    struct Fields {
        bool m_isConfirming = false;
    };

    void onLike(CCObject* sender) {
        // Si el ajuste está apagado o ya confirmamos, ejecutamos original
        if (!Mod::get()->getSettingValue<bool>("confirm-like") || m_fields->m_isConfirming) {
            m_fields->m_isConfirming = false;
            return LikeItemLayer::onLike(sender);
        }

        // Marcamos que estamos en proceso de confirmación
        m_fields->m_isConfirming = true;

        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to like?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    this->onLike(sender);
                } else {
                    // Si cancela, reseteamos la bandera
                    m_fields->m_isConfirming = false;
                }
            }
        );
    }

    void onDislike(CCObject* sender) {
        if (!Mod::get()->getSettingValue<bool>("confirm-dislike") || m_fields->m_isConfirming) {
            m_fields->m_isConfirming = false;
            return LikeItemLayer::onDislike(sender);
        }

        m_fields->m_isConfirming = true;

        geode::createQuickPopup(
            "Confirm", 
            "Are you sure you want to dislike?", 
            "Cancel", "Yes",
            [this, sender](auto, bool btn2) {
                if (btn2) {
                    this->onDislike(sender);
                } else {
                    m_fields->m_isConfirming = false;
                }
            }
        );
    }
};
